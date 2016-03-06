#ifndef _C_RUBE_GUIRENDERER_H_
#define _C_RUBE_GUIRENDERER_H_

typedef struct {
    GLuint buttonVBO;
} GUIRenderer;

extern GUIRenderer guiRenderer;

void InitGUIRenderer (void);
void DrawButtons (GUIButton *array, int numButtons);
void DestroyGUIRenderer (void);

void _BuildVBO (void);

#endif