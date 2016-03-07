#ifndef _C_RUBE_GUI_H_
#define _C_RUBE_GUI_H_

#define GUI_NUM_BUTTONS 11

typedef struct {
    GUIButton buttons[GUI_NUM_BUTTONS];
    GUIButton *highlightedButton;
    Cube *cube;
} GUI;

extern GUI gui;

void InitializeGUI (void);
void _BuildButtons (void);

void Render (void);
void Update (float dt);
void Resize (int w, int h);
void GuiOnMouseUp (int x, int y);
void GuiOnMouseDown (int x, int y);
void GuiOnMouseMove (int x, int y);

#endif