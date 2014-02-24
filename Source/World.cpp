#include "World.h"



World::World(int screenwidth, int screenheight)
{
	this->screenwidth = screenwidth;
	this->screenheight = screenheight;
	platforms = new vector<b2Body*>;
	circles = new vector<b2Body*>;
	joints = new vector<b2Joint*>;
	isFired = new vector<bool>;
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
	int particleCount = 60; //Temporary
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
		int minRoll = 30;
		int maxRoll = 100;
		float roll = (minRoll + (rand() % (maxRoll - minRoll))) / 100.0;
		dX *= roll;
		dY *= roll;
		addNewCircle(posX + dX, posY + dY, circleRadius, -1);
	}

	joinCircleJoints();

	int platformGroup = 1;
	platforms->push_back(addRect(100, 300, 50, 10, false, platformGroup));
	platforms->push_back(addRect(600, 300, 50, 10, false, platformGroup));
	//platforms->push_back(addRect(screenwidth / 2, 0 + 10, screenwidth, 10, false, platformGroup));
	//platforms->push_back(addRect(10, screenheight / 2, 10, screenheight, false, platformGroup));
	//platforms->push_back(addRect(screenwidth - 10, screenwidth / 3, 10, screenheight, false, platformGroup));
	//platforms->push_back(addRect(screenwidth / 2, screenheight - 30, screenwidth, 30, false, platformGroup));
	


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
	//pullParticlesToCenter();

	//Center camera at center of the particle field
	b2Vec2 windowCenter = b2Vec2(screenwidth / 2, screenheight / 2);
	b2Vec2 fieldCenter = circles->at(0)->GetPosition();
	fieldCenter *= M2P;
	b2Vec2 cameraCenter = fieldCenter - windowCenter;
	cameraCenter *= P2M;
	world->ShiftOrigin(cameraCenter);
}

void World::updateChar(){

	b2Fixture* F = circles->at(0)->GetFixtureList();
	b2CircleShape* circleShape = (b2CircleShape*) F->GetShape();

	b2Vec2 pos = circles->at(0)->GetWorldCenter();
	gluLookAt(0, 0, 0,
		0.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f);

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
	//Pop off closest particles
	bool couldFire = shootParticle(x, y);

	if (couldFire) {
		//Push particles away from mouse
		int ant = circles->size();
		for (int i = 0; i < ant; i++) {
			if (!i || !isFired->at(i - 1)) {
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
	}
}
void World::joinCircleJoints(){
	b2Body *mainBody = circles->at(0);

	//Joint all particles
	int ant = circles->size();
	for (int i = 1; i < ant; i++) {
		b2Body * tempBody = circles->at(i);

		//Initialize join settings
		b2DistanceJointDef  *join = new b2DistanceJointDef();
		join->Initialize(mainBody, tempBody, mainBody->GetPosition(), tempBody->GetPosition());
		join->collideConnected = true;
		join->type = e_ropeJoint; //b2JointType(x);

		//Add joint
		b2Joint *newJoint = world->CreateJoint(join);
		joints->push_back(newJoint);
		isFired->push_back(false);
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

bool World::shootParticle(int x, int y) {
	//Get field position
	b2Body *mainBody = circles->at(0);
	b2Vec2 mainXY = mainBody->GetPosition();
	mainXY *= M2P;

	//Calculate shoot direction
	b2Vec2 mouseXY = b2Vec2(x, y);
	b2Vec2 dist = mouseXY - mainXY;

	//Calculate force
	const float SPEED = 40; //TODO: Move this into the class
	float xDivider = ((dist.x > 0) ? dist.x : -dist.x); //Dist as positive
	float yDivider = ((dist.y > 0) ? dist.y : -dist.y); //Dist as positive
	float divider = ((xDivider >= yDivider) ? xDivider : yDivider); //Largest dist as positive
	float xGravityForce = 1; //Force to counteract friction
	float yGravityForce = 1; //Force to counteract gravity
	b2Vec2 direction = b2Vec2(dist.x / divider, dist.y / divider);
	direction *= SPEED; //Apply base speed of the object

	int closestParticle = 0;
	int closestTotDist = 0;
	int particlesLeft = 0;

	//Find closest particle
	int ant = circles->size();
	for (int i = 1; i < ant; i++) {
		//Get handle
		b2Body *tempBody = circles->at(i);
		b2Vec2 tempXY = tempBody->GetPosition();
		tempXY *= M2P;

		//Calculate particle distance
		b2Vec2 parDist = mouseXY - tempXY;
		int parTotDist = ((parDist.x < 0) ? -parDist.x : parDist.x) + ((parDist.y < 0) ? -parDist.y : parDist.y);

		//Make sure not popped
		if (!isFired->at(i - 1)) {
			particlesLeft++;

			//If first or closer
			if (!closestParticle || parTotDist < closestTotDist) {
				closestParticle = i;
				closestTotDist = parTotDist;
			}
		}
	}

	//If particle found
	if (closestParticle) {
		//Pop off
		int jointId = closestParticle - 1;

		b2Joint *popJoint = joints->at(jointId);
		joints->at(jointId) = joints->at(0);
		isFired->at(jointId) = true;
		world->DestroyJoint(popJoint);

		//Apply force
		b2Body *parBody = circles->at(closestParticle);
		b2Vec2 parXY = parBody->GetPosition();
		parXY *= M2P;
		parBody->ApplyForce(direction, parXY, true);

		cout << "Particles left: " << particlesLeft - 1 << "!\n";
	}
	else {
		cout << "No particles left!\n";
	}

	return closestParticle;
}