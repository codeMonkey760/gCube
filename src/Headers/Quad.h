#ifndef _C_RUBE_QUAD_H
#define _C_RUBE_QUAD_H

/* hmmm .... ideally, rotation would be stored as a four component
 * quaternion .... but I really really don't want to look up the math
 * for that .... :(
 */
typedef struct {
    float posW[3];
    float rightW[3];
    float upW[3];
    float lookW[3];
    
    float scale[3];
    float color[3];
} Quad;

void InitQuadArray(Quad *array, int numQuads);
void DrawQuadArray(Quad *array, int numQuads, Camera *cam);
void BuildMatricies(Quad *singleQuad, float wMtx[16], float witMtx[16]);

#endif /* QUAD_H */

