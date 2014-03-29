#include "Texture.h"
/*
Texture::Texture(const char* texturefilename)
{
	texture = IMG_Load(texturefilename);
	textureID = 0;
	
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 
				 0, GL_RGB, 
				 texture->w, 
				 texture->h, 
				 0, 
				 GL_RGB, 
				 GL_UNSIGNED_BYTE, 
				 texture->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GLuint Texture::GetTexture()
{
	return textureID;
}

void Texture::EnableTexture(GLhandleARB shaderprogram, 
							const char* textureVariable)
{
	int texUnitLoc = glGetUniformLocation(shaderprogram, textureVariable);

	glProgramUniform1i(shaderprogram, texUnitLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

}*/