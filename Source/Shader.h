#ifndef OMK_SHADER_H_
#define OMK_SHADER_H_

#include <iostream>
#include <fstream>
#include <glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "FileReader.h"

class Shader
{
private:
	GLhandleARB vertexShader,
				fragmentShader,
				shaderProgram;
	void		printInfoLog(GLhandleARB obj);
	std::string ReadFile(const char* filename);
public:
	Shader(const char* vertexshaderfilename, const char* fragmentshaderfilename);
	GLhandleARB* GetShaderProgram();
};

#endif