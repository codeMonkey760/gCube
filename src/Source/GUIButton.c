/*
Copyright 2016 Sean Paget
Email: codeMonkey760@hotmail.com
    
This file is part of gCube.

gCube is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

gCube is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with gCube.  If not, see <http://www.gnu.org/licenses/>.
Please read COPYING.txt for details
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "Util.h"
#include "Camera.h"
#include "Cubelet.h"
#include "Cube.h"
#include "GUIButton.h"
#include "Config.h"

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
    
    /*
    button->posPercent[0] = percentX;
    button->posPercent[1] = percentY;
    button->dim[0] = percentW;
    button->dim[1] = percentH;
    */
    
    button->posPercent[0] = percentX;
    button->posPercent[1] = percentY;
    button->dim[0] = percentW * CONFIG_guiScale;
    button->dim[1] = percentH * CONFIG_guiScale;

    button->rot = rotZ;
    
    _RecalcTMtx(button);
}

void _RecalcTMtx (GUIButton *button) {
    if (button == NULL) return;
    
    float sMtx[16] = {0.0f};
    float rMtx[16] = {0.0f};
    float tMtx[16] = {0.0f};
    float posH[3] = {0.0f};
    
    posH[0] = (button->posPercent[0] * 2.0f) - 1.0f;
    posH[1] = (button->posPercent[1] * -2.0f) + 1.0f;
    
    Mat4ScalingFA(sMtx,button->dim);
    Mat4RotationZ(rMtx,button->rot);
    Mat4TranslationFA(tMtx, posH);
    
    //NOTE: This is wrong
    Mat4Mult(button->tMtx, sMtx, rMtx);
    Mat4Mult(button->tMtx, button->tMtx, tMtx);
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
}

void ButtonOnMouseExit (GUIButton *button) {
    if (button == NULL) return;
    if (button->visible == false || button->enabled == false) return;
    
    button->depressed = false;
    button->highlighted = false;
    button->mouseDownInBounds = false;
    button->mixValue = 0.7f;
}

