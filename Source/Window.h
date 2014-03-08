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
#include <thread>
#include "InputData.h"
using namespace std;



class Window {
private:
	World *world;
	InputData *worldQue;
	int screenheight;
	int screenwidth;
	int maxWidth, maxHeight;
	int minWidth, minHeight;
	int flags;
	bool running;
	bool isFullscreen;
	int timer;
	SDL_Window* window;
	SDL_GLContext context;
	SDL_Event e;
	thread *worldSimulation;
	
public:
	Window(int w = 800, int h = 600);
	~Window();
	void threading();
	void mainLoop();
	void SetupSDL();
	void SetupOGL();
	void Render();
	void RenderOrtho();
	void RenderCube(float scalex = 1, float scaley = 1, float scalez = 1);
	void CheckKeyEvent(SDL_Event e);
	void SetupWorld();
	World* GetWorld();
};