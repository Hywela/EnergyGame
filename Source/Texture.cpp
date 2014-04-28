#include "Texture.h"


Texture::Texture(const char* imagepath)
{

	
	textureImage = IMG_Load(imagepath);
	if (!textureImage)
	{
		fprintf(stderr, "Couldn't load %s.\n", "123.bmp");
		
	}
	else{
	textureID = 0;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D,
		0, GL_RGB,
		textureImage->w,
		textureImage->h,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		textureImage->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	SDL_FreeSurface(textureImage);
	}
}


Texture::~Texture()
{
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

	glBindTexture(GL_TEXTURE_2D, textureID);

}