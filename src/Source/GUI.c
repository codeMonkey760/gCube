#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Util.h"
#include "Camera.h"
#include "Cubelet.h"
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
    
	//top slice left
    SetPRD(&cb,0.1f, 0.3f, 0.1f, 0.1f, 270.0f);
    cb.sliceId = 3;
    cb.sliceForward = true;
    gui.buttons[1] = cb;

    //bottom slice down
    SetPRD(&cb, 0.1f, 0.7f, 0.1f, 0.1f, 270.0f);
    cb.sliceId = 2;
    cb.sliceForward = false;
    gui.buttons[2] = cb;

    //left slice down
    SetPRD(&cb, 0.3f, 0.9f, 0.1f, 0.1f, 180.0f);
    cb.sliceId = 0;
    cb.sliceForward = true;
    gui.buttons[3] = cb;

    //right slice down
    SetPRD(&cb, 0.7f, 0.9f, 0.1f, 0.1f, 180.0f);
    cb.sliceId = 1;
    cb.sliceForward = false;
    gui.buttons[4] = cb;

    //bottom slice right
    SetPRD(&cb, 0.9f, 0.7f, 0.1f, 0.1f, 90.0f);
    cb.sliceId = 2;
    cb.sliceForward = true;
    gui.buttons[5] = cb;

    // top slice right
    SetPRD(&cb, 0.9f, 0.3f, 0.1f, 0.1f, 90.0f);
    cb.sliceId = 3;
    cb.sliceForward = false;
    gui.buttons[6] = cb;

    //right slice up
    SetPRD(&cb, 0.7f, 0.1f, 0.1f, 0.1f, 0.0f);
    cb.sliceId = 1;
    cb.sliceForward = true;
    gui.buttons[7] = cb;

    // forward slice left
    SetPRD(&cb,0.4f, 0.9f, 0.1f, 0.1f, 0.0f);
    cb.texId = GetTextureByName("rot.png");
    cb.sliceId = 4;
    cb.sliceForward = true;
    gui.buttons[8] = cb;

    // forward slice right
    SetPRD(&cb, 0.6f, 0.9f, 0.1f, 0.1f, 0.0f);
    cb.sliceId = 4;
    cb.sliceForward = false;
    gui.buttons[9] = cb;

    // invert the whole cube
    SetPRD(&cb, 0.5f, 0.1f, 0.1f, 0.1f, 0.0f);
    cb.sliceId = 6;
    cb.sliceForward = true;
    gui.buttons[10] = cb;
}

void Render (void) {
    DrawButtons(gui.buttons,GUI_NUM_BUTTONS);
}

void Update (float dt) {
    int i;
    for (i = 0; i < GUI_NUM_BUTTONS; ++i) {
        ButtonUpdate (&gui.buttons[i],dt);
    }
}

void Resize (int w, int h) {
    return;
}

void GuiOnMouseUp (int x, int y) {
    float MouseXPercent = ((float)WindowWidth) / ((float)x);
    float MouseYPercent = ((float)WindowHeight) / ((float)y);
    int i;
    
    for (i = 0; i < GUI_NUM_BUTTONS; ++i) {
        if (InBounds(&gui.buttons[i],MouseXPercent,MouseYPercent)) {
            ButtonOnMouseUp(&gui.buttons[i]);
            return;
        }
    }
}

void GuiOnMouseDown (int x, int y) {
    float MouseXPercent = ((float) WindowWidth) / ((float)x);
    float MouseYPercent = ((float) WindowHeight) / ((float)y);
    int i;
    
    for (i = 0; i < GUI_NUM_BUTTONS; ++i) {
        if (InBounds(&gui.buttons[i], MouseXPercent, MouseYPercent)) {
            ButtonOnMouseDown(&gui.buttons[i]);
            return;
        }
    }
}

void GuiOnMouseMove (int x, int y) {
    float MouseXPercent = ((float)WindowWidth) / ((float)x);
    float MouseYPercent = ((float)WindowHeight) / ((float)y);
    int i;
    
    if (gui.highlightedButton != NULL) {
        if (InBounds(gui.highlightedButton,MouseXPercent,MouseYPercent) != true) {
            ButtonOnMouseExit(gui.highlightedButton);
            gui.highlightedButton = NULL;
        } else {
            return;
        }
    }
    
    for (i = 0; i < GUI_NUM_BUTTONS; ++i) {
        if (InBounds(&gui.buttons[i], MouseXPercent, MouseYPercent)) {
            gui.highlightedButton = &gui.buttons[i];
            ButtonOnMouseEnter(&gui.buttons[i]);
            return;
        }
    }
}
