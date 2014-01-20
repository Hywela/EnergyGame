#include <iostream>
#include <SDL.h>
#include "Window.h"
#include "SolidSphere.h"
#include <vector>
SDL_Event e;
float postionX = 0;
float postionY = 0;

void CheckKeyEvent(SDL_Event e)
{
	switch (e.key.keysym.sym)
	{
	case SDLK_RIGHT:
	{
					   postionX += 0.5f;
		break;
	}
	case SDLK_UP:
	{
					   postionY -= 0.5f;
					   break;
	}
	case SDLK_DOWN:
	{
					   postionY += 0.5f;
					   break;
	}
	case SDLK_LEFT:
	{
					  postionX -= 0.5f;
					  break;
	}
	case SDLK_q:
	{
				   //running = false;
				   break;
	}
	default:
	{
			   break;
	}
	}
}
int main(int argc, char *argv[])
{
	std::vector<SolidSphere> sphers;

	Window *win;
	win = new Window();
	int x, y, z;
	bool running = true;
	int mouseX, mouseY, oldMouseX, oldMouseY;
	mouseX = 0; mouseY = 0; oldMouseX = 0; oldMouseY = 0;
	float Xangle = 0.0f;
	float Yangle = 0.0f;
	int timer = SDL_GetTicks();	
	SolidSphere *spher;
	spher = new SolidSphere(1, 24 ,48);
	for (int i = 0; i > 10; i++){
	
	
}
	while (running)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_KEYDOWN:
			{					
							
								CheckKeyEvent(e);
								break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
					
								

			}
			case SDL_MOUSEBUTTONUP:
			{
									 
			}
			default:
			{
					   break;
			}
			}
		}

		Yangle += 1;
		if (Yangle > 360)
			Yangle = 0;
		std::cout << " angle  " << Yangle;
		win->Render(postionX, postionY, Yangle, 0);
		
		SDL_Delay((1000 / 30) - (timer - SDL_GetTicks()));
		timer = SDL_GetTicks();
	}
	return 0;
}