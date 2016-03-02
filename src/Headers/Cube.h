#ifndef _C_RUBE_CUBE_H_
#define _C_RUBE_CUBE_H_

#define NUM_CUBELETS 26
#define DEFAULT_SLICE_SPEED ((M_PI) * 2.0f)
#define DEFAULT_SLICE_ROTATION_LENGTH ((M_PI) / 2.0f)

typedef struct {
    Cubelet **cubelets;
    int numCubelets;
    
    float pivotPoint[3];
    float pivotAxis[3];
    float thetaRemaining;
    float radiansPerSecond;
    
} SliceAnimation;

typedef struct {
    float posW[3];
    float rotation[4];
    float scale[3];
    
    Cubelet cubelets[NUM_CUBELETS];
    SliceAnimation *curAnimation;
}Cube;

typedef enum {
    SLICE_POS_X,
    SLICE_NEG_X,
    SLICE_POS_Y,
    SLICE_NEG_Y,
    SLICE_POS_Z,
    SLICE_NEG_Z
}Slice;

void InitCube (Cube *cube);
void UpdateCube (Cube *cube, float dt);
void RenderCube (Cube *cube, Camera *cam);

void InitNewSliceAnimation (
    SliceAnimation **sa, 
    float newPivotPoint[3],
    float newPivotAxis[3],
    float initialTheta,
    float newRadiansPerSecond,
    Cubelet **cubeletsToAnimate,
    int numCubelets
);
bool UpdateSliceAnimation (SliceAnimation *sa, float dt);
void DestroySliceAnimation (SliceAnimation *sa);

void _StartSliceAnimation (Cube *cube, Slice slice, Camera *cam);
void _InitShuffleSequence (void);

void _PositionCubelets (Cube *cube);

// ========================
// TEST CODE
// =======================
void _SliceRotationTest (Cube *cube);

#endif

