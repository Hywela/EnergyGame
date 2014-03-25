#pragma once
#include "Render.h"
#include <SDL.h>
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <Box2D\Box2D.h>
#include "InputQueue.h"
#include "InputData.h"
#include <iostream>
#include "Consts.h"
#include "RenderQue.h"
#include <string>
#include <Windows.h>
#include "SDL_ttf.h"
#include "Init.h"

/**
	Render.h 
	Class for sending data to the renderer and interacting with the renderer.
	Gets information data from world.h/.cpp (what to draw)
	Que based class.
												-Kristoffer
**/
class Render  {
private:
	Init *init;
	TTF_Font *font;
	static Render *instance;
	InputQueue *inQueue;
	int screenWidth;
	int screenHeight;
	int maxWidth, maxHeight;
	int minWidth, minHeight;
	RenderQue *renderQueue;
	bool renderNow;
	bool shutDown;
	
	//Camer
	int cameraX;
	int cameraY;
	//MainMenue
	TTF_Font *menueFont;
	struct button{
		int posX;
		int posY;
		string tekst;
		b2Vec3 color;
	};
	vector<button> *menueObjects;
public:
	//constructor
	Render(Init *init, InputQueue *que, RenderQue *rque);
	~Render();
	//Loops //call for rendering 
	void (Render::*loop)();
	void mainLoop(string fps = "", string puz = "", string par = "");
	void renderLoop(RenderQue *renQue, InputQueue *que);
	void mainMenue();
	//init
	void initiaze(int h, int w, InputQueue *que);
	void setUpOGL();
	void setUpSDL(int flags);
	//render
	void render();
	void drawCircle(b2Vec2 center, float angle, float radius, b2Vec3 color);
	void drawSquare(b2Vec2* points, b2Vec2 center, float angle, b2Vec3 color);
	void renderOrtho();
	void renderThis(); //bool
	void renderText(const TTF_Font *Font, const GLubyte& R, const GLubyte& G, const GLubyte& B,
		const double& X, const double& Y, const double& Z, const std::string& Text);
	void startRendering();
	void endRendering();
	//ques
	void setQue(InputQueue *que);
	RenderQue* getQue();
	void pushBackMenueObj(int posX,int posY, string tekst);
	//controll manipulasjon
	int menueMouseClickCheck(int x, int y);
	void menueMouseHoverCheck(int x, int y);
	void setCameraDirectionX(int offsett);
	void zerOutCamera();
	Init* getInit();
};

