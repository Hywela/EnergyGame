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
				geomShader,
				shaderProgram;
	void		printInfoLog(GLhandleARB obj);
	std::string ReadFile(const char* filename);
public:
	Shader(const char* vertexshaderfilename, const char* fragmentshaderfilename);
	Shader(const char* vertexshaderfilename, const char* fragmentshaderfilename, const char* gsgn);
	GLhandleARB* GetShaderProgram();
};

#endif