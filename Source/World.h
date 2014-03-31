#pragma once
#include "Circle.h"
#include "Platform.h"
#include <random>
#include <fstream>
#include <Windows.h>
#include <freeglut\freeglut.h>
#include <SDL_mixer.h>
#include "Consts.h"
#include "Render.h"
#include "Puzzle.h"
#include "Particle.h"
#include "PlatformVBO.h"
using namespace std;



class World {
private:
	PlatformVBO *platformVBO;
	ParticleVBO *particleVBO;
	ParticleVBO *mainCharParticleVBO;

	//Main world objects
	b2World *world;
	
	int screenwidth, screenheight;

	//Camera variables
	float spawnX;
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
	int numParticles, numStored;
	bool lost;
	int score;

	//Random wall variables
	int spawnCooldown;
	int numWalls;

	//Audio variables
	Mix_Music *bg_music;
	Mix_Chunk *snd_Shoot;

	//InputQueue *inputQueue;
	//RenderQue *renderQueue;

public:
	World(int screenWidth, int screenHeight, PlatformVBO *platformRendering, ParticleVBO *particleRendering, ParticleVBO *mainCharRendering);
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
	void storeParticles();
	void retriveParticles();
	bool gameOver();
	int getScore();

	//Random wall functions
	void cleanWalls();
	void cleanParticles();
	void spawnRandomWalls();
};
