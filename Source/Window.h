#pragma once
#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <SDL_image.h>
#include <vector>
#include "Box.h"
#include "Player.h"
using namespace std;



class Window {
private:
	int screenheight;
	int screenwidth;
	int flags;
	bool running;
	bool mouseHeld;
	int lastx, lasty;
	int timer;
	float rotx, roty, rotz;

	SDL_Window* window;
	SDL_GLContext context;
	SDL_Event e;
	vector <Box*> boxes;
	Player *player;

public:
	Window(int w = 800, int h = 600);
	~Window();

	void SetupSDL();
	void SetupOGL();
	void Render();
	void RenderScene();
	void RenderGUI();
	void RenderCube(float scalex = 1, float scaley = 1, float scalez = 1);
	void CheckKeyEvent(SDL_Event e);
	void SetupWorld();
};