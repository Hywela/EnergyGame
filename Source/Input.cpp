#include "Input.h"


Input::Input(Window *mainWindow){
	window = mainWindow;
	running = true;
}


Input::~Input(){
}


void Input::inputLoop(){
	while (running){
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_KEYDOWN: {
									  printf("%s\n", "Key down");
									  break;
				}
				case SDL_MOUSEBUTTONDOWN: {
											  printf("%s\n", "Mouse down");
											  window->applyForce(e.button.x, e.button.y);
											  break;
				}
				case SDL_MOUSEBUTTONUP: {
											printf("%s\n", "Mouse Up");
											break;
				}
				case SDL_MOUSEMOTION: {
										  printf("%s\n", "Mouse Motion");
										  break;
				}
			}
		}
	}
}