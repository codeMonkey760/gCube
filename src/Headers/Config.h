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

#ifndef _C_RUBE_CONFIG_H_
#define _C_RUBE_CONFIG_H_

// default sensitivity values for mouse control
extern float CONFIG_defaultPitchSens;
extern float CONFIG_defaultYawSens;
extern float CONFIG_defaultZoomSens;

// default zoom limits
extern float CONFIG_minZoom;
extern float CONFIG_maxZoom;

// starting orbital radius
extern float CONFIG_defaultOrbitalRadius;

// speed in radians per second for how fast the slices move
extern float CONFIG_sliceRotationSpeed;

// the starting width of the window
extern int CONFIG_width;

// the starting height of the window
extern int CONFIG_height;

// number of random rotation to make when shuffling
extern int CONFIG_shuffleSize;

// scaling factor for the gui
extern float CONFIG_guiScale;

// Load configuration file
void LoadConfigFile (void);
void CreateConfigFile (void);
void _ProcessContents (char *contents,int fileSize);

#endif

