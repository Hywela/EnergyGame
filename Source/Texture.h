#pragma once
#include <iostream>
#include <fstream>
#include <glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <SDL.h>
#include <SDL_image.h>
class Texture
{
private:
	GLuint textureID;
	SDL_Surface* textureImage;
public:
	Texture(const char* imagepath);
	~Texture();
	void EnableTexture(GLhandleARB shaderprogram, const char* textureVariable);
	GLuint GetTexture();
};

