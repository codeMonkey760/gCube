#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Shader.h"
#include "CubeModel.h"

void init (void);
void render (void);
void update (void);
void timer (float dt);
void finalize (void);

GLFWwindow *window = NULL;

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
	if (action == GLFW_PRESS) {
		fprintf(stdout, "Mouse press: %d\n", button);
	}
}

void init (void) {
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window,mouse_button_callback);
	glfwSwapInterval(1);
	glClearColor(0.2f,0.2f,0.8f,1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	fprintf(stdout, "GL_VERSION: %s\n",glGetString(GL_VERSION));
	fprintf(stdout, "GL_VENDOR: %s\n",glGetString(GL_VENDOR));
	fprintf(stdout, "GL_RENDER: %s\n",glGetString(GL_RENDERER));

	fprintf(stdout,"Program start\nPress Q to quit\n");
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
	float cubeColor[] = {0.75f, 0.25f, 0.1f};

	glViewport(0,0,640,480);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader);

	glUniform3fv(gDiffuseColor,1,cubeColor);

	glEnableVertexAttribArray(inPosL);
	glBindBuffer(GL_ARRAY_BUFFER,GetCubeVBO());

	glVertexAttribPointer(inPosL,3,GL_FLOAT,GL_FALSE,0,NULL);

	glDrawArrays(GL_TRIANGLES, 0, VBO_SIZE_IN_INDICES);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);

	glUseProgram(0);
}

void update (void) {
	static double prevTimeStamp = 0.0f;
	double nextTimeStamp = 0.0f;
	float deltaTime = 0.0f;

	nextTimeStamp = glfwGetTime();
	deltaTime = (float) (nextTimeStamp - prevTimeStamp);
	prevTimeStamp = nextTimeStamp;

	timer(deltaTime);
}

void timer (float dt) {
	static float elapsed = 0.0f;
	static int frames = 0;

	elapsed += dt;
	frames++;
	if (elapsed >= 1.0f) {
		elapsed -= 1.0f;
		fprintf(stdout,"Frames: %d\n",frames);
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

	while (glfwWindowShouldClose(window) == false) {
		update();
		render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	finalize();
	return EXIT_SUCCESS;
}
