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
	world = new b2World(b2Vec2(0, 0)); //9.81
	platform = new Platform();
	circle = new Circle();
	world->SetGravity(b2Vec2(0, 6));

	//Create particles
	int particleCount = 50; //Temporary
	float degreeStep = 360 / particleCount;
	int posX = 400;
	int posY = 300;
	int fieldRadius = 50;
	float circleRadius = 0.1;
	float pi = 3.14159265;

	//Add central body part for joints
	circles->push_back(addMainChar(posX, posY, 0.3, true, 1));
	//addNewCircle(posX, posY, 0.5, -1);

	for (int d = 0; d < 360; d += degreeStep) {
		float xTurn = cos(d * pi / 180.0F);
		float yTurn = sin(d * pi / 180.0F);
		int dX = (xTurn * fieldRadius);
		int dY = (yTurn * fieldRadius);
		addNewCircle(posX + dX, posY + dY, circleRadius, -1);
	}

	//joinCircleJoints();

	int platformGroup = 1;
	platforms->push_back(addRect(100, 300, 50, 10, false, platformGroup));
	platforms->push_back(addRect(600, 300, 50, 10, false, platformGroup));
	platforms->push_back(addRect(screenwidth / 2, 0 + 10, screenwidth, 10, false, platformGroup));
	platforms->push_back(addRect(10, screenheight / 2, 10, screenheight, false, platformGroup));
	platforms->push_back(addRect(screenwidth - 10, screenwidth / 3, 10, screenheight, false, platformGroup));
	platforms->push_back(addRect(screenwidth / 2, screenheight - 30, screenwidth, 30, false, platformGroup));
	


}
void World:: updateWorld(){
	b2Body * B = world->GetBodyList();

		updateChar();
	while (B != NULL)
	{
		b2Fixture* F = B->GetFixtureList();
		while (F != NULL)
		{
			switch (F->GetType())
			{
			case b2Shape::e_circle:
			{
									  									  /* Do stuff with a circle shape */
							
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

	//Pull particles
	pullParticlesToCenter();
	
}

void World::updateChar(){



	for (int i = 0; i < circles->size(); i++){
		b2Fixture* F = 	circles->at(i)->GetFixtureList();
		b2CircleShape* circleShape = (b2CircleShape*) F->GetShape();
		
			circle->draw(circles->at(i)->GetWorldCenter(), circles->at(i)->GetAngle(), circleShape->m_radius);

	
	}

}
void World::step(){
	world->Step(1.0 / 32.0, 8, 3);
}

void World::addNewCircle(int x, int y, float radius, int grp){
	circles->push_back(addCircle(x, y, radius, true, grp));
}
void World::addNewRect(){

}
b2Body* World::addCircle(int x, int y, float radius, bool dyn, int grp){

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
	fixturedef.density = 0.0;

	fixturedef.friction = 0.0;
	fixturedef.restitution = 0.0;
	fixturedef.isSensor = true;
	fixturedef.filter.groupIndex = grp;
	body->CreateFixture(&fixturedef);
	//body->SetUserData(&ground);

	return body;
}
b2Body* World::addMainChar(int x, int y, float radius, bool dyn, int grp){

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
	fixturedef.density = 0.0;

	fixturedef.friction = 0.0;
	fixturedef.restitution = 0.0;

	fixturedef.filter.groupIndex = grp;
	body->CreateFixture(&fixturedef);
	//body->SetUserData(&ground);

	return body;
}
b2Body* World::addRect(int x, int y, int w, int h, bool dyn, int grp)
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
	fixturedef.restitution = 0.0;

	fixturedef.filter.groupIndex = grp;
	body->CreateFixture(&fixturedef);
	body->SetUserData(0);
	return body;
}
void World::applyForce(int x, int y){
	//Move particles towards center
	int ant = circles->size();
	for (int i = 0; i < ant; i++) {
		//Get handle
		b2Body *tempBody = circles->at(i);
		b2Vec2 tempXY = tempBody->GetPosition();
		tempXY *= M2P;

		//Calculate direction
		b2Vec2 mouseXY = b2Vec2(x, y);
		b2Vec2 dist = mouseXY - tempXY;

		//Calculate force
		const float SPEED = 40; //TODO: Move this into the class
		float xDivider = ((dist.x > 0) ? dist.x : -dist.x); //Dist as positive
		float yDivider = ((dist.y > 0) ? dist.y : -dist.y); //Dist as positive
		float divider = ((xDivider >= yDivider) ? xDivider : yDivider); //Largest dist as positive
		float xGravityForce = 3; //Force to counteract friction
		float yGravityForce = 6; //Force to counteract gravity
		b2Vec2 direction = b2Vec2((dist.x / divider) * xGravityForce, (dist.y / divider) * yGravityForce);
		direction *= SPEED; //Apply base speed of the object

		//Apply force
		tempBody->ApplyForce(-direction, tempXY, true);
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

void World::pullParticlesToCenter() {
	//Move particles towards center
	int ant = circles->size();
	b2Body *mainBody = circles->at(0);
	b2Vec2 mainXY = mainBody->GetPosition();
	mainXY *= M2P;

	for (int i = 1; i < ant; i++){
		//Get handle
		b2Body *tempBody = circles->at(i);
		b2Vec2 tempXY = tempBody->GetPosition();
		tempXY *= M2P;

		//Calculate direction
		b2Vec2 dist = mainXY - tempXY;
		
		//Calculate force
		float xDivider = ((dist.x > 0) ? dist.x : -dist.x); //Dist as positive
		float yDivider = ((dist.y > 0) ? dist.y : -dist.y); //Dist as positive
		float divider = ((xDivider >= yDivider) ? xDivider : yDivider); //Largest dist as positive
		const float SPEED = divider; //TODO: Move this into the class
		b2Vec2 direction = b2Vec2((dist.x / divider), (dist.y / divider));
		direction *= SPEED; //Apply base speed of the object

		//Calculate breakforce to keep you in the field even if you come in shitty fast
		b2Vec2 pull = tempBody->GetLinearVelocity();
		int maxForceClose = 8;
		bool tooQuick = false;

		if (pull.x > maxForceClose || pull.x <-maxForceClose) {
			pull.x /= 1.25;
			tooQuick = true;
		}
		if (pull.y > maxForceClose || pull.y <-maxForceClose) {
			pull.y /= 1.25;
			tooQuick = true;
		}

		//If within the actual field
		if (divider < 50 && tooQuick) {
			tempBody->SetLinearVelocity(pull);
		}
		else if (divider < 5) {
			pull.x *= 1;
			pull.y *= 1;
			tempBody->SetLinearVelocity(pull);
		}

		//Apply force
		tempBody->ApplyForce(direction, tempXY, true);
	}
}