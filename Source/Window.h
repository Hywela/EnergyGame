#pragma once
#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <SDL_image.h>
#include <vector>
#include <Box2D\Box2D.h>
#include "World.h"
using namespace std;



class Window {
private:

	World *world;
	int screenheight;
	int screenwidth;
	int flags;
	bool running;
	int timer;

	SDL_Window* window;
	SDL_GLContext context;
	SDL_Event e;
	
	
public:
	Window(int w = 800, int h = 600);
	~Window();
	void mainLoop();
	void SetupSDL();
	void SetupOGL();
	void Render();
	void RenderOrtho();
	void RenderCube(float scalex = 1, float scaley = 1, float scalez = 1);
	void CheckKeyEvent(SDL_Event e);
	void SetupWorld();
	SDL_Window* getSDLWindow();
};