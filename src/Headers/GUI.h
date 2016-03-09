#ifndef _C_RUBE_GUI_H_
#define _C_RUBE_GUI_H_

#define GUI_NUM_BUTTONS 11

typedef struct {
    GUIButton buttons[GUI_NUM_BUTTONS];
    GUIButton *highlightedButton;
    Cube *cube;
    Camera *cam;
} GUI;

extern GUI gui;

void InitializeGUI (Cube *cube, Camera *cam);
void DestroyGUI (void);
void _BuildButtons (void);

void RenderGUI (void);
void UpdateGUI (float dt);
void ResizeGUI (int w, int h);
void GuiOnMouseUp (int x, int y);
void GuiOnMouseDown (int x, int y);
void GuiOnMouseMove (int x, int y);

void OnButtonClick (Cube *cube, Camera *cam, int sliceId, bool sliceForward);
#endif
