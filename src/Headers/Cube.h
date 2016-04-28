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

#ifndef _C_RUBE_CUBE_H_
#define _C_RUBE_CUBE_H_

#define NUM_CUBELETS 26
#define DEFAULT_SLICE_SPEED CONFIG_sliceRotationSpeed
#define DEFAULT_SLICE_ROTATION_LENGTH 90.0f

typedef struct {
    Cubelet **cubelets;
    int numCubelets;
    
    float pivotAxis[3];
    float degreesRemaining;
    float degreesPerSecond;
    bool forward;
    
} SliceAnimation;

typedef struct {
    float posW[3];
    float rotation[4];
    float scale[3];
    
    Cubelet cubelets[NUM_CUBELETS];
    SliceAnimation *curAnimation;
    
    int shuffleSize;
    int *shuffle;
    int curShuffle;
}Cube;

typedef enum {
    SLICE_POS_X,
    SLICE_NEG_X,
    SLICE_POS_Y,
    SLICE_NEG_Y,
    SLICE_POS_Z,
    SLICE_NEG_Z,
    SLICE_WHOLE_CUBE
}Slice;

void InitCube (Cube *cube);
void UpdateCube (Cube *cube, float dt);
void RenderCube (Cube *cube, Camera *cam);
void SaveCube (Cube *cube);
void LoadCube (Cube *cube);
void DestroyCube (Cube *cube);

void InitNewSliceAnimation (
    SliceAnimation **sa, 
    float newPivotAxis[3],
    float initialDegrees,
    float newDegreesPerSecond,
    bool sliceForward,
    Cubelet **cubeletsToAnimate,
    int numCubelets
);

bool UpdateSliceAnimation (SliceAnimation *sa, float dt);
void DestroySliceAnimation (SliceAnimation *sa);
bool CheckCubeForWin (Cube *cube);

void StartShuffleSequence (Cube *cube, int sizeOfShuffle);
bool IsShuffling (Cube *cube);
void DestroyShuffleSequence (Cube *cube);

void _StartSliceAnimation(Cube *cube, Camera *cam, Slice slice, bool sliceForward);
void SaveAnimation(SliceAnimation *sa, FILE *file);
void LoadAnimation(Cube *cube, SliceAnimation *sa, FILE *file);

void _PositionCubelets (Cube *cube);

#endif

