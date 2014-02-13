#include "Window.h"
#pragma once
class Input{
private:
	Window *window;
	World *world;
	bool running;
	SDL_Event e;
	SDL_Window* SDLwindow;
	SDL_GLContext context;
	int flags;
public:
	Input(Window *mainWindow);
	~Input();

	void inputLoop();
};

