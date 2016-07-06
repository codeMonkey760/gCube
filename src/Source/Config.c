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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Config.h"

float CONFIG_defaultPitchSens =       0.25f;
float CONFIG_defaultYawSens =         0.25f;
float CONFIG_defaultZoomSens =        1.0f;
float CONFIG_minZoom =                7.5f;
float CONFIG_maxZoom =               20.0f;
float CONFIG_defaultOrbitalRadius =  10.0f;
float CONFIG_sliceRotationSpeed =   720.0f;
int   CONFIG_width =               1024;
int   CONFIG_height =               728;
int   CONFIG_shuffleSize =          120;
float CONFIG_guiScale =               1.0f;

void LoadConfigFile (void) {
    FILE *fp = NULL;
    unsigned int fs = 0;
    char *contents = NULL;
    
    fp = OpenFile("gCube.cfg","r");
    if (fp == NULL) return;
    
    fseek(fp,0L, SEEK_END);
    fs = ftell(fp);
    fseek(fp,0L, SEEK_SET);
    fs++;
    contents = calloc(1,fs);
    fread(contents, 1, fs, fp);
    fclose(fp);
    fp = NULL;
    
    _ProcessContents(contents,fs);
    
    free(contents);
    contents = NULL;
}

void _ProcessContents(char *contents, int size) {
    char *iter = contents;
    char valueAsString[16] = {0};
    char tagAsString[64] = {0};
    char *curChar = tagAsString;
    float valueAsFloat = 0.0f;
    int valueAsInt = 0;
    int charCount;
    
    while (iter <= (contents + size) && (*iter) != 0) {
    
        memset(tagAsString,0,sizeof(char) * 64);
        curChar = tagAsString;
        charCount = 0;
        while ( (*iter) != ' ' && (*iter) != 0 && charCount++ < 63) {
            (*curChar++) = (*iter++);
        }
        if ( (*iter) == 0) continue;
    
        while ( ((*iter) == ' ' || (*iter) == '=') && (*iter) != 0) {
            iter++;
        }
        if ( (*iter) == 0) continue;
    
        memset(valueAsString,0,sizeof(char) * 16);
        curChar = valueAsString;
        charCount = 0;
        while ( (*iter) != '\n' && (*iter) != ' ' && (*iter) != 0 && charCount++ < 15) {
            (*curChar++) = (*iter++);
        }
        
        if ((*iter) != 0) iter++;
    
        if (_CompareTags(tagAsString, "pitchSens")) {
            valueAsFloat = atof(valueAsString);
            CONFIG_defaultPitchSens = valueAsFloat;
        } else if (_CompareTags(tagAsString, "yawSens")) {
            valueAsFloat = atof(valueAsString);
            CONFIG_defaultYawSens = valueAsFloat;
        } else if (_CompareTags(tagAsString, "zoomSens")) {
            valueAsFloat = atof(valueAsString);
            CONFIG_defaultZoomSens = valueAsFloat;
        } else if (_CompareTags(tagAsString, "minZoom")) {
            valueAsFloat = atof(valueAsString);
            CONFIG_minZoom = valueAsFloat;
        } else if (_CompareTags(tagAsString, "maxZoom")) {
            valueAsFloat = atof(valueAsString);
            CONFIG_maxZoom = valueAsFloat;
        } else if (_CompareTags(tagAsString, "orbitalRadius")) {
            valueAsFloat = atof(valueAsString);
            CONFIG_defaultOrbitalRadius = valueAsFloat;
        } else if (_CompareTags(tagAsString, "sliceRotationSpeed")) {
            valueAsFloat = atof(valueAsString);
            CONFIG_sliceRotationSpeed = valueAsFloat;
        } else if (_CompareTags(tagAsString, "windowWidth")) {
            valueAsInt = atoi(valueAsString);
            CONFIG_width = valueAsInt;
        } else if (_CompareTags(tagAsString, "windowHeight")) {
            valueAsInt = atoi(valueAsString);
            CONFIG_height = valueAsInt;
        } else if (_CompareTags(tagAsString, "shuffleSize")) {
            valueAsInt = atoi(valueAsString);
            CONFIG_shuffleSize = valueAsInt;
        } else if (_CompareTags(tagAsString, "guiScale")) {
            valueAsFloat = atof(valueAsString);
            CONFIG_guiScale = valueAsFloat;
        }
    }
}

