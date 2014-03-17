#pragma once
#include "Render.h"
#include <SDL.h>
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <freeglut\freeglut.h>
#include <Box2D\Box2D.h>
#include "InputQueue.h"
#include "InputData.h"
#include <iostream>
#include "Consts.h"
#include "RenderQue.h"
#include <string>
#include <Windows.h>
#include "SDL_ttf.h"
#include <string>
/**
	Render.h 
	Class for sending data to the renderer and interacting with the renderer.
	Gets information data from world.h/.cpp (what to draw)
	Que based class.
												-Kristoffer
**/
class Render  {
private:

	TTF_Font *font;
	static Render *instance;
	InputQueue *inQueue;
	int screenwidth;
	int screenheight;
	SDL_Window* window;
	SDL_GLContext context;
	int maxWidth, maxHeight;
	int minWidth, minHeight;
	RenderQue *renderQueue;
	bool renderNow;
	bool shutDown;
public:
	Render(int h, int w, InputQueue *que, RenderQue *rque);
	~Render();
	/*static Render* get(){
		if (!instance) {
			instance = new Render();
		}
		return instance;
	}*/
	void mainLoop(string fps = "", string puz = "", string par = "");
	void setContext(SDL_Window* window, SDL_GLContext context);
	void initiaze(int h, int w, InputQueue *que);
	void renderLoop(RenderQue *renQue, InputQueue *que);
	void render();
	void drawCircle(b2Vec2 center, float angle, float radius, b2Vec3 color);
	void drawSquare(b2Vec2* points, b2Vec2 center, float angle, b2Vec3 color);
	void setUpOGL();
	void renderOrtho();
	void setQue(InputQueue *que);
	void setUpSDL(int flags);
	RenderQue* getQue();
	void renderThis();
	void renderText(const TTF_Font *Font, const GLubyte& R, const GLubyte& G, const GLubyte& B,
		const double& X, const double& Y, const double& Z, const std::string& Text);
	void startRendering();
	void endRendering();
};

