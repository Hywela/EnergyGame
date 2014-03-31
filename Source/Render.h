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
#include "PlatformVBO.h"
#include "ParticleVBO.h"
#include "Shader.h"

/**
	Render.h 
	Class for sending data to the renderer and interacting with the renderer.
	Gets information data from world.h/.cpp (what to draw)
	Que based class.
												-Kristoffer
**/

class Render  {
private:
	//test


	//
	Init *init;
	int screenWidth;
	int screenHeight;
	int maxWidth, maxHeight;
	int minWidth, minHeight;
	bool renderNow;
	bool shutDown;
	b2Vec2 mousePointer;
	//Camer
	int cameraX;
	int cameraY;
	//MainMenu
	TTF_Font *font, *menuFont, *popupFont;
	struct button{
		b2Vec2 buttonBox[4];
		int posX;
		int posY;
		string tekst;
		SDL_Color color;
		boolean disabled;
	};

	vector<button> *popupButtons, *menuObjects, *pauseObjects, *scoreButtons, *scoreTexts, *settingsButtons;
	PlatformVBO *platformVBO,*backgroundVBO, *pauseVBO;
	ParticleVBO *particleVBO, *mainCharParticleVBO;
	
	//shader
	Shader *shader;
	GLint mUniform;
	
	//Unifroms
	GLint lightColor;
	GLint mUniformscreenHeight;
	GLint lightAttenuation;
	GLint radius;
	GLint lightpos;
	GLint platformLightpos;
	GLint numLigth;
	

public:
	//constructor
	Render(Init *init);
	~Render();
	//Loops //call for rendering 
	void (Render::*loop)();
	void mainLoop(string fps = "", string puz = "", string par = "", string sco = "");
	void renderLoop(RenderQue *renQue, InputQueue *que);
	void mainMenu(string fps);
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
	void renderText(const TTF_Font *Font, SDL_Color Color,
		const double& X, const double& Y, const double& Z, const std::string& Text);
	void startRendering();
	void endRendering();
	void pauseLoop();
	void popupMenu();
	void scoreLoop(vector <int> scores, int scoreFinal, int scorePos, bool inGame);
	void settingsLoop(int musVol);

	void pushOrClearPopupMenu(int type, int x, int y);
	void pushBackMenuObj(int posX,int posY, string tekst);
	void pushBackPauseObj(int posX, int posY, string tekst);
	void pushBackScoreBtn(int posX, int posY, string tekst);
	void pushBackScoreTxt(int posX, int posY, string tekst);
	void pushBackSettingsBtn(int posX, int posY, string tekst);

	//controll manipulasjon
	void setMousePointLigth(int x, int y);
	int menuMouseClickCheck(int x, int y);
	int pauseMouseClickCheck(int x, int y);
	int scoreMouseClickCheck(int x, int y);
	int settingsMouseClickCheck(int x, int y);
	void menuMouseHoverCheck(int x, int y, int type);
	void setCameraDirectionX(int offsett);
	void zeroOutCamera();
	void setBackgroundSquare(int posX, int posY, int offsetX, int offsetY, b2Vec3 color, PlatformVBO *tempVBO);
	Init* getInit();
	PlatformVBO* getPlatformVBO();
	ParticleVBO* getParticleVBO();
	ParticleVBO* getMainCharParticleVBO();
};

