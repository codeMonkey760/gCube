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

#include "GUIButton.h"
#include "GUI.h"

// glfw call backs
static void error_callback(int error, const char *desc);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void mouse_button_callback (GLFWwindow *window, int button, int action, int mods);
static void mouse_scroll_callback (GLFWwindow *window, double x, double y);
static void mouse_position_callback (GLFWwindow *window, double x, double y);
static void window_resize_callback(GLFWwindow *window, int width, int height);

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
Cube cube;
char *APPNAME;
int WindowWidth;
int WindowHeight;

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
    double x = 0.0, y = 0.0;
    int pos[2] = {0};
    glfwGetCursorPos(window, &x, &y);
    pos[0] = ((int) x);
    pos[1] = ((int) y);
    
    if (action == GLFW_PRESS) {
        OnMouseDown(&camera,pos,button);
        GuiOnMouseDown( (int) x, (int) y);
    } else if (action == GLFW_RELEASE) {
        OnMouseUp(&camera,pos,button);
        GuiOnMouseUp( (int) x, (int) y);
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
    int pos[2] = {0};
    pos[0] = ((int) x);
    pos[1] = ((int) y);
    
    OnMouseMove(&camera,pos);
    GuiOnMouseMove( (int) x, (int) y);
}

/*
 window resize callback ... called by glfw when the window resizes
 */
static void window_resize_callback(GLFWwindow *window, int width, int height) {
    WindowWidth = width;
    WindowHeight = height;
    RebuildPerspectiveMatrix(
        &camera, 
        100.0f,
        ((float) WindowWidth) / ((float) WindowHeight),
        0.01f,
        100.0f
    );
}

/*
 Performs general program specific initialization
 */

void init (void) {
    int pos[2] = {45,0};

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window,mouse_button_callback);
    glfwSetCursorPosCallback(window,mouse_position_callback);
    glfwSetScrollCallback(window,mouse_scroll_callback);
    glfwSetWindowSizeCallback(window,window_resize_callback);
    glfwSwapInterval(1);
    glClearColor(0.2f,0.2f,0.8f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);
    // lol cubelets need front faces culled and gui is all front faces
    // need to fix my facing issues before face culling can be enabled!
    //glEnable(GL_CULL_FACE);
    fprintf(stdout, "GL_VERSION: %s\n",glGetString(GL_VERSION));
    fprintf(stdout, "GL_VENDOR: %s\n",glGetString(GL_VENDOR));
    fprintf(stdout, "GL_RENDER: %s\n",glGetString(GL_RENDERER));

    InitCamera(&camera);
    RebuildPerspectiveMatrix(
        &camera, 
        100.0f, 
        ((float) WindowWidth)/ ((float) WindowHeight), 
        0.01f, 
        100.0f
    );

    fprintf(stdout,"Program start\nPress Q to quit\n");
}

/*
 Performs initialization specific to the glfw window
 */
void initWindow (void) {
    const GLFWvidmode *vidmode = NULL;
    int screenW, screenH;

    vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    screenW = vidmode->width;
    screenH = vidmode->height;

    window = glfwCreateWindow(WindowWidth,WindowHeight,APPNAME, NULL, NULL);
    if (window == NULL) {
        return;
    }

    glfwSetWindowPos(window, (screenW / 2) - (WindowWidth / 2), (screenH / 2) - (WindowHeight / 2));

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
    glViewport(0,0,WindowWidth,WindowHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderCube(&cube,&camera);
    RenderGUI();
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
    UpdateGUI(deltaTime);
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
    DestroyGUIShader();
    DestroyTextures();
    DestroyGUI();
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
    if (InitGUIShader() == false) {
        finalize();
        return EXIT_FAILURE;
    }
    InitCubeletVBOs();
    InitTextures();
    InitCube(&cube);
	InitializeGUI();
    
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
