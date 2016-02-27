#ifndef _C_RUBE_CUBE_H_
#define _C_RUBE_CUBE_H_

#define NUM_CUBELETS 26

typedef struct {
    float posW[3];
    float rotation[4];
    float scale[3];
    
    Cubelet cubelets[NUM_CUBELETS];
}Cube;

typedef struct {
    Cubelet *cubelets;
    int numCubelets;
    
    float pivotPoint[3];
    float pivotAxis[3];
    float thetaRemaining;
    float radiansPerSecond;
    
} SliceAnimation;

void InitCube (Cube *cube);
void UpdateCube (Cube *cube, float dt);
void RenderCube (Cube *cube, Camera *cam);

void InitNewSliceAnimation (
    SliceAnimation **sa, 
    float newPivotPoint[3],
    float newPivotAxis[3],
    float initialTheta,
    float newRadiansPerSecond,
    Cubelet *cubeletsToAnimate,
    int numCubelets
);
bool UpdateSliceAnimation (SliceAnimation *sa, float dt);
void DestroySliceAnimation (SliceAnimation *sa);

void _PositionCubelets (Cube *cube);

#endif

