#include "Window.h"
#pragma once
class Input{
private:
	Window *window;
	bool running;
	SDL_Event e;
public:
	Input(Window *mainWindow);
	~Input();

	void inputLoop();
};