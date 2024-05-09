
/*

SHAPES VIEWER WITH MULTIPLE VIEW MODES

Use keys UP, DOWN, LEFT, and RIGHT arrows to move the objects along X and Y axis.
Use keys W, A, S, and D letters to rotate the objects along X and Y axis.
Use I and O letters to move the objects along Z axis.
Use P letter to enable/disable wireframe view mode.

*/

// Includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL2/SOIL2.h>
#include "Utils.h"
using namespace std;

// Define the vertex Array Object and Vertex Buffer Objects
#define numVAOs 1
#define numVBOs 3

// Declaring variables used across Source.cpp
float cameraX, cameraY, cameraZ; // Declare float values for camera position on XYZ.
float cubeLocX, cubeLocY, cubeLocZ; // Declare float values for cube position on XYZ
float pyrLocX, pyrLocY, pyrLocZ; // Declare float values for pyriamid position on XYZ.
float moveSpeed = 0.1f; // Speed of cube moving along x and y axis.
float rotateSpeed = 0.1f; // Speed of cube rotation
float angleX = 0.0f; // Rotation angle around X-axis
float angleY = 0.0f; // Rotation angle around Y-axis
bool wireframeModeEnabled = false; // Variable to track wireframe mode.
GLuint renderingProgram;
GLuint textureImage;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// Declaring variables used in display() function, so that they won’t need to be allocated during rendering
GLuint mvLoc, projLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;

void setupVertices(void) { // 36 vertices, 12 traingles, makes a 2x2x2 cube.
	float cubePositions[108] = {
		-1.0f,  1.0f, -1.0f,	-1.0f, -1.0f, -1.0f,	1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,	1.0f,  1.0f, -1.0f,		-1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,	1.0f, -1.0f,  1.0f,		1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,	1.0f,  1.0f,  1.0f,		1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,	-1.0f, -1.0f,  1.0f,	1.0f,  1.0f,  1.0f,
		 -1.0f, -1.0f,  1.0f,	-1.0f,  1.0f,  1.0f,	1.0f,  1.0f,  1.0f,
		 -1.0f, -1.0f,  1.0f,	-1.0f, -1.0f, -1.0f,	-1.0f,  1.0f,  1.0f,
		 -1.0f, -1.0f, -1.0f,	-1.0f,  1.0f, -1.0f,	-1.0f,  1.0f,  1.0f,
		 -1.0f, -1.0f,  1.0f,	1.0f, -1.0f,  1.0f,		1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,	-1.0f, -1.0f, -1.0f,	-1.0f, -1.0f,  1.0f,
		 -1.0f,  1.0f, -1.0f,	1.0f,  1.0f, -1.0f,		1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,	-1.0f,  1.0f,  1.0f,	-1.0f,  1.0f, -1.0f
	};

	// pyramid with 18 vertices, comprising 6 triangles (four sides, and two on the bottom).
	float pyramidPositions[54] = { 
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,// front face
		1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,// right face
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,// back face
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,// left face
		-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base - left front
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f// base - right back
	};

	// Texture coordinates to allow the brick texture to cover the traingles within the pyramid.
	float pyrTexCoords[36] = {
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	};

	// Texture coordinates to allow the brick texture to cover the traingles within the cube.
	float cubeTexCoords[72] = {
		// Front face
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		// Back face
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		// Top face
		0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		// Bottom face
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		// Right face
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		// Left face
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	};
	
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	// Bind and fill data for cube positions
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions, GL_STATIC_DRAW);

	// Bind and fill data for pyramid positions
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);

	// Bind and fill data for pyramid texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyrTexCoords), pyrTexCoords, GL_STATIC_DRAW);

	// Bind and fill data for cube texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexCoords), cubeTexCoords, GL_STATIC_DRAW);
}

void init(GLFWwindow* window) { // application specific initialization tasks.
	renderingProgram = createShaderProgram(); // create shader program is in Utils.cpp
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
	cubeLocX = -2.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f; // shift down Y to reveal perspective.
	pyrLocX = 1.0f; pyrLocY = -2.0f, pyrLocZ = 0.0f;
	setupVertices();
	// Creates the OpenGL texture from the loadTexture function in Utils.cpp
	textureImage = loadTexture("brick-face.jpg");
} 

// Function for detecting key press to move shapes along x, y.
void moveShape(int direction) { 
	switch (direction) {
	case GLFW_KEY_UP:
		cubeLocY += moveSpeed;
		pyrLocY += moveSpeed;
		break;
	case GLFW_KEY_DOWN:
		cubeLocY -= moveSpeed;
		pyrLocY -= moveSpeed;
		break;
	case GLFW_KEY_LEFT:
		cubeLocX -= moveSpeed;
		pyrLocX -= moveSpeed;
		break;
	case GLFW_KEY_RIGHT:
		cubeLocX += moveSpeed;
		pyrLocX += moveSpeed;
		break;
	}
}

// Function for detecting key press to rotate shapes along x, y. And moving along Z.
void rotateShape(int direction) { 
	switch (direction) {
	case GLFW_KEY_W:
		angleX += rotateSpeed;
		break;
	case GLFW_KEY_S:
		angleX -= rotateSpeed;
		break;
	case GLFW_KEY_A:
		angleY += rotateSpeed;
		break;
	case GLFW_KEY_D:
		angleY -= rotateSpeed;
		break;
	case GLFW_KEY_I: // Zoom in along Z-axis
		cubeLocZ -= moveSpeed;
		pyrLocZ -= moveSpeed;
		break;
	case GLFW_KEY_O: // Zoom out along Z-axis
		cubeLocZ += moveSpeed;
		pyrLocZ += moveSpeed;
		break;
	}
}

void toggleWireframeMode() {
	wireframeModeEnabled = !wireframeModeEnabled; // Toggle wireframe mode
	if (wireframeModeEnabled) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Enable wireframe mode
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Disable wireframe mode
	}
}

void display(GLFWwindow* window, double currentTime) { // code for drawing to the GLFWwindow.
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);

	// get the uniform variables for the MV and projection matrices
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	// build perspective matrix
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

	// DRAW THE CUBE
	// build view matrix, model matrix, and model - view matrix
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
	mMat = glm::rotate(mMat, angleX, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
	mMat = glm::rotate(mMat, angleY, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
	mvMat = vMat * mMat;
	// copy perspective and MV matrices to corresponding uniform variables
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	// adjust OpenGL settings and draw cube
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// DRAW THE PYRAMID
	// build view matrix, model matrix, and model - view matrix
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ)); // Reset view matrix
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(pyrLocX, pyrLocY, pyrLocZ)); // Translate pyramid to its position
	mMat = glm::rotate(mMat, angleX, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate pyramid around X-axis
	mMat = glm::rotate(mMat, angleY, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate pyramid around Y-axis
	mvMat = vMat * mMat;
	// copy perspective and MV matrices to corresponding uniform variables
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// DRAW TEXTURES
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureImage);

	// Draw the pyramid
	glDrawArrays(GL_TRIANGLES, 0, 18);

}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		moveShape(key);
		rotateShape(key);
		if (key == GLFW_KEY_P) {
			toggleWireframeMode(); // Toggle wireframe mode when W is pressed
		}
	}
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); } // GLFW is initialized.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Code must be compatible with GLFW version 4.3.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // version 4.3.
	GLFWwindow* window = glfwCreateWindow(1200, 800, "Shapes Viewer with multiple viewmodes, by Leon Hill", NULL, NULL); // GLFW window is created.
	glfwMakeContextCurrent(window); // Makes the window 'current', drawing it.
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); } // GLEW is initialized.
	glfwSwapInterval(1); // Vsync.

	init(window); // calls init once.

	glfwSetKeyCallback(window, keyCallback); // Set the key callback function

	while (!glfwWindowShouldClose(window)) // Rendering loop calling the display function repetedly. Terminates on user quitting.
	{
		display(window, glfwGetTime()); // calls display() repetedly. GetTime ensures animations run a the same speed.
		glfwSwapBuffers(window); // Vsync and paints the screen.
		glfwPollEvents(); // handles events such as key pressing.
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
