#ifndef _C_RUBE_CUBE_H_
#define _C_RUBE_CUBE_H_

#define NUM_CUBELETS 26

typedef struct {
    float posW[3];
    float rotation[4];
    float scale[3];
    
    Cubelet cubelets[NUM_CUBELETS];
}Cube;

void InitCube (Cube *cube);
void UpdateCube (Cube *cube, float dt);
void RenderCube (Cube *cube, Camera *cam);

void _PositionCubelets (Cube *cube);

#endif

