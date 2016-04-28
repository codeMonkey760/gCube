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

#ifndef _C_RUBE_CUBELET_H_
#define _C_RUBE_CUBELET_H_

typedef struct {
    int id;
    
    float posW[3];
    float rotation[4]; // rotation internally represented by a quaternion
    float scale[3];
    
    bool stickers[6];
    
} Cubelet;

void InitCubeletArray(Cubelet *array, int numCubelets);
void DrawCubeletArray(Cubelet *array, int numCubelets, Camera *cam);
void BuildMatricies(Cubelet *singleCubelet, float wMtx[16], float witMtx[16]);
bool CheckCubelet(Cubelet *cubelet);

void SaveCubelet (Cubelet *cubelet, FILE *file);
void LoadCubelet (Cubelet *cubelet, FILE *file);

#endif