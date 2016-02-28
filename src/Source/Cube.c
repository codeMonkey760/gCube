#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "Camera.h"
#include "Util.h"
#include "Cubelet.h"
#include "Shader.h"
#include "CubeletModel.h"
#include "Cube.h"

#define SHUFFLE_SIZE 60
static SliceAnimation *shuffle[SHUFFLE_SIZE];
static int curAnimation = 0;

void InitCube (Cube *cube) {
    if (cube == NULL) return;
    
    memset(cube,0,sizeof(Cube));
    
    cube->rotation[3] = 1.0f;
    
    cube->scale[0] = 1.0f;
    cube->scale[1] = 1.0f;
    cube->scale[2] = 1.0f;
    
    InitCubeletArray(cube->cubelets,NUM_CUBELETS);
    _PositionCubelets(cube);
    _InitShuffleSequence();
}

void UpdateCube (Cube *cube, float dt) {
    if (cube == NULL) return;
    
    // ============================
    // TEST CODE
    // =========================
    
    if (cube->curAnimation == NULL) {
        _SliceRotationTest(cube);
    }
    
    // ===========================
    // End TEST CODE
    // =========================
    
    if (cube->curAnimation != NULL) {
        if (UpdateSliceAnimation(cube->curAnimation,dt)) {
            DestroySliceAnimation(cube->curAnimation);
            cube->curAnimation = NULL;
        }
    }
}

void RenderCube (Cube *cube, Camera *cam) {
    if (cube == NULL || cam == NULL) return;
    
    DrawCubeletArray(cube->cubelets, NUM_CUBELETS, cam);
}

void InitNewSliceAnimation (
    SliceAnimation **sa,
    float            newPivotPoint[3],
    float            newPivotAxis [3],
    float            initialTheta,
    float            newRadiansPerSecond,
    Cubelet **cubeletsToAnimate,
    int numCubelets
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
    curSA->thetaRemaining = initialTheta;
    curSA->cubelets = calloc(numCubelets,sizeof(Cubelet*));
    curSA->radiansPerSecond = newRadiansPerSecond;
    curSA->numCubelets = numCubelets;
    
    for (i = 0; i < numCubelets; ++i) {
        curSA->cubelets[i] = cubeletsToAnimate[i];
    }
}

bool UpdateSliceAnimation (SliceAnimation *sa, float dt) {
    if (sa == NULL) return;
    
    bool result = false;
    float deltaTheta = sa->radiansPerSecond * dt;
    float q[4] = {0.0f};
    float negQ[4] = {0.0f};
    int i;
    
    if (deltaTheta > sa->thetaRemaining) {
        deltaTheta = sa->thetaRemaining;
        sa->thetaRemaining = 0.0f;
        result = true;
    } else {
        sa->thetaRemaining -= deltaTheta;
    }
    
    QuaternionFromAxisAngle(sa->pivotAxis[0], sa->pivotAxis[1], sa->pivotAxis[2], deltaTheta, q);
    QuaternionFromAxisAngle(sa->pivotAxis[0], sa->pivotAxis[1], sa->pivotAxis[2], -deltaTheta, negQ);
    for (i = 0; i < sa->numCubelets; ++i) {
        QuaternionVec3Rotation(sa->cubelets[i]->posW,q,sa->cubelets[i]->posW);
        QuaternionMult(negQ,sa->cubelets[i]->rotation,sa->cubelets[i]->rotation);
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

void _StartSliceAnimation(Cube *cube, Slice slice) {
    SliceAnimation *sa = NULL;
    float pivotAxis[3] = {0.0f};
    Cubelet *cubelets[9] = {NULL};
    int i = 0, count = 0;
    if (cube == NULL || slice < SLICE_POS_X || slice > SLICE_NEG_Z) return;
    
    if (cube->curAnimation != NULL) return;
    
    if (slice == SLICE_POS_X) {
        pivotAxis[0] = 1.0f;
        for (i = 0; i < 26; ++i) {
            if (cube->cubelets[i].posW[0] > 1.0f) {
                cubelets[count++] = &(cube->cubelets[i]);
                if (count == 9) break;
            }
        }
    } else if (slice == SLICE_NEG_X) {
        pivotAxis[0] = -1.0f;
        for (i = 0; i < 26; ++i) {
            if (cube->cubelets[i].posW[0] < -1.0f) {
                cubelets[count++] = &(cube->cubelets[i]);
                if (count == 9) break;
            }
        }
    } else if (slice == SLICE_POS_Y) {
        pivotAxis[1] = 1.0f;
        for (i = 0; i < 26; ++i) {
            if (cube->cubelets[i].posW[1] > 1.0f) {
                cubelets[count++] = &(cube->cubelets[i]);
                if (count == 9) break;
            }
        }
    } else if (slice == SLICE_NEG_Y) {
        pivotAxis[1] = -1.0f;
        for (i = 0; i < 26; ++i) {
            if (cube->cubelets[i].posW[1] < -1.0f) {
                cubelets[count++] = &(cube->cubelets[i]);
                if (count == 9) break;
            }
        }
    } else if (slice == SLICE_POS_Z) {
        pivotAxis[2] = 1.0f;
        for (i = 0; i < 26; ++i) {
            if (cube->cubelets[i].posW[2] > 1.0f) {
                cubelets[count++] = &(cube->cubelets[i]);
                if (count == 9) break;
            }
        }
    } else if (slice == SLICE_NEG_Z) {
        pivotAxis[2] = -1.0f;
        for (i = 0; i < 26; ++i) {
            if (cube->cubelets[i].posW[2] < -1.0f) {
                cubelets[count++] = &(cube->cubelets[i]);
                if (count == 9) break;
            }
        }
    }
    
    if (count != 9) {
        fprintf(stderr,"Invalid slice selection!\n");
        exit(1);
    }
    
    InitNewSliceAnimation(
        &sa,
        pivotAxis,
        pivotAxis, 
        DEFAULT_SLICE_ROTATION_LENGTH,
        DEFAULT_SLICE_SPEED,
        cubelets,
        9
    );
    
    if (sa != NULL) {
        cube->curAnimation = sa;
    }
}

void _InitShuffleSequence (void) {
    int i;
    for (i = 0; i < SHUFFLE_SIZE; ++i) {
        shuffle[i] = NULL;
    }
}

// =========================
// TEST CODE
// =========================

void _SliceRotationTest (Cube *cube) {
    if (cube == NULL) return;
    
    float pivotAxis[3] = {1.0f, 0.0f, 0.0f};
    Cubelet *cubelets[9] = {NULL};
    int i = 0, count = 0;
    
    for (i = 0; i < NUM_CUBELETS; ++i) {
        if (cube->cubelets[i].posW[0] < 0.0f) {
            cubelets[count++] = &(cube->cubelets[i]);
            if (count == 9) break;
        }
    }
    
    InitNewSliceAnimation(
        &(cube->curAnimation),
        pivotAxis,
        pivotAxis,
        M_PI / 2.0f,
        M_PI * 2.0f,
        &cubelets,
        9
    );
}