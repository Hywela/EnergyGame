#pragma once
#include <SDL.h>
#include <vector>
#include "Circle.h"
#include <random>
#include <ctime>
using namespace std;



class Player {
private:
	float x, y, z;
	Circle* body;
	vector <Circle*> energyOrbs;
	int fieldRadius;
	float fieldForce;

public:
	Player(float x, float y);
	~Player();

	void Render();
	void GetPosition(float &x, float &y);
	void SpawnOrb();
	void Update();
	void PickRandomFieldSpot(int &x, int &y);
	void Keyboard(SDL_Event e);
	void Move(float speedx, float speedy);
};