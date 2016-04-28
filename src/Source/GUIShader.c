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

#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "Shader.h"
#include "GUIShader.h"
#include "Util.h"

GUIShader guiShader;

bool InitGUIShader (void) {
    int linked = GL_FALSE, logSize = 0;
    char *log = NULL;
    
    memset(&guiShader, 0, sizeof(GUIShader));
    
    guiShader.shader = INVALID_SHADER_ID;
    guiShader.vs = INVALID_SHADER_ID;
    guiShader.fs = INVALID_SHADER_ID;
    guiShader.inPosL = INVALID_SHADER_ID;
    guiShader.inTexC = INVALID_SHADER_ID;
    guiShader.gTMtx = INVALID_SHADER_ID;
    guiShader.gTexMtx = INVALID_SHADER_ID;
    guiShader.gTexture = INVALID_SHADER_ID;
    guiShader.gAmbient = INVALID_SHADER_ID;
    guiShader.gMixValue = INVALID_SHADER_ID;
    
    guiShader.vs = _CompileShader(GL_VERTEX_SHADER, GUIVS);
    if (guiShader.vs == 0) {
        return false;
    }
    
    guiShader.fs = _CompileShader(GL_FRAGMENT_SHADER, GUIFS);
    if (guiShader.fs == 0) {
        return false;
    }
    
    guiShader.shader = glCreateProgram();
    glAttachShader(guiShader.shader, guiShader.vs);
    glAttachShader(guiShader.shader, guiShader.fs);
    glLinkProgram(guiShader.shader);
    glGetProgramiv(guiShader.shader, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        glGetProgramiv(guiShader.shader, GL_INFO_LOG_LENGTH, &logSize);
        logSize *= sizeof(char);
        log = calloc(1,logSize + (1 * sizeof(char)));
        glGetProgramInfoLog(shader,logSize,&logSize,log);
        fprintf(stderr, "GUI Shader linking error:\n%s\n",log);
        free(log);
        log = NULL;
        return false;
    }
    
    guiShader.inPosL = glGetAttribLocation(guiShader.shader, "posL");
    guiShader.inTexC = glGetAttribLocation(guiShader.shader, "intexC");
    
    guiShader.gTMtx = glGetUniformLocation(guiShader.shader, "gTMtx");
    guiShader.gTexMtx = glGetUniformLocation(guiShader.shader, "gTexMtx");
    guiShader.gTexture = glGetUniformLocation(guiShader.shader, "gTexture");
    guiShader.gAmbient = glGetUniformLocation(guiShader.shader, "gAmbient");
    guiShader.gMixValue = glGetUniformLocation(guiShader.shader, "gMixValue");
    
    glDetachShader(guiShader.shader, guiShader.vs);
    glDetachShader(guiShader.shader, guiShader.fs);
    
    if (
        guiShader.inPosL    == INVALID_SHADER_ID ||
        guiShader.inTexC    == INVALID_SHADER_ID ||
        guiShader.gTMtx     == INVALID_SHADER_ID ||
        guiShader.gTexMtx   == INVALID_SHADER_ID ||
        guiShader.gTexture  == INVALID_SHADER_ID ||
        guiShader.gAmbient  == INVALID_SHADER_ID ||
        guiShader.gMixValue == INVALID_SHADER_ID
    ) {
        fprintf(stderr, "Bad uniform or attribute location!\n");
        return false;
    }
    
    return true;
}

void DestroyGUIShader (void) {
    glDeleteShader(guiShader.vs);
    glDeleteShader(guiShader.fs);
    glDeleteProgram(guiShader.shader);
    
    guiShader.vs = guiShader.fs = guiShader.shader = 0;
}