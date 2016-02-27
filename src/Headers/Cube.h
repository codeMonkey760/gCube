#ifndef _C_RUBE_CUBE_H_
#define _C_RUBE_CUBE_H_

typedef struct {
    float posW[3];
    float rotation[4];
    float scale[3];
    
    Cubelet cubelets[26];
}Cube;

void InitCube (Cube *cube);
void UpdateCube (Cube *cube, float dt);
void RenderCube (Cube *cube, Camera *cam);

#endif

