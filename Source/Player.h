#pragma once
#include <SDL.h>
#include <vector>
#include "Circle.h"
#include <random>
#include <ctime>
#include "Consts.h"


using namespace std;



class Player {
private:
	 
	float x, y, z;
	Circle* body;
	vector <Circle*> energyOrbs;
	int fieldRadius;
	float fieldForce;

public:
	Player();
	Player(float x, float y);
	~Player();
	void draw( b2Vec2 center, float angle);
	void Render();
	void GetPosition(float &x, float &y);
	void SpawnOrb();
	void Update();
	void PickRandomFieldSpot(int &x, int &y);
	void Keyboard(SDL_Event e);
	void Move(float speedx, float speedy);
	void addCircles(int x, int y, int w, int h, bool dyn, b2World *world);
	
};