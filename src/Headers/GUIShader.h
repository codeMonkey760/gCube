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

#ifndef _C_RUBE_GUISHADER_H_
#define _C_RUBE_GUISHADER_H_

typedef struct {
    int shader;
    int vs;
    int fs;
    
    int inPosL;
    int inTexC;
    
    int gTMtx;
    int gTexMtx;
    
    int gTexture;
    int gAmbient;
    int gMixValue;
} GUIShader;

extern GUIShader guiShader;

bool InitGUIShader (void);
void DestroyGUIShader (void);

#endif