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
#include <Box2D\Box2D.h>
#include "Platform.h"
using namespace std;



class Window {
private:
	b2World *world; // The world 
	b2Body *addCircle;	// 
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
	Platform *platform;
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
	void DoStuffWithShapes();
	void addCircleToWorld();
	void addPlatform();
};