#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "Camera.h"
#include "Util.h"
#include "Cubelet.h"
#include "Shader.h"
#include "CubeletModel.h"
#include "Cube.h"

void InitCube (Cube *cube) {
    if (cube == NULL) return;
    
    memset(cube,0,sizeof(Cube));
    
    cube->rotation[3] = 1.0f;
    
    cube->scale[0] = 1.0f;
    cube->scale[1] = 1.0f;
    cube->scale[2] = 1.0f;
    
    cube->curShuffle = -1;
    
    InitCubeletArray(cube->cubelets,NUM_CUBELETS);
    _PositionCubelets(cube);
    
    StartShuffleSequence(cube,120);
}

void UpdateCube (Cube *cube, float dt) {
    if (cube == NULL) return;
    
    // check to see if there's an activate animation
    if (cube->curAnimation != NULL) {
        
        // if there is an active animation ... run it
        // and check to see if it's over
        if (UpdateSliceAnimation(cube->curAnimation,dt)) {
            
            // if it is over ... destroy it
            DestroySliceAnimation(cube->curAnimation);
            cube->curAnimation = NULL;
            
            // if we're shuffling than advance to the next animation
            // in the shuffle
            if (IsShuffling(cube)) {
                cube->curShuffle++;
                // check to see if there is a next animation in the shuffle sequence
                if (cube->curShuffle == cube->shuffleSize) {
                    DestroyShuffleSequence(cube);
                } else {
                    _StartSliceAnimation(cube, NULL, cube->shuffle[cube->curShuffle], false);
                }
            } else {
                if (CheckCubeForWin(cube) == true) {
                    printf("VICTORY!\n");
                }
            }
        }
    }
}

void RenderCube (Cube *cube, Camera *cam) {
    if (cube == NULL || cam == NULL) return;
    
    DrawCubeletArray(cube->cubelets, NUM_CUBELETS, cam);
}

void DestroyCube (Cube *cube) {
    if (cube == NULL) return;
    
    DestroySliceAnimation(cube->curAnimation);
    DestroyShuffleSequence(cube);
}

void InitNewSliceAnimation (
    SliceAnimation **sa,
    float            newPivotPoint[3],
    float            newPivotAxis [3],
    float            initialDegrees,
    float            newDegreesPerSecond,
    bool             sliceForward,
    Cubelet        **cubeletsToAnimate,
    int              numCubelets
) {
    SliceAnimation *curSA = NULL;
    int i = 0;
    
    if (
        sa                == NULL ||
        (*sa)             != NULL ||
        newPivotPoint     == NULL ||
        newPivotAxis      == NULL ||
        cubeletsToAnimate == NULL ||
        numCubelets        < 0    ||
        numCubelets        > 26
    ) return;
    
    curSA = calloc(1,sizeof(SliceAnimation));
    (*sa) = curSA;
    
    Vec3Copy(curSA->pivotPoint, newPivotPoint);
    Vec3Copy(curSA->pivotAxis, newPivotAxis);
    curSA->degreesRemaining = initialDegrees;
    curSA->cubelets = calloc(numCubelets,sizeof(Cubelet*));
    curSA->degreesPerSecond = newDegreesPerSecond;
    curSA->forward = sliceForward;
    curSA->numCubelets = numCubelets;
    
    for (i = 0; i < numCubelets; ++i) {
        curSA->cubelets[i] = cubeletsToAnimate[i];
    }
}

bool UpdateSliceAnimation (SliceAnimation *sa, float dt) {
    if (sa == NULL) return;
    
    bool result = false;
    float deltaDegrees = sa->degreesPerSecond * dt;
    float q[4] = {0.0f};
    int i;
    
    if (deltaDegrees > sa->degreesRemaining) {
        deltaDegrees = sa->degreesRemaining;
        sa->degreesRemaining = 0.0f;
        result = true;
    } else {
        sa->degreesRemaining -= deltaDegrees;
    }
    
    deltaDegrees *= (sa->forward == false) ? -1.0f : 1.0f;
    
    QuaternionFromAxisAngle(sa->pivotAxis[0], sa->pivotAxis[1], sa->pivotAxis[2], deltaDegrees, q);
    for (i = 0; i < sa->numCubelets; ++i) {
        QuaternionVec3Rotation(sa->cubelets[i]->posW,q,sa->cubelets[i]->posW);
        QuaternionMult(q,sa->cubelets[i]->rotation,sa->cubelets[i]->rotation);
    }
    
    return result;
}

void DestroySliceAnimation (SliceAnimation *sa) {
    if (sa == NULL) return;
    
    free(sa->cubelets);
    sa->cubelets = NULL;
    free(sa);
    sa = NULL;
}

bool CheckCubeForWin (Cube *cube) {
    int i;
    for (i = 0; i < NUM_CUBELETS; ++i) {
        if (CheckCubelet(&cube->cubelets[i]) == false) {
            return false;
        }
    }
    
    return true;
}

void _PositionCubelets (Cube *cube) {
    int i,x,y,z,curCubelet = 0;
    float offset = 2.2f;
    if (cube == NULL) return;
    
    for (x = 0; x < 3; ++x) {
        for (y = 0; y < 3; ++y) {
            for (z = 0; z < 3; ++z) {
                if (x == 1 && y == 1 && z == 1) continue;
                
                cube->cubelets[curCubelet].posW[0] = (float) (x-1) * offset;
                cube->cubelets[curCubelet].posW[1] = (float) (y-1) * offset;
                cube->cubelets[curCubelet].posW[2] = (float) (z-1) * offset;
                
                curCubelet++;
            }
        }
    }
    
    for (i = 0; i < NUM_CUBELETS; ++i) {
        if (cube->cubelets[i].posW[0] < -1.0f) {
            cube->cubelets[i].stickers[STICKER_NEG_X] = true;
        }
        if (cube->cubelets[i].posW[0] >  1.0f) {
            cube->cubelets[i].stickers[STICKER_POS_X] = true;
        }
        if (cube->cubelets[i].posW[1] < -1.0f) {
            cube->cubelets[i].stickers[STICKER_NEG_Y] = true;
        }
        if (cube->cubelets[i].posW[1] >  1.0f) {
            cube->cubelets[i].stickers[STICKER_POS_Y] = true;
        }
        if (cube->cubelets[i].posW[2] < -1.0f) {
            cube->cubelets[i].stickers[STICKER_NEG_Z] = true;
        }
        if (cube->cubelets[i].posW[2] >  1.0f) {
            cube->cubelets[i].stickers[STICKER_POS_Z] = true;
        }
    }
}

void _StartSliceAnimation(Cube *cube, Camera *cam, Slice slice, bool sliceForward) {
    SliceAnimation *sa = NULL;
    float pivotAxis[4] = {0.0f};
    float cubeletPos[4] = {0.0f,0.0f,0.0f,1.0f};
    Cubelet *cubelets[26] = {NULL};
    int i = 0, count = 0;
    float avm[16] = {0.0f};
    float iavm[16] = {0.0f};
    bool validInv = false;
    float rotationLength = DEFAULT_SLICE_ROTATION_LENGTH;
    
    if (cube == NULL || slice < SLICE_POS_X || slice > SLICE_WHOLE_CUBE) return;
    
    if (cam != NULL) {
        CopyAdjustedViewMtx(cam,avm);
        validInv = Mat4Inverse(iavm,avm);
    } else {
        Mat4Identity(avm);
        Mat4Identity(iavm);
        validInv = true;
    }
    
    if (validInv == false){
        fprintf(stderr, "Invalid matrix inverse detected!\n");
        return;
    }
    
    if (cube->curAnimation != NULL) return;
    
    if (slice == SLICE_POS_X) {
        pivotAxis[0] = 1.0f;
        for (i = 0; i < 26; ++i) {
            Vec3Copy(cubeletPos,cube->cubelets[i].posW);
            Mat4Vec4Mult(avm,cubeletPos,cubeletPos);
            if (cubeletPos[0] > 1.0f) {
                cubelets[count++] = &(cube->cubelets[i]);
                if (count == 9) break;
            }
        }
    } else if (slice == SLICE_NEG_X) {
        pivotAxis[0] = -1.0f;
        for (i = 0; i < 26; ++i) {
            Vec3Copy(cubeletPos,cube->cubelets[i].posW);
            Mat4Vec4Mult(avm,cubeletPos,cubeletPos);
            if (cubeletPos[0] < -1.0f) {
                cubelets[count++] = &(cube->cubelets[i]);
                if (count == 9) break;
            }
        }
    } else if (slice == SLICE_POS_Y) {
        pivotAxis[1] = 1.0f;
        for (i = 0; i < 26; ++i) {
            Vec3Copy(cubeletPos,cube->cubelets[i].posW);
            Mat4Vec4Mult(avm,cubeletPos,cubeletPos);
            if (cubeletPos[1] > 1.0f) {
                cubelets[count++] = &(cube->cubelets[i]);
                if (count == 9) break;
            }
        }
    } else if (slice == SLICE_NEG_Y) {
        pivotAxis[1] = -1.0f;
        for (i = 0; i < 26; ++i) {
            Vec3Copy(cubeletPos,cube->cubelets[i].posW);
            Mat4Vec4Mult(avm,cubeletPos,cubeletPos);
            if (cubeletPos[1] < -1.0f) {
                cubelets[count++] = &(cube->cubelets[i]);
                if (count == 9) break;
            }
        }
    } else if (slice == SLICE_POS_Z) {
        pivotAxis[2] = 1.0f;
        for (i = 0; i < 26; ++i) {
            Vec3Copy(cubeletPos,cube->cubelets[i].posW);
            Mat4Vec4Mult(avm,cubeletPos,cubeletPos);
            if (cubeletPos[2] > 1.0f) {
                cubelets[count++] = &(cube->cubelets[i]);
                if (count == 9) break;
            }
        }
    } else if (slice == SLICE_NEG_Z) {
        pivotAxis[2] = -1.0f;
        for (i = 0; i < 26; ++i) {
            Vec3Copy(cubeletPos,cube->cubelets[i].posW);
            Mat4Vec4Mult(avm,cubeletPos,cubeletPos);
            if (cubeletPos[2] < -1.0f) {
                cubelets[count++] = &(cube->cubelets[i]);
                if (count == 9) break;
            }
        }
    } else if (slice == SLICE_WHOLE_CUBE) {
        pivotAxis[0] = 1.0f;
        for (i = 0; i < 26; ++i) {
            cubelets[count++] = &(cube->cubelets[i]);
        }
        rotationLength = 180.0f;
    }
    
    if (
        (slice != SLICE_WHOLE_CUBE && count != 9) ||
        (slice == SLICE_WHOLE_CUBE && count != 26) 
    ) {
        fprintf(stderr,"Invalid slice selection!\n");
        exit(1);
    }
    
    Mat4Vec4Mult(iavm,pivotAxis,pivotAxis);
    
    InitNewSliceAnimation(
        &sa,
        pivotAxis,
        pivotAxis,
        rotationLength,
        DEFAULT_SLICE_SPEED,
        sliceForward,
        cubelets,
        count
    );
    
    if (sa != NULL) {
        cube->curAnimation = sa;
    }
}

void StartShuffleSequence (Cube *cube, int sizeOfShuffle) {
    int i;
    if (sizeOfShuffle < 1 || cube == NULL) return;
    
    srand(time(NULL));
    cube->shuffleSize = sizeOfShuffle;
    cube->curShuffle = 0;
    cube->shuffle = (int*) calloc(cube->shuffleSize, sizeof(int));
    
    for (i = 0; i < cube->shuffleSize; ++i) {
        cube->shuffle[i] = rand() % SLICE_WHOLE_CUBE;
    }
    
    _StartSliceAnimation(cube, NULL, cube->shuffle[0], false);
}

bool IsShuffling (Cube *cube) {
    if (cube == NULL) return false;
    return cube->curShuffle != -1;
}

void DestroyShuffleSequence (Cube *cube) {
    if (cube == NULL) return;
    
    if (cube->shuffle != NULL) {
        free(cube->shuffle);
        cube->shuffle = NULL;
    }
    cube->curShuffle = -1;
    cube->shuffleSize = 0;
}