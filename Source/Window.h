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
#include "Init.h"

using namespace std;


	

class Window {
private:
	World *world;
	bool running;
	bool isFullscreen;
	int timer;
	SDL_Event e;
	InputQueue *inQueue;
	RenderQue *renderQueue;
	thread *worldSimulation;
	Render *ren;
	
	//FPS test variables
	Uint32 fps_lasttime;
	Uint32 fps_current;
	Uint32 fps_frames;
	
public:
	Window();
	~Window();
	void (Window::*leftMouseClick)();
	void (Window::*loopType)();
	void checkForMouseInput();
	void startWorld();
	void gameLoop();
	void menueLoop();
	void mainLoop();
	void setupWorld();
	void gameLeftMouseClick();
	void menueLeftMouseClick();
	void buildMenue();
	

};