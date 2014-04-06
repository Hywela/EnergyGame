#pragma once
#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <gl\GL.h>
#include <SDL_image.h>
#include <vector>
#include <Box2D\Box2D.h>
#include <SDL_mixer.h>
#include <fstream>
#include "World.h"
#include "InputData.h"
#include "InputQueue.h"
#include <thread>
#include "RenderData.h"
#include "RenderQue.h"
#include "Render.h"
#include "Init.h"

using namespace std;


	

class Window {
private:
	bool tempOFFON;
	World *world;
	bool running;
	bool isFullscreen;
	bool inGame;
	bool paused;
	bool showDebug;
	int timer;
	SDL_Event e;
	Render *ren;
	vector <int> highScores;
	int scoreFinal, scorePos;
	menu currentMenu;
	int musicVolume, effectVolume;
	
	//FPS test variables
	Uint32 fps_lasttime;
	Uint32 fps_current;
	Uint32 fps_frames;
	
public:
	Window();
	~Window();
	void (Window::*leftMouseClick)();
	void (Window::*loopType)();
	void checkForMouseInput();
	void startWorld();
	void gameLoop();
	void menuLoop();
	void mainLoop();
	void pauseLoop();
	void scoreLoop();
	void settingsLoop();
	void setupWorld();
	void gameLeftMouseClick();
	void menuLeftMouseClick();
	void pauseLeftMouseClick();
	void scoreLeftMouseClick();
	void settingsLeftMouseClick();
	void buildMenu();
	void resumeGame();
	void restartGame();
	void exitGame();
	void pauseGame();
	void showHighscores();
	void newGame();
	void showSettings();
	void endGame();
	void saveData();
	void loadData();
};