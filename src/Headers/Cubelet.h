#ifndef _C_RUBE_CUBELET_H_
#define _C_RUBE_CUBELET_H_

typedef struct {
    float posW[3];
    float rotation[4]; // rotation internally represented by a quaternion
    float scale[3];
    
    bool stickers[6];
    
} Cubelet;

void InitCubeletArray(Cubelet *array, int numCubelets);
void DrawCubeletArray(Cubelet *array, int numCubelets, Camera *cam);
void BuildMatricies(Cubelet *singleCubelet, float wMtx[16], float witMtx[16]);
bool CheckCubelet(Cubelet *cubelet);

#endif