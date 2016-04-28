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

#ifndef _C_RUBE_SHADER_H_
#define _C_RUBE_SHADER_H_

extern int shader;
extern int vs;
extern int fs;

extern int inPosL;
extern int inNormL;
extern int inTexC;

extern int gWMtx;
extern int gWITMtx;
extern int gWVPMtx;
extern int gTexMtx;
extern int gDiffuseColor;
extern int gCamPos;
extern int gTexture;

#define INVALID_SHADER_ID -1

int InitShader (void);
void DestroyShader (void);

int _CompileShader (GLenum type, char *src);

#endif
