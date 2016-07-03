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

#ifndef _C_RUBE_GUI_H_
#define _C_RUBE_GUI_H_

#define GUI_NUM_BUTTONS 12

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

void HideShuffleButton (void);
void ShowShuffleButton (void);

void OnButtonClick (Cube *cube, Camera *cam, int sliceId, bool sliceForward);
#endif

