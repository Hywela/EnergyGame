#pragma once
#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <SDL_image.h>

class Init
{
private:
	int screenWidth;
	int screenHeight;
	Uint32 flags;
public:
	SDL_Window* window;
	SDL_GLContext glContext;

	Init( Uint32 flags);
	void OpenGL();
	void SDL();
	int getScreenHeight();
	int getScreenWidth();
	int  CleanUp();

};
