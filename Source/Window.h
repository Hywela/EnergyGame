#pragma once
#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <gl\GL.h>
#include <SDL_image.h>
#include <vector>
#include <Box2D\Box2D.h>
#include "World.h"
#include "InputData.h"
#include "InputQueue.h"
#include <thread>
#include "RenderData.h"
#include "RenderQue.h"
#include "Render.h"
using namespace std;



class Window {
private:
	World *world;
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
	InputQueue *inQueue;
	RenderQue *renderQueue;
	thread *worldSimulation;
	thread *render;
	Render *ren;

	
public:
	Window(int w = 800, int h = 600);
	~Window();

	void mainLoop();
	void setupWorld();

};