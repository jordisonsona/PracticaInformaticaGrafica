#pragma once
#define GLEW_STATIC
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <string.h>
using namespace std;

class Shader
{
public:
	GLuint Program;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();
	void Use();
};

