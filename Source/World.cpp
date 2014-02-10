#include "World.h"



World::World(int screenwidth, int screenheight)
{
	this->screenwidth = screenwidth;
	this->screenheight = screenheight;
	platforms = new vector<b2Body*>;
	circles = new vector<b2Body*>;
}


World::~World()
{
}

void World::setupWorld(){
	world = new b2World(b2Vec2(0.0, 9.81));
	platform = new Platform();
	circle = new Circle();
	world->SetGravity(b2Vec2(0, 0));
	addNewCircle(100, 200, 0.5);
	addNewCircle(250, 250, 0.1);
	//addNewCircle(90, 190, 0.5);
	addNewCircle(120, 120, 0.1);
	joinCircleJoints();
	platforms->push_back(addRect(100, 300, 50, 10, false));
	platforms->push_back(addRect(600, 300, 50, 10, false));
	platforms->push_back(addRect(screenwidth / 2, 0 + 10, screenwidth, 10, false));
	platforms->push_back(addRect(10, screenheight / 2, 10, screenheight, false));
	platforms->push_back(addRect(screenwidth - 10, screenwidth / 3, 10, screenheight, false));
	platforms->push_back(addRect(screenwidth / 2, screenheight - 30, screenwidth, 30, false));

	

}
void World:: updateWorld(){
	b2Body * B = world->GetBodyList();
	while (B != NULL)
	{
		b2Fixture* F = B->GetFixtureList();
		while (F != NULL)
		{
			switch (F->GetType())
			{
			case b2Shape::e_circle:
			{
									  b2CircleShape* circleShape = (b2CircleShape*) F->GetShape();
									  /* Do stuff with a circle shape */
									
									  circle->draw(B->GetWorldCenter(), B->GetAngle(), circleShape->m_radius);
									  break;
			}
			case b2Shape::e_polygon:
			{
									   b2PolygonShape* poly = (b2PolygonShape*) F->GetShape();
									   /* Do stuff with a polygon shape */
									   b2Vec2 points[4];
									   for (int i = 0; i < 4; i++){
										   points[i] = ((b2PolygonShape*) B->GetFixtureList()->GetShape())->GetVertex(i);
									   }
									   platform->draw(points, B->GetWorldCenter(), B->GetAngle());
									   break;
			}

			}
			F = F->GetNext();
		}

		B = B->GetNext();
	}


}
void World::step(){

	world->Step(1.0 / 30.0, 8, 3);
}

void World::addNewCircle(int x, int y, float radius){
	circles->push_back(addCircle(x, y, radius, true));
}
void World::addNewRect(){

}
b2Body* World::addCircle(int x, int y, float radius, bool dyn){

	b2BodyDef bodydef;
	bodydef.position.Set(x*P2M, y*P2M);
	if (dyn)
		bodydef.type = b2_dynamicBody;
	b2Body* body = world->CreateBody(&bodydef);

	b2CircleShape shape;

	shape.m_radius = radius;
	shape.m_p.Set(0, 0);

	b2FixtureDef fixturedef;
	fixturedef.shape = &shape;
	fixturedef.density = 1.0;

	fixturedef.friction = 0.0;
	fixturedef.restitution = 0.0;
	body->CreateFixture(&fixturedef);
	//body->SetUserData(&ground);

	return body;
}

b2Body* World::addRect(int x, int y, int w, int h, bool dyn)
{

	b2BodyDef bodydef;
	bodydef.position.Set(x*P2M, y*P2M);
	if (dyn)
		bodydef.type = b2_dynamicBody;
	else
		bodydef.type = b2_staticBody;

	b2Body* body = world->CreateBody(&bodydef);

	b2PolygonShape shape;
	shape.SetAsBox(P2M*w / 2, P2M*h / 2);

	b2FixtureDef fixturedef;
	fixturedef.shape = &shape;
	fixturedef.density = 1.0;
	fixturedef.friction = 0.0;
	fixturedef.restitution = 0.8;
	body->CreateFixture(&fixturedef);
	body->SetUserData(0);
	return body;
}
void World::applyForce(int x, int y){

	int ant = circles->size();
	for (int i = 0; i < 1; i++){
		b2Body * tempBody = circles->at(i);
		b2Vec2 point = tempBody->GetPosition();
		b2Vec2 temp = { 0, 0 };
		b2Vec2 tempXY = tempBody->GetPosition();
		b2Vec2 myForce( tempXY.y - y , tempXY.x - x);
		

			tempBody->ApplyForce(myForce, point, true);
	}
}
void World::joinCircleJoints(){

	int ant = circles->size();
	b2Body * mainBody = circles->at(0);
	for (int i = 1; i < ant; i++){
		b2Body * tempBody = circles->at(i);
		b2DistanceJointDef  *join = new b2DistanceJointDef();

	



		join->Initialize(mainBody, tempBody, mainBody->GetPosition(), tempBody->GetPosition() );
		join->collideConnected = true;
	
		world->CreateJoint(join);
	
	}


}