#include <stdio.h>
#include <stdlib.h>

#include "Config.h"

float CONFIG_defaultPitchSens =       0.25f;
float CONFIG_defaultYawSens =         0.25f;
float CONFIG_defaultZoomSens =        1.0f;
int   CONFIG_minZoom =                7.5f;
int   CONFIG_maxZoom =               20.0f;
float CONFIG_defaultOrbitalRadius =  10.0f;
float CONFIG_sliceRotationSpeed =   720.0f;
int   CONFIG_width =               1024;
int   CONFIG_height =               728;
int   CONFIG_shuffleSize =          120;
float CONFIG_guiScale =               1.0f;

void LoadConfigFile (void) {
    char path[4096] = {0};
    char *home = NULL;
    FILE* fp = NULL;
    unsigned int fs = 0;
    char *contents = NULL;
    
    home = getenv("HOME");
    if (home == NULL) return;
    
    strcpy(path, home);
    strcat(path,"/.gCube/gCube.cfg");
    
    fp = fopen(path, "r");
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

void _WriteDefaultConfigFile (void) {
    ;
}

void _ProcessContents(char *contents, int size) {
    char *iter = contents;
    
    while (iter < (contents + size)) {
        if (_CompareTags("pitchSens")) {
            ;
        } else if (_CompareTags("yawSens")) {
            ;
        } else if (_CompareTags("zoomSens")) {
            ;
        } else if (_CompareTags("minZoom")) {
            ;
        } else if (_CompareTags("maxZoom")) {
            ;
        }
        // FINISH FOR THE REST OF THE VALUES
    }
}