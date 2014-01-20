#pragma once
#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <SDL_image.h>
#include "SolidSphere.h"
class Window
{
private:
	// GLOBAL OBJECTS
	SDL_Window* window;
	SDL_GLContext context;

	int screenheight = 600;
	int screenwidth = 800;
	int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	
public:
	void SetupSDL();
	void SetupOGL();
	void Render(int x, int y, float yAngle, float xAngle);
	void RenderObject();
	Window();
	~Window();
};

