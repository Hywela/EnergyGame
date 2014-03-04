#include "World.h"



World::World(int screenwidth, int screenheight) {
	this->screenwidth = screenwidth;
	this->screenheight = screenheight;

	platforms = new vector<b2Body*>;
	circles = new vector<b2Body*>;
	joints = new vector<b2Joint*>;
	isFired = new vector<bool>;
	circleColors = new vector<b2Vec3>;
	platformColors = new vector<b2Vec3>;
}

World::~World() {

}

void World::setupWorld() {
	//Create world objects
	world = new b2World(b2Vec2(0, 0)); //9.81
	platform = new Platform();
	circle = new Circle();
	world->SetGravity(b2Vec2(-6, 0));

	//Load level
	char buffer[64];
	string dataText, argText;
	string levelFile = "Levels/Level_2.txt";
	ifstream lvl(levelFile);
	int phase = 0;

	//Helper variables for loading levels
	int particleCount, x, y, w, h, group;
	string dynamic, type;
	score = 0;
	scoreAviable = 0;

	//Load level
	while (!lvl.eof()) {
		//Read to buffer
		lvl >> buffer;
		argText = buffer;
		dataText = buffer;
		dataText.erase(std::remove(dataText.begin(), dataText.end(), ','), dataText.end());

		if (dataText.find("Particles:") != -1) {
			//Store next read (particle data)
			lvl >> particleCount;
		}
		else if (dataText.find("type") != -1) {
			//Done with description line, start phase 1
			phase = 1;
		}
		else if (phase) {
			//Read platform data
			switch(phase) {
				case 1: {x = atoi(dataText.c_str()); break;}
				case 2: {y = atoi(dataText.c_str()); break;}
				case 3: {w = atoi(dataText.c_str()); break;}
				case 4: {h = atoi(dataText.c_str()); break;}
				case 5: {dynamic = dataText; break;}
				case 6: {group = atoi(dataText.c_str()); break;}
				case 7: {
					type = dataText;
					bool thisDynamic = ((dynamic.find("true") != -1) ? true : false);
					b2Vec3 thisColor = ((type.find("solid") != -1) ? COLOR_SOLID : COLOR_UNLIT);
					scoreAviable += ((type.find("solid") != -1) ? 0 : 1);
					
					//All data collected, create platform
					platforms->push_back(addRect(x, y, w, h, thisDynamic, group));
					platformColors->push_back(thisColor);

					//Reset phase and look for more platforms
					phase = 0;
					break;
				}
			}
			phase++;
		}
	}

	cout << "World loaded!\nObjective: Light up " << scoreAviable << " platforms. You get " << particleCount << " particles!\n";

	//Helper variables for creating particles
	float degreeStep = 360 / particleCount;
	int posX = 400;
	int posY = 300;
	int fieldRadius = 50;
	float circleRadius = 0.1;
	float pi = 3.14159265;

	//Add central body part for joints
	circles->push_back(addMainChar(posX, posY, 0.3, true, 1));
	b2Vec3 mainColor = b2Vec3(1, 1, 1);
	circleColors->push_back(mainColor);

	//Create particles
	for (float d = 0; d < 360; d += degreeStep) {
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

		//Add random color
		int minColor = 1;
		int maxColor = 100;
		float rollR = (minColor + (rand() % (maxColor - minColor))) / 100.0;
		float rollG = (minColor + (rand() % (maxColor - minColor))) / 100.0;
		float rollB = (minColor + (rand() % (maxColor - minColor))) / 100.0;
		b2Vec3 newColor = b2Vec3(rollR, rollG, rollB);
		circleColors->push_back(newColor);
	}

	//Create joints
	joinCircleJoints();
}

void World::updateWorld() {
	//Update world
	updatePlatforms();

	//Update charcter
	updateChar();

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

void World::updateChar() {
	for (int i = 0; i < circles->size(); i++) {
		b2Fixture* F = 	circles->at(i)->GetFixtureList();
		b2CircleShape* circleShape = (b2CircleShape*) F->GetShape();
	
		circle->draw(circles->at(i)->GetWorldCenter(), circles->at(i)->GetAngle(), circleShape->m_radius, circleColors->at(i));
	}
}

void World::updatePlatforms() {
	b2Body *B = world->GetBodyList();
	int colorId = platforms->size() - 1;

	while (B != NULL) {
		b2Fixture* F = B->GetFixtureList();

		while (F != NULL) {
			switch (F->GetType()) {
				case b2Shape::e_polygon: {
					b2PolygonShape* poly = (b2PolygonShape*) F->GetShape();

					b2Vec2 points[4];
					for (int i = 0; i < 4; i++) {
						points[i] = poly->GetVertex(i);
					}

					//Check if platform is unlit
					b2Vec3 curColor = platformColors->at(colorId);
					if (curColor.x == COLOR_UNLIT.x && curColor.y == COLOR_UNLIT.y && curColor.z == COLOR_UNLIT.z) {
						b2Body *platBody = platforms->at(colorId);
						b2Vec2 platPos = platBody->GetPosition();
						b2Vec2 x1 = platPos + points[0];
						b2Vec2 x2 = platPos + points[2];
						platPos *= M2P;
						x1 *= M2P;
						x2 *= M2P;

						//Check if any particles collide
						for (int i = 1; i < circles->size(); i++) {
							if (isFired->at(i - 1)) {
								//Get handle
								b2Body *parBody = circles->at(i);
								b2Vec2 parXY = parBody->GetPosition();
								parXY *= M2P;

								if (parXY.x >= x1.x && parXY.x <= x2.x) {
									if (parXY.y >= x1.y && parXY.y <= x2.y) {
										//Light platform
										curColor = COLOR_LIT;
										platformColors->at(colorId) = COLOR_LIT;
										score++;
										cout << "You have lit (" << score << " / " << scoreAviable << ") platforms!\n";
										if (score == scoreAviable) {
											cout << "Congratulations! You won!\n";
										}
									}
								}
							}
						}
					}

					platform->draw(points, B->GetWorldCenter(), B->GetAngle(), curColor);
					
					colorId--;
					break;
				}
			}

			F = F->GetNext();
		}

		B = B->GetNext();
	}
}

void World::step() {
	world->Step(1.0 / 32.0, 8, 3);
}

void World::addNewCircle(int x, int y, float radius, int grp) {
	circles->push_back(addCircle(x, y, radius, true, grp));
}

void World::addNewRect() {

}

b2Body* World::addCircle(int x, int y, float radius, bool dyn, int grp) {
	b2BodyDef bodydef;
	bodydef.position.Set(x * P2M, y * P2M);
	if (dyn) {
		bodydef.type = b2_dynamicBody;
	}
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

b2Body* World::addMainChar(int x, int y, float radius, bool dyn, int grp) {
	b2BodyDef bodydef;
	bodydef.position.Set(x * P2M, y * P2M);
	if (dyn) {
		bodydef.type = b2_dynamicBody;
	}
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

b2Body* World::addRect(int x, int y, int w, int h, bool dyn, int grp) {
	b2BodyDef bodydef;
	bodydef.position.Set(x * P2M, y * P2M);
	if (dyn) {
		bodydef.type = b2_dynamicBody;
	}
	else {
		bodydef.type = b2_staticBody;
	}
	b2Body* body = world->CreateBody(&bodydef);
	
	b2PolygonShape shape;
	shape.SetAsBox(P2M * w / 2, P2M * h / 2);

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

void World::applyForce(int x, int y) {
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

void World::joinCircleJoints() {
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
	const float SPEED = 400; //TODO: Move this into the class
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

		parBody->SetLinearVelocity(b2Vec2(0, 0));
		parBody->ApplyForce(direction, parXY, true);

		cout << "Particles left: " << particlesLeft - 1 << "!\n";
	}
	else {
		cout << "No particles left!\n";
	}

	return closestParticle;
}
