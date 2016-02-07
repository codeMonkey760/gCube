#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Shader.h"
#include "QuadModel.h"
#include "Camera.h"
#include "Quad.h"

void init (void);
void render (void);
void update (void);
void timer (float dt);
void finalize (void);

GLFWwindow *window = NULL;
Camera camera;
int lastMousePos[2] = {-1};
#define NUM_QUADS 5
Quad quads[NUM_QUADS];

static void error_callback(int error, const char *desc) {
    fprintf(stderr, "ERROR! error: %d, %s",error,desc);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        fprintf(stdout, "Key press: %d\n", key);
        if (key == GLFW_KEY_Q) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }
}

static void mouse_button_callback (GLFWwindow *window, int button, int action, int mods) {  
    if (button == 1) {
        camera.tracking = action == GLFW_PRESS;
        if (camera.tracking == false) {
            lastMousePos[0] = -1;
            lastMousePos[0] = -1;
        }
    }
}

static void mouse_scroll_callback (GLFWwindow *window, double x, double y) {
    static double lastYOffset = 0.0;
    float deltaZoom = y - ((float) lastYOffset);
    Zoom(&camera,deltaZoom);
}

static void mouse_position_callback (GLFWwindow *window, double x, double y) {
    int deltaPos[2] = {0};
    if (camera.tracking == false) return;
    
    if (lastMousePos[0] == -1 || lastMousePos[1] == -1) {
        lastMousePos[0] = ((int) x);
        lastMousePos[1] = ((int) y);
        return;
    } else {
        deltaPos[0] = ((int) x) - lastMousePos[0];
        deltaPos[1] = ((int) y) - lastMousePos[1];
    }
    lastMousePos[0] = ((int) x);
    lastMousePos[1] = ((int) y);
    
    OnMouseMove(&camera,deltaPos);
}

void init (void) {
    int pos[2] = {45,0};
    int fSize = 2.0f;

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window,mouse_button_callback);
    glfwSetCursorPosCallback(window,mouse_position_callback);
    glfwSetScrollCallback(window,mouse_scroll_callback);
    glfwSwapInterval(1);
    glClearColor(0.2f,0.2f,0.8f,1.0f);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    fprintf(stdout, "GL_VERSION: %s\n",glGetString(GL_VERSION));
    fprintf(stdout, "GL_VENDOR: %s\n",glGetString(GL_VENDOR));
    fprintf(stdout, "GL_RENDER: %s\n",glGetString(GL_RENDERER));

    InitCamera(&camera);
    //RebuildOrthographicMatrix(&camera,-fSize, fSize, -fSize, fSize, -fSize, fSize);
    RebuildPerspectiveMatrix(&camera, 45.0f, (640/480), 0.01f, 100.0f);

    fprintf(stdout,"Program start\nPress Q to quit\n");
}

void initQuads (void) {
    InitQuadArray(quads,NUM_QUADS);
    
    quads[0].color[0] = 1.0f;
    
    quads[1].posW[0] = -5.0f;
    quads[1].color[1] = 1.0f;
    
    quads[2].posW[0] = 5.0f;
    quads[2].color[2] = 1.0f;
    
    quads[3].posW[1] = 5.0f;
    quads[3].color[0] = 1.0f;
    quads[3].color[2] = 1.0f;
    
    quads[4].posW[1] = -5.0f;
    quads[4].color[0] = 1.0f;
    quads[4].color[1] = 1.0f;
}

void initWindow (void) {
    const GLFWvidmode *vidmode = NULL;
    int screenW, screenH;
    int w = 640, h = 480;

    vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    screenW = vidmode->width;
    screenH = vidmode->height;

    window = glfwCreateWindow(w,h,"GLFW Test", NULL, NULL);
    if (window == NULL) {
        return;
    }

    glfwSetWindowPos(window, (screenW / 2) - (w / 2), (screenH / 2) - (h / 2));

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum glewErr = glewInit();
    if (glewErr != GLEW_OK) {
        fprintf(stderr,"GLEW did not initialize: %s\n",glewGetErrorString(glewErr));
    }
}

void render (void) {
    glViewport(0,0,640,480);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawQuadArray(quads,NUM_QUADS,&camera);
}

void update (void) {
    static double prevTimeStamp = 0.0f;
    double nextTimeStamp = 0.0f;
    float deltaTime = 0.0f;

    nextTimeStamp = glfwGetTime();
    deltaTime = (float) (nextTimeStamp - prevTimeStamp);
    prevTimeStamp = nextTimeStamp;

    timer(deltaTime);
    UpdateCamera(&camera, deltaTime);
}

void timer (float dt) {
    static float elapsed = 0.0f;
    static int frames = 0;

    elapsed += dt;
    frames++;
    if (elapsed >= 1.0f) {
        elapsed -= 1.0f;
        //fprintf(stdout,"Frames: %d\n",frames);
        frames = 0;
    }
}

void finalize (void) {
    fprintf(stdout,"Program end\n");
    DestroyCubeVBO();
    DestroyShader();
    glfwTerminate();
}

int main (int argc, char **argv) {
    glfwSetErrorCallback(error_callback);

    if (glfwInit() == false) {
        return EXIT_FAILURE;
    }

    initWindow();
    if (window == NULL) {
        finalize();
        return EXIT_FAILURE;
    }

    init();
    if (InitShader() == GL_FALSE) {
        finalize();
        return EXIT_FAILURE;
    }
    InitCubeVBO();
    initQuads();

    while (glfwWindowShouldClose(window) == false) {
        update();
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    finalize();
    return EXIT_SUCCESS;
}
