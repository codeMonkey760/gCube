#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "Shader.h"
#include "Util.h"

int shader = INVALID_SHADER_ID;
int vs = INVALID_SHADER_ID;
int fs = INVALID_SHADER_ID;

int inPosL = INVALID_SHADER_ID;
int inNormL = INVALID_SHADER_ID;
int inTexC = INVALID_SHADER_ID;

int gWMtx = INVALID_SHADER_ID;
int gWITMtx = INVALID_SHADER_ID;
int gWVPMtx = INVALID_SHADER_ID;
int gTexMtx = INVALID_SHADER_ID;
int gDiffuseColor = INVALID_SHADER_ID;
int gCamPos = INVALID_SHADER_ID;
int gTexture = INVALID_SHADER_ID;

int InitShader (void) {
    int	linked = GL_FALSE,
        logSize = 0;
    char *log = NULL;

    vs = _CompileShader(GL_VERTEX_SHADER,CubeletVS);
    if (vs == 0) {
        return GL_FALSE;
    }

    fs = _CompileShader(GL_FRAGMENT_SHADER,CubeletFS);
    if (fs == 0) {
        glDeleteShader(vs);
        vs = 0;
        return GL_FALSE;
    }

    shader = glCreateProgram();
    glAttachShader(shader,vs);
    glAttachShader(shader,fs);
    glLinkProgram(shader);
    glGetProgramiv(shader,GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        glGetProgramiv(shader,GL_INFO_LOG_LENGTH, &logSize);
        logSize *= sizeof(char);
        log = calloc(1,logSize+1);
        glGetProgramInfoLog(shader,logSize,&logSize,log);
        glDeleteProgram(shader);
        glDeleteShader(vs);
        vs = 0;
        glDeleteShader(fs);
        fs = 0;
        fprintf(stderr, "Program linking error:\n%s\n",log);
        free(log);
        log = NULL;
        return GL_FALSE;
    }

    inPosL = glGetAttribLocation(shader,"posL");
    inNormL = glGetAttribLocation(shader, "normL");
    inTexC = glGetAttribLocation(shader, "intexC");
    
    gWMtx = glGetUniformLocation(shader, "gWMtx");
    gWITMtx = glGetUniformLocation(shader, "gWITMtx");
    gWVPMtx = glGetUniformLocation(shader,"gWVPMtx");
    gTexMtx = glGetUniformLocation(shader, "gTexMtx");
    gDiffuseColor = glGetUniformLocation(shader, "gDiffuseColor");
    gCamPos = glGetUniformLocation(shader, "gCamPos");
    gTexture = glGetUniformLocation(shader, "gTexture");

    glDetachShader(shader,vs);
    glDetachShader(shader,fs);

    if (
        inPosL == INVALID_SHADER_ID ||
        inNormL == INVALID_SHADER_ID ||
        inTexC == INVALID_SHADER_ID ||
        gWMtx == INVALID_SHADER_ID ||
        gWITMtx == INVALID_SHADER_ID ||
        gWVPMtx == INVALID_SHADER_ID ||
        gTexMtx == INVALID_SHADER_ID ||
        gDiffuseColor == INVALID_SHADER_ID || 
        gCamPos == INVALID_SHADER_ID ||
        gTexture == INVALID_SHADER_ID
    ) {
            fprintf(stderr, "Bad uniform or attribute location!\n");
            return GL_FALSE;
    }

    return GL_TRUE;
}

void DestroyShader (void) {
    glDeleteProgram(shader);
    shader = 0;
    glDeleteShader(vs);
    vs = 0;
    glDeleteShader(fs);
    fs = 0;
}

int _CompileShader (GLenum type, char *src) {
    int sc = NULL,
        compiled = GL_FALSE,
        logSize = 0
    ;
    char *log = NULL;

    sc = glCreateShader(type);
    glShaderSource(sc,1,(const GLchar **) &src,NULL);
    glCompileShader(sc);

    glGetShaderiv(sc, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        glGetShaderiv(sc,GL_INFO_LOG_LENGTH, &logSize);
        log = calloc(1,(sizeof(GLchar) * logSize) + 1);
        glGetShaderInfoLog(sc, logSize, &logSize, log);
        fprintf(stderr,"Shader compile message:\n%s\n",log);
        free(log);
        log = NULL;
        glDeleteShader(sc);
        sc = 0;
    }

    return sc;
}