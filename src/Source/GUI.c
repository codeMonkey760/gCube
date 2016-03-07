#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Util.h"
#include "Cube.h"
#include "GUIButton.h"
#include "GUIRenderer.h"
#include "GUI.h"

GUI gui;

void InitializeGUI (void) {
    memset(&gui,0,sizeof(GUI));
    
    _BuildButtons();
}

void _BuildButtons (void) {
    GUIButton cb;
    InitButton(&cb);
    
    // all buttons
    cb.ambientColor[0] = 1.0f;
    cb.ambientColor[1] = 1.0f;
    cb.ambientColor[2] = 1.0f;
    cb.texId = GetTextureByName("arrow.png");
    cb.onClick = NULL;
    
    //left slice up
    SetPRD(&cb,0.3f,0.1f,0.1f,0.1f,0.0f);
    cb.sliceId = 0;
    cb.sliceForward = false;
    gui.buttons[0] = cb;
    
    SetPRD(&cb,0.1f, 0.3f, 0.1f, 0.1f, 270.0f);
    
    
    
}

void Render (void) {
    ;
}

void Update (float dt) {
    ;
}

void Resize (int w, int h) {
    ;
}

void GuiOnMouseUp (int x, int y) {
    ;
}

void GuiOnMouseDown (int x, int y) {
    ;
}

void GuiOnMouseMove (int x, int y) {
    ;
}
