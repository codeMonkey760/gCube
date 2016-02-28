#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "Shader.h"
#include "CubeletModel.h"
#include "Camera.h"
#include "Cubelet.h"
#include "Cube.h"

// glfw call backs
static void error_callback(int error, const char *desc);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void mouse_button_callback (GLFWwindow *window, int button, int action, int mods);
static void mouse_scroll_callback (GLFWwindow *window, double x, double y);
static void mouse_position_callback (GLFWwindow *window, double x, double y);

void init (void);
void initCubelets (void);
void updateCubelets (float dt);
void initWindow (void);
void render (void);
void update (void);
void timer (float dt);
void finalize (void);
int main (int argc, char **argv);

GLFWwindow *window = NULL;
Camera camera;
int lastMousePos[2] = {-1};
Cube cube;
char *APPNAME;

/*
 error call back ... called by glfw when an error occurs
 */
static void error_callback(int error, const char *desc) {
    fprintf(stderr, "ERROR! error: %d, %s",error,desc);
}

/*
 keyboard call back ... called by glfw when something happens on the keyboard
 */
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        fprintf(stdout, "Key press: %d\n", key);
        if (key == GLFW_KEY_Q) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }
}

/*
 mouse button call back ... called by glfw when something happens with the mouse buttons
 */
static void mouse_button_callback (GLFWwindow *window, int button, int action, int mods) {  
    if (button == 1) {
        camera.tracking = action == GLFW_PRESS;
        if (camera.tracking == false) {
            lastMousePos[0] = -1;
            lastMousePos[0] = -1;
        }
    }
}

/*
 * mouse scroll call back ... called by glfw when something happens with the mouse scroll wheel
 */
static void mouse_scroll_callback (GLFWwindow *window, double x, double y) {
    static double lastYOffset = 0.0;
    float deltaZoom = y - ((float) lastYOffset);
    Zoom(&camera,deltaZoom);
}

/*
 mouse position callback ... called by glfw when the mouse moves
 */
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

/*
 Performs general program specific initialization
 */

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
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    fprintf(stdout, "GL_VERSION: %s\n",glGetString(GL_VERSION));
    fprintf(stdout, "GL_VENDOR: %s\n",glGetString(GL_VENDOR));
    fprintf(stdout, "GL_RENDER: %s\n",glGetString(GL_RENDERER));

    InitCamera(&camera);
    //RebuildOrthographicMatrix(&camera,-fSize, fSize, -fSize, fSize, -fSize, fSize);
    RebuildPerspectiveMatrix(&camera, 100.0f, (640.0f/480.0f), 0.01f, 100.0f);

    fprintf(stdout,"Program start\nPress Q to quit\n");
}

/*
 Performs initialization specific to the glfw window
 */
void initWindow (void) {
    const GLFWvidmode *vidmode = NULL;
    int screenW, screenH;
    int w = 640, h = 480;

    vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    screenW = vidmode->width;
    screenH = vidmode->height;

    window = glfwCreateWindow(w,h,APPNAME, NULL, NULL);
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

/*
 Renders frames
 */
void render (void) {
    glViewport(0,0,640,480);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderCube(&cube,&camera);
}

/*
 Perform general engine wide update with respect to time
 */
void update (void) {
    static double prevTimeStamp = 0.0f;
    double nextTimeStamp = 0.0f;
    float deltaTime = 0.0f;

    nextTimeStamp = glfwGetTime();
    deltaTime = (float) (nextTimeStamp - prevTimeStamp);
    prevTimeStamp = nextTimeStamp;

    timer(deltaTime);
    UpdateCamera(&camera, deltaTime);
    UpdateCube(&cube,deltaTime);
}

/*
 updates the engine's built in timer
 */
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

/*
 Performs general engine wide cleanup during program destruction
 */
void finalize (void) {
    fprintf(stdout,"Program end\n");
    DestroyCubeletVBOs();
    DestroyShader();
    DestroyTextures();
    glfwTerminate();
}

/*
 Main
 */
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
    InitCubeletVBOs();
    InitTextures();
    InitCube(&cube);
    
    /* TEST CODE */ /*
    printf("arrow.png was mapped to: %d\n",GetTextureByName("arrow.png"));
    printf("blender.png was mapped to: %d\n",GetTextureByName("blender.png"));
    printf("rot.png was mapped to: %d\n",GetTextureByName("rot.png"));
    printf("invert.png was mapped to: %d\n",GetTextureByName("invert.png"));
    printf("Frame.png was mapped to: %d\n",GetTextureByName("Frame.png"));
    */ /* END TEST CODE */

    while (glfwWindowShouldClose(window) == false) {
        update();
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    finalize();
    return EXIT_SUCCESS;
}
