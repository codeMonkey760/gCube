#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "Util.h"
#include "GUIButton.h"

void InitButton (GUIButton *button) {
    if (button == NULL) return;
    
    memset(button,0,sizeof(GUIButton));
    
    button->visible = true;
    button->enabled = true;
    button->sliceForward = true;
    
    button->dim[0] = 1.0f;
    button->dim[1] = 1.0f;
    button->dim[2] = 1.0f;
    
    Mat4Identity(button->tMtx);
    button->texMtx[0] = 1.0f;
    button->texMtx[4] = 1.0f;
    button->texMtx[8] = 1.0f;
    button->texId = -1;
    
    button->mixValue = 0.7f;
}

void SetPRD (
    GUIButton *button,
    float percentX, 
    float percentY, 
    float percentW, 
    float percentH, 
    float rotZ
) {
    if (button == NULL) return;
    
    button->posPercent[0] = percentX;
    button->posPercent[1] = percentY;
    button->dim[0] = percentW;
    button->dim[1] = percentH;
    button->rot = rotZ;
    
    _RecalcTMtx(button);
}

void _RecalcTMtx (GUIButton *button) {
    if (button == NULL) return;
    
    float sMtx[16] = {0.0f};
    float rMtx[16] = {0.0f};
    float tMtx[16] = {0.0f};
    float scale[3] = {1.0f,1.0f,1.0f};
    
    Mat4Scaling(sMtx,button->dim);
    Mat4RotationZ(rMtx,button->rot);
    Mat4Translation(tMtx, button->posPercent);
    
    //NOTE: This is wrong
    Mat4Mult(sMtx, rMtx, button->tMtx);
    Mat4Mult(tMtx, button->tMtx, button->tMtx);
}

void ButtonUpdate (GUIButton *button, float dt) {
    if (button == NULL) return;
    
    return;
}

bool InBounds (GUIButton *button, float x, float y) {
    if (button == NULL) return false;
    
    float hw = button->dim[0] / 2.0f;
    float hh = button->dim[1] / 2.0f;
    
    float x1 = button->posPercent[0] - hw;
    float y1 = button->posPercent[1] - hh;
    float x2 = button->posPercent[0] + hw;
    float y2 = button->posPercent[1] + hh;
    
    if (
        x1 < x && x < x2 &&
        y1 < y && y < y2 &&
        button->visible == true &&
        button->enabled == true
    ) {
        return true;
    }
    
    return false;
}

void ButtonOnMouseUp (GUIButton *button) {
    if (button == NULL) return;
    
    if (button->visible == false || button->enabled == false) return;
    
    button->depressed = false;
    button->mixValue = 0.9f;
    
    if (button->mouseDownInBounds == true) {
        if (button->onClick != NULL) {
            button->onClick(button->sliceId, button->sliceForward);
        }
    }
    
    button->mouseDownInBounds = false;
}

void ButtonOnMouseDown (GUIButton *button) {
    if (button == NULL) return;
    
    if (button->visible == false || button->enabled == false) return;
    
    button->mixValue = 0.5f;
    button->mouseDownInBounds = true;
    button->depressed = true;
}

void ButtonOnMouseEnter (GUIButton *button) {
    if (button == NULL) return;
    if (button->visible == false || button->enabled == false) return;
    
    button->highlighted = true;
    button->mixValue = 0.9f;
    if (button->onHighlight != NULL) {
        button->onHighlight();
    }
}

void ButtonOnMouseExit (GUIButton *button) {
    if (button == NULL) return;
    if (button->visible = false || button->enabled == false) return;
    
    button->depressed = false;
    button->highlighted = false;
    button->mouseDownInBounds = false;
    button->mixValue = 0.7f;
}