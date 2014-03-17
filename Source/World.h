#pragma once
#include "Circle.h"
#include "Platform.h"
#include <random>
#include <fstream>
#include <Windows.h>
#include <freeglut\freeglut.h>
#include "Consts.h"
#include "InputData.h"
#include "InputQueue.h"
#include "RenderData.h"
#include "RenderQue.h"
#include "Render.h"
#include "Puzzle.h"
#include "Particle.h"
using namespace std;



const int ROOF = 0;
const int FLOOR = 600;
const int WALLSIZE = 10;
const float START_CAMERASPEED = 5;
const float CAMERASPEED_INCREASE = 0;
const int START_WALLS = 10;
const int WALL_INCREASE = 4;
const int WALL_DISTANCE = 100;
const int START_PARTICLES = 20;


class World {
private:
	//Main world objects
	b2World *world;
	Platform *platform;
	Circle *circle;
	int screenwidth, screenheight;

	//Camera variables
	int spawnX;
	int camOffX, camOffY;
	float cameraSpeed, newSpeed;

	//Puzzle variables
	vector <b2Body*> *platforms;
	vector <Puzzle*> *puzzles;
	vector <b2Vec3> *platformColors;
	int puzzleId;
	bool inProgress;
	int puzzlesSolved;

	//Player variables
	b2Body *playerBody;
	b2Vec3 playerColor;
	vector <Particle*> *particles;
	int numParticles;
	bool lost;

	//Random wall variables
	int spawnCooldown;
	int numWalls;

	InputQueue *inputQueue;
	RenderQue *renderQueue;

public:
	World(int screenWidth, int screenHeight, InputQueue *inQueue, RenderQue *renderQue);
	~World();
	void checkForInput();

	//Main world functions
	void setupWorld();
	void step();
	b2Body* addCircle(int x, int y, float radius, bool dyn, int grp = 1);
	b2Body* addMainChar(int x, int y, float radius, bool dyn, int grp = 1);
	b2Body* addRect(int x, int y, int w, int h, bool dyn, int grp = 1);
	b2Body* addInvisibleWall(int x, int y, int w, int h, bool dyn, int grp = 1);

	//Update functions
	void updateWorld();
	void updatePlatforms();
	void updateChar();

	//Puzzle functions
	void loadPuzzles(string file);
	void spawnPuzzle(int puzzleId);
	bool startPuzzle();
	bool endPuzzle();
	void spawnGroundParticles(int n, b2Vec2 pos, int r);
	void setPuzzle(int id);
	int getPuzzlesSolved();

	//Player functions
	void applyForce(int x, int y);
	void joinCircleJoints();
	int shootParticle(int x, int y);
	void spawnCharacter();
	int getParticlesLeft();

	//Random wall functions
	void cleanWalls();
	void cleanParticles();
	void spawnRandomWalls();
};
