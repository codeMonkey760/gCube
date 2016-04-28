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