#pragma once
#include "Window.h"



class Input {
private:
	Window *window;
	bool running;
	SDL_Event e;

public:
	Input(Window *mainWindow);
	~Input();

	void inputLoop();
};
