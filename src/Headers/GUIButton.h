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

#ifndef _C_RUBE_GUIBUTTON_H_
#define _C_RUBE_GUIBUTTON_H_

typedef struct {
    bool visible;
    bool enabled;
    bool depressed;
    bool highlighted;
    bool mouseDownInBounds;
    int sliceId;
    bool sliceForward;
    
    float posPercent[3];  //Position of the button's center in screen percentage
    float dim[3];         // Dimensions of the button in screen percent
    float rot;
    
    float tMtx[16];
    float texMtx[9];
    int texId;
    
    float ambientColor[3];
    float mixValue;
} GUIButton;

void InitButton (GUIButton *button);
void SetPRD (
    GUIButton *button, 
    float percentX, 
    float percentY, 
    float percentW, 
    float percentH, 
    float rotZ
);
void _RecalcTMtx (GUIButton *button);
bool InBounds (GUIButton *button, float percentX, float percentY);
void ButtonUpdate (GUIButton *button, float dt);
void ButtonOnMouseUp (GUIButton *button);
void ButtonOnMouseDown (GUIButton *button);
void ButtononMouseEnter (GUIButton *button);
void ButtononMouseExit (GUIButton *button);

#endif
