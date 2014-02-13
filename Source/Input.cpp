#include "Input.h"


Input::Input(Window *mainWindow){
	window = mainWindow;
	running = true;
	//world = window->getWorld();
	flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
}


Input::~Input(){
}


void Input::inputLoop(){
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Couldnt init SDL2! SDL_Error: " << SDL_GetError() << endl;
	}

	SDLwindow = SDL_CreateWindow("First SDL2 OGL App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, flags);
	context = SDL_GL_CreateContext(SDLwindow);

	while (running){
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_KEYDOWN: {
									 // printf("%s\n", "Key down");
									  break;
				}
				case SDL_MOUSEBUTTONDOWN: {
											  printf("%s\n", "Mouse down");
											  //world->applyForce(e.button.x, e.button.y);
											  break;
				}
				case SDL_MOUSEBUTTONUP: {
											//printf("%s\n", "Mouse Up");
											break;
				}
				case SDL_MOUSEMOTION: {
										 // printf("%s\n", "Mouse Motion");
										  break;
				}
			}
		}
	}
}