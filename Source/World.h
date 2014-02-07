#pragma once
#include "Circle.h"
#include "Platform.h"


class World
{
private:
	Circle *circleObject;
	b2World *world; // The world 
	Platform *platform;
	Circle *circle;
	vector <b2Body*> *platforms;
	vector <b2Body*> *circles;
	int  screenwidth , screenheight;

public:
	World(int screenwidth, int screenheight);
	~World();
	void setupWorld();
	void updateWorld();
	void step();
	void addNewCircle(int x, int y, float radius);
	void addNewRect();
	b2Body* addCircle(int x, int y, float radius, bool dyn);
	b2Body* addRect(int x, int y, int w, int h, bool dyn);
	void applyForce(int x, int y);

};

