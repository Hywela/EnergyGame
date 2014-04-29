#include "Texture.h"


Texture::Texture(const char* imagepath)
{

	
	textureImage = IMG_Load(imagepath);
	if (!textureImage)
	{
		fprintf(stderr, "Couldn't load %s.\n", imagepath);
		
	}
	else{
		GLint  nOfColors;
		GLenum texture_format;
	
	// get the number of channels in the SDL surface
	nOfColors = textureImage->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
		if (textureImage->format->Rmask == 0x000000ff)
			texture_format = GL_RGBA;
		else
			texture_format = GL_BGRA;
	}
	else if (nOfColors == 3)     // no alpha channel
	{
		if (textureImage->format->Rmask == 0x000000ff)
			texture_format = GL_RGB;
		else
			texture_format = GL_BGR;
	}
	else {
		printf("warning: the image is not truecolor..  this will probably break\n");
		// this error should not go unhandled
	}
	textureID = 0;
glGenTextures(1, &textureID);
	glEnable(GL_TEXTURE_2D);


	
	
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D,
		0, nOfColors,
		textureImage->w,
		textureImage->h,
		0,
		texture_format,
		GL_UNSIGNED_BYTE,
		textureImage->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	SDL_FreeSurface(textureImage);
	glDisable(GL_TEXTURE_2D);
	}
}


Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
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