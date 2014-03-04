#pragma once
#include "Circle.h"
#include "Platform.h"
#include <random>
#include <fstream>
#include <Windows.h>
#include "Consts.h"
using namespace std;



const b2Vec3 COLOR_SOLID = b2Vec3(0.7, 0.7, 0.7);
const b2Vec3 COLOR_UNLIT = b2Vec3(0.7, 0, 0);
const b2Vec3 COLOR_LIT = b2Vec3(0, 1, 0);
const b2Vec3 COLOR_BLACK = b2Vec3(0.1, 0.1, 0.1);
const int ROOF = 0;
const int FLOOR = 600;
const int WALLSIZE = 10;
const float CAMERA_SPEED = 3;
struct PuzzleData {
	int x, y, w, h, group;
	bool dynamic, door;
	b2Vec3 color;
	int cameraSpeed, timeLimit;
};


class World {
private:
	//Circle *circleObject;
	b2World *world;
	Platform *platform;
	Circle *circle;
	vector <b2Body*> *platforms;
	vector <b2Body*> *circles;
	vector <b2Joint*> *joints;
	vector <bool> *isFired;
	vector <b2Vec3> *circleColors;
	vector <b2Vec3> *platformColors;
	int screenwidth, screenheight;
	int score, scoreAviable;
	bool winmsg;
	float cameraSpeed;
	int particles;
	int spawnX, spawnY;
	vector <vector <PuzzleData>> *puzzles;
	b2Body* puzzleDoor;
	int doorId;
	int tasksTotal, tasksDone;

public:
	World(int screenwidth, int screenheight);
	~World();

	void setupWorld();
	void updateWorld();
	void step(); 
	void addNewCircle(int x, int y, float radius, int grp = 1);
	void addNewRect();
	b2Body* addCircle(int x, int y, float radius, bool dyn, int grp = 1);
	b2Body* addMainChar(int x, int y, float radius, bool dyn, int grp = 1);
	b2Body* addRect(int x, int y, int w, int h, bool dyn, int grp = 1);
	b2Body* addInvisibleWall(int x, int y, int w, int h, bool dyn, int grp = 1);
	void applyForce(int x, int y);
	void joinCircleJoints();
	void pullParticlesToCenter();
	void updateChar();
	void updatePlatforms();
	bool shootParticle(int x, int y);
	void loadWorld(string file);
	void spawnCharacter();
	void cleanWalls();
	void spawnRandomWalls();
	void loadPuzzles(string file);
	void spawnPuzzle(int puzzleId);
};
