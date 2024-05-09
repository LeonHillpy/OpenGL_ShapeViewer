
#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Declare function prototypes
void printShaderLog(GLuint shader);
void printProgramLog(int prog);
bool checkOpenGLError();
std::string readShaderSource(const char *filePath);
GLuint createShaderProgram();
GLuint loadTexture(const char *texImagePath);