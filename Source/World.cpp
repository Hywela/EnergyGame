#include "World.h"



World::World(int screenwidth, int screenheight, PlatformVBO *platformRendering, ParticleVBO *particleRendering, ParticleVBO *mainCharRendering) {
	//Initialize world variables
	this->screenwidth = screenwidth;
	this->screenheight = screenheight;
	platformVBO = platformRendering;
	particleVBO = particleRendering;
	mainCharParticleVBO = mainCharRendering;

	//Initialize camera variables
	spawnX = 0;
	cameraSpeed = START_CAMERASPEED;
	newSpeed = START_CAMERASPEED;
	camOffX = camOffY = 0;

	//Initialize puzzle variables
	platforms = new vector<b2Body*>;
	puzzles = new vector<Puzzle*>;
	platformColors = new vector<b2Vec3>;
	inProgress = false;
	puzzlesSolved = 0;

	//Initialize player variables
	particles = new vector<Particle*>;
	numParticles = START_PARTICLES;
	numStored = 0;
	lost = false;
	score = 0;

	//Initialize random wall variables
	numWalls = START_WALLS - WALL_INCREASE;

	//Set random seed based on time
	time_t t = time(0);

	tm *now = new tm();
	localtime_s(now, &t);

	int h = now->tm_hour;
	int m = now->tm_min;
	int s = now->tm_sec;
	int time = (h * 10000) + (m * 100) + s;
	srand(time);
	
	//Set audio variables
	bg_music = NULL;
	snd_Shoot = NULL;

	//Start world setup
	setupWorld();
}

World::~World() {
	platformVBO->clear();
	particleVBO->clear();
	mainCharParticleVBO->clear();
	delete world;
	delete platforms;
	delete puzzles;
	delete platformColors;
	delete particles;

	//Clear music
	Mix_FreeMusic(bg_music);
	bg_music = NULL;
}

void World::checkForInput() {
/*	bool exit = false;

	while (!exit) {
		if (inputQueue->getSize() > 0) {
			InputData input = inputQueue->pop();	
			//printf("pop item : %i", input.getType());
	
			switch (input.getType()) {
				case 0: {
					//type 0 == mouse click
					applyForce(input.getX(), input.getY());
					break;
				}
				case 1: {
					//type 1 = step
					step();
					break;
				}
				case 2: {
					//type 2 = update world
					updateWorld();	
					break;
				}
			}
		}
	}*/
}



//Main world functions
void World::setupWorld() {
	//Create world objects
	world = new b2World(b2Vec2(0, 0)); //9.81

	world->SetGravity(b2Vec2(0, 0));

	world->ShiftOrigin(b2Vec2(screenwidth / 2, screenheight / 2));
	//Spawn character
	spawnCharacter();
	joinCircleJoints();

	//Center camera at center of the particle field
	b2Vec2 windowCenter = b2Vec2(screenwidth / 2, screenheight / 2);
	b2Vec2 fieldCenter = playerBody->GetPosition();
	fieldCenter *= M2P;
	b2Vec2 cameraCenter = fieldCenter - windowCenter;
	camOffX = cameraCenter.x;
	camOffY = cameraCenter.y;	
	cameraCenter *= P2M;
	world->ShiftOrigin(cameraCenter);

	//Load puzzles
	loadPuzzles("Levels/Puzzles.txt"); 

	int roomW = INCREDIBLY_LONG;
	int roomH = FLOOR - ROOF;
	int roomX = (-camOffX) + (roomW / 2);
	int floorY = (-camOffY) + FLOOR;
	int roofY = (-camOffY) + ROOF;

	spawnX = 1;
	spawnCooldown = 0;

	//Set up edge wall
	platforms->push_back(addInvisibleWall(0, 0, DEATHWALL_SIZE, screenheight, false, -1));
	platformColors->push_back(COLOR_BLACK);

	//Spawn roof and floor
	platforms->push_back(addRect(roomX, floorY, roomW, WALLSIZE, false, 1));
	platformColors->push_back(COLOR_SOLID);

	platforms->push_back(addRect(roomX, roofY, roomW, WALLSIZE, false, 1));
	platformColors->push_back(COLOR_SOLID);

	//Spawn tutorial puzzle
	setPuzzle(0);

	//Load sounds
	string shootSnd = DIR_EFFECTS + "Super Mario - Fireball Effect.wav";
	snd_Shoot = Mix_LoadWAV(shootSnd.c_str());

	//Play music
	string song1 = DIR_MUSIC + "Tobu - Colors.mp3";
	bg_music = Mix_LoadMUS(song1.c_str());

	if (bg_music != NULL) {
		Mix_PlayMusic(bg_music, -1);
	}
	else {
		cout << "ERROR: The background music was not loaded!\n";
	}
}

void World::step(int fps) {
	//checkForInput();
	world->Step(1.0/fps, 8, 3);
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
	fixturedef.restitution = 0.8;
	//fixturedef.isSensor = true;
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
	//fixturedef.isSensor = true;
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

b2Body* World::addInvisibleWall(int x, int y, int w, int h, bool dyn, int grp) {
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
	//fixturedef.isSensor = true;
	fixturedef.filter.groupIndex = grp;
	body->CreateFixture(&fixturedef);
	body->SetUserData(0);

	return body;
}



//Update functions
void World::updateWorld() {
	//Check if player is dead
	b2Vec2 playerPos = playerBody->GetPosition();
	playerPos *= M2P;

	if (playerPos.x < DEATH_WALL_SPACE) {
		lost = true;
	}

	//If not game over
	if (!lost) {
		//Update renderer
		//RenderData update(2);
		//renderQueue->push(update);

		//Update platforms
		updatePlatforms();

		//Update charcter
		updateChar();

		//Clean old walls
		cleanWalls();

		//Clean old particles
		cleanParticles();

		/*	Tells the render that new input to the render stack is inc,
			and to wait til the world is done outputing information 
			to send it to the renderer. */
		//renderQueue->push(update);

		//Update puzzles
		if (!inProgress) {
			//If too far on right side
			if (playerPos.x >= CAMERA_SPEEDUP_RANGE) {
				//Speedup camera
				cameraSpeed += 0.1;
			}
			else {
				//Use normal speed
				cameraSpeed = newSpeed;
			}

			if (startPuzzle()) {
				inProgress = true;
			}
		}
		else {
			if (endPuzzle()) {
				inProgress = false;
				puzzlesSolved++;

				//Spawn new
				int randomPuzzleId = randomRange(1, 4);
				setPuzzle(randomPuzzleId);

				//Spawn particles for bonus
				int offX = puzzles->at(puzzleId)->getSpawn() + 200;
				int offY = 150;
				//spawnGroundParticles(10, b2Vec2(offX, offY), 30);
			}
		}

		//Add camera movement
		if (cameraSpeed) {
			b2Vec2 cameraPos = b2Vec2(cameraSpeed, 0);
			cameraPos *= P2M;
			world->ShiftOrigin(cameraPos);
		
			//Update spawn orgin
			spawnX -= cameraSpeed;

			if (puzzlesSolved) {
				score++;
			}

			//Update puzzle orgin
			puzzles->at(puzzleId)->shiftOrginX(-cameraSpeed);
		}

		//Refresh position of pushwall
		b2Body *invWall = platforms->at(0);
		int invX = (DEATHWALL_SIZE / 2) + 100;
		int invY = (-camOffY) + (FLOOR / 2);
		b2Vec2 moveTo = b2Vec2(invX, invY);
		moveTo *= P2M;
		invWall->SetTransform(moveTo, 0.0);
	}
}

void World::updateChar() {
	b2Vec2 playerPos = playerBody->GetPosition();
	playerPos *= M2P;
	b2Fixture *playerF = playerBody->GetFixtureList();
	b2CircleShape* playerShape = (b2CircleShape*) playerF->GetShape();
	int type = 1;

	//Draw player body
	particleVBO->clear();
	mainCharParticleVBO->clear();
	mainCharParticleVBO->setCenter(playerBody->GetWorldCenter());
	mainCharParticleVBO->pushBack(playerBody->GetWorldCenter(), playerBody->GetAngle(), playerShape->m_radius, playerColor);

	//Update all particles
	int ant = particles->size();
	for (int i = 1; i < ant; i++) {
		Particle *tempParticle = particles->at(i);
		b2Body *tempBody = tempParticle->getBody();
		b2Vec3 tempColor = tempParticle->getColor();
		b2Fixture *F = tempBody->GetFixtureList();
		b2CircleShape* circleShape = (b2CircleShape*) F->GetShape();
	
		//Draw circle body	
		particleVBO->pushBack(tempBody->GetWorldCenter(), tempBody->GetAngle(), circleShape->m_radius, tempColor);
	
		particleVBO->pushBackCenter(tempBody->GetWorldCenter());
		/*if (tempParticle->isFired())
		particleVBO->pushBackCenter(tempBody->GetWorldCenter());*/
		//Update particle	
		tempParticle->update();
	

		//Check for particles on ground
		if (tempParticle->onGround()) {
			b2Vec2 parPos = tempBody->GetPosition();
			parPos *= M2P;

			b2Vec2 playerDist = playerPos - parPos;
			int distX = ((playerDist.x < 0) ? -playerDist.x : playerDist.x);
			int distY = ((playerDist.y < 0) ? -playerDist.y : playerDist.y);
			int totDist = distX + distY;

			//If in pickup range
			if (totDist <= PICKUP_RANGE) {
				//Initialize join settings
				b2DistanceJointDef *join = new b2DistanceJointDef();
				join->Initialize(playerBody, tempBody, playerBody->GetPosition(), tempBody->GetPosition());
				join->collideConnected = true;
				join->type = e_ropeJoint;

				//Add joint
				b2Joint *newJoint = world->CreateJoint(join);
				tempParticle->setJoint(newJoint);

				//printf("Picked up particle!\n");
				numParticles++;
			}
		}
	}
}

void World::updatePlatforms() {
	platformVBO->clear();
	b2Body *B = world->GetBodyList();
	int colorId = platforms->size() - 1;
	//squareVBO->clear();
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
					if (curColor.x == COLOR_LIT.x && curColor.y == COLOR_LIT.y && curColor.z == COLOR_LIT.z) {
						
						platformVBO->pushBackLigthPostionLit(B->GetWorldCenter());
					}
					if (curColor.x == COLOR_UNLIT.x && curColor.y == COLOR_UNLIT.y && curColor.z == COLOR_UNLIT.z) {

						platformVBO->pushBackLigthPostionUnlit(B->GetWorldCenter());

					}


					if (curColor.x == COLOR_UNLIT.x && curColor.y == COLOR_UNLIT.y && curColor.z == COLOR_UNLIT.z) {
						b2Body *platBody = platforms->at(colorId);
						b2Vec2 platPos = platBody->GetPosition();
						b2Vec2 x1 = platPos + points[0];
						b2Vec2 x2 = platPos + points[2];
						platPos *= M2P;
						x1 *= M2P;
						x2 *= M2P;

						
						//Check if any particles collide
						int ant = particles->size();
						for (int i = 0; i < ant; i++) {
							Particle *tempParticle = particles->at(i);

							if (tempParticle->isFired()) {
								//Get handle
								b2Body *parBody = tempParticle->getBody();
								b2Vec2 parXY = parBody->GetPosition();
								parXY *= M2P;

								if (parXY.x >= x1.x - 10 && parXY.x <= x2.x + 10) {
									if (parXY.y >= x1.y - 10 && parXY.y <= x2.y + 10) {
										//Light platform
										curColor = COLOR_LIT;
										
										platformColors->at(colorId) = COLOR_LIT;

										Puzzle *puzzle = puzzles->at(puzzleId);
										puzzle->taskDone();
									}
								}
							}
						}
					}

					//Draw platform
					int a = 0;
					platformVBO->pushBack( points, B->GetWorldCenter(), B->GetAngle(), curColor);
			
					colorId--;
					break;
				}
			}

			F = F->GetNext();
		}

		B = B->GetNext();
	}
}



//Puzzle functions
void World::loadPuzzles(string file) {
	//Load puzzles
	char buffer[64];
	string dataText, argText;
	ifstream lvl(file);
	int phase = 0;

	//Helper variables for loading puzzles
	PartData data;
	string type, dynamic;
	vector <PartData> parts;
	Puzzle *puzzle = NULL;
	int bonus;
	int challenge;

	//Load level
	while (!lvl.eof()) {
		//Read to buffer
		lvl >> buffer;
		argText = buffer;
		dataText = buffer;
		dataText.erase(std::remove(dataText.begin(), dataText.end(), ','), dataText.end());

		if (dataText.find("door") != -1) {
			//Done with description line, start phase 1
			phase = 1;
		}
		else if (phase && dataText.find("puzzle") != -1) {
			lvl >> buffer;

			if (puzzle != NULL) {
				puzzle->setParts(parts);
				puzzle->setChallenge(challenge);
				puzzle->setBonus(bonus);
				puzzle->setScale(screenwidth);
				puzzles->push_back(puzzle);
			}
			puzzle = new Puzzle();
			parts.clear();
			bonus = 0;
			challenge = 0;
		}
		else if (phase) {
			//Check for extra data
			if (dataText.find("Particles:") != -1) {
				lvl >> challenge;
			}
			else if (dataText.find("Bonus:") != -1) {
				lvl >> bonus;
			}
			else {
				//Read platform data
				switch(phase) {
					case 1: {data.x = atoi(dataText.c_str()); break;}
					case 2: {data.y = atoi(dataText.c_str()); break;}
					case 3: {data.w = atoi(dataText.c_str()); break;}
					case 4: {data.h = atoi(dataText.c_str()); break;}
					case 5: {dynamic = dataText; break;}
					case 6: {data.group = atoi(dataText.c_str()); break;}
					case 7: {type = dataText; break;}
					case 8: {
						data.door = ((dataText.find("true") != -1) ? true : false);
						data.dynamic = ((dynamic.find("true") != -1) ? true : false);
						data.color = ((type.find("solid") != -1) ? COLOR_SOLID : COLOR_UNLIT);

						//All data collected, create platform
						parts.push_back(data);

						//Reset phase and look for more platforms
						phase = 0;
						break;
					}
				}
				phase++;
			}
		}
	}

	//Save last puzzle
	puzzle->setParts(parts);
	puzzle->setChallenge(challenge);
	puzzle->setBonus(bonus);
	puzzle->setScale(screenwidth);
	puzzles->push_back(puzzle);
}

void World::spawnPuzzle(int puzzleId) {
	Puzzle *puzzle = puzzles->at(puzzleId);
	this->puzzleId = puzzleId;

	//Store offset
	puzzle->setSpawn(spawnX);
	int doorNum = 0;

	//Spawn all parts
	vector <PartData> parts = puzzle->getParts();
	int size = parts.size();
	for (int i = 0; i < size; i++) {
		PartData part = parts[i];

		if (part.door && !doorNum) {
			//Skip entrance door
			doorNum++;
		}
		else {
			int offX = (-camOffX) + spawnX + part.x;
			int offY = (-camOffY) + part.y;

			//Create bodies to world
			b2Body *body = addRect(offX, offY, part.w, part.h, part.dynamic, part.group);
			platforms->push_back(body);
			platformColors->push_back(part.color);

			if (parts[i].door) {
				if (doorNum == 1) {
					puzzle->setExit(platforms->size() - 1);
				}
				doorNum++;
			}
		}
	}
}

bool World::startPuzzle() {
	bool started = false;
	bool inside = false;
	bool stopped = false;
	Puzzle *puzzle = puzzles->at(puzzleId);

	//Check if already activated
	if (!puzzle->isActivated()) {
		b2Vec2 playerPos = playerBody->GetPosition();
		playerPos *= M2P;

		//Check if body is inside puzzle
		if (puzzle->isPlayerInside(playerPos)) {
			printf("Puzzle number %i, followed by %i walls.\n", puzzlesSolved + 1, numWalls);
			PartData entrance = puzzle->getEntrance();

			//Get data
			int offX = (-camOffX) + puzzle->getSpawn() + entrance.x;
			int offY = (-camOffY) + entrance.y;

			//Close door behind player
			platforms->push_back(addRect(offX, offY, entrance.w, entrance.h, entrance.dynamic, entrance.group));
			platformColors->push_back(entrance.color);

			//Backup player particles
			if (puzzlesSolved) {
				//See if challenge puzzle
				int challenge = puzzle->getChallenge();

				//If challenge puzzle
				if (challenge) {
					storeParticles();

					//Spawn assigned particles to puzzle
					b2Vec2 playerPos = playerBody->GetPosition();
					playerPos *= M2P;
					playerPos.x += camOffX;
					playerPos.y += camOffY;
					spawnGroundParticles(challenge + 1, playerPos, 30);
				}
			}

			inside = true;
		}
	}
	else {
		inside = true;
	}

	//Check if camera is over puzzle
	if (puzzle->cameraAtCenter(b2Vec2((screenwidth / 2), 0))) {
		cameraSpeed = 0;
		stopped = true;
	}

	if (inside && stopped) {
		started = true;
	}

	return started;
}

bool World::endPuzzle() {
	bool ended = false;
	Puzzle *puzzle = puzzles->at(puzzleId);

	if (puzzle->isDone() || puzzle->hasFailed()) {
		//Open exit door
		int exitId = puzzle->deleteExit();
		b2Body *body = platforms->at(exitId);

		//Remove door body
		world->DestroyBody(body);
		platforms->erase(platforms->begin() + exitId);
		platformColors->erase(platformColors->begin() + exitId);

		//Start moving camera again
		newSpeed += CAMERASPEED_INCREASE;
		cameraSpeed = newSpeed;

		//Give back particles stored
		if (puzzlesSolved) {
			retriveParticles();
		}

		ended = true;
	}

	return ended;
}

void World::spawnGroundParticles(int n, b2Vec2 pos, int r) {
	//Helper variables for creating particles
	float degreeStep = 360 / n;
	int posX = pos.x;
	int posY = pos.y;
	int fieldRadius = r;
	float circleRadius = 0.1;
	float pi = 3.14159265;

	//Create particles
	for (float d = 0; d < 360; d += degreeStep) {
		float xTurn = cos(d * pi / 180.0F);
		float yTurn = sin(d * pi / 180.0F);
		int dX = (xTurn * fieldRadius);
		int dY = (yTurn * fieldRadius);
		float roll = randomRange(30, 100) / 100.0;
		dX *= roll;
		dY *= roll;

		int offX = (-camOffX) + posX + dX;
		int offY = (-camOffY) + posY + dY;

		b2Body *newBody = addCircle(offX, offY, circleRadius, -1);
		particles->push_back(new Particle());
		particles->back()->setBody(newBody);
	}
}

void World::setPuzzle(int id) {
	//Spawn puzzle
	spawnPuzzle(id);
	spawnX += 900;

	//Increase difficulty
	numWalls += WALL_INCREASE;

	//Spawn random walls
	for (int i = 0; i < numWalls; i++) {
		spawnRandomWalls();
	}
}

int World::getPuzzlesSolved() {
	return puzzlesSolved;
}



//Player functions
void World::applyForce(int x, int y) {
	//Pop off closest particles
	int couldFire = shootParticle(x, y);

	if (couldFire != -1) {
		//Push particles away from mouse
		int ant = particles->size();
		for (int i = 0; i < ant; i++) {
			Particle *tempParticle = particles->at(i);

			if (!tempParticle->isFired() && !tempParticle->onGround()) {
				//Get handle
				b2Body *tempBody = tempParticle->getBody();
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

		//Get player handle
		b2Vec2 playerPos = playerBody->GetPosition();
		playerPos *= M2P;

		//Calculate direction
		b2Vec2 mouseXY = b2Vec2(x, y);
		b2Vec2 dist = mouseXY - playerPos;

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
		playerBody->ApplyForce(-direction, playerPos, true);
	}
}

void World::joinCircleJoints() {
	//Joint all particles
	int ant = particles->size();
	for (int i = 0; i < ant; i++) {
		Particle *tempParticle = particles->at(i);
		b2Body *tempBody = tempParticle->getBody();

		//Initialize join settings
		b2DistanceJointDef *join = new b2DistanceJointDef();
		join->Initialize(playerBody, tempBody, playerBody->GetPosition(), tempBody->GetPosition());
		join->collideConnected = true;
		join->type = e_ropeJoint;

		//Add joint
		b2Joint *newJoint = world->CreateJoint(join);
		tempParticle->setJoint(newJoint);
	}
}

int World::shootParticle(int x, int y) {
	//Get field position
	b2Vec2 mainXY = playerBody->GetPosition();
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

	int closestParticle = -1;
	int closestTotDist = 0;
	int particlesLeft = 0;

	//Find closest particle
	int ant = particles->size();
	for (int i = 0; i < ant; i++) {
		//Get handle
		Particle *tempParticle = particles->at(i);
		b2Body *tempBody = tempParticle->getBody();
		b2Vec2 tempXY = tempBody->GetPosition();
		tempXY *= M2P;

		//Calculate particle distance
		b2Vec2 parDist = mouseXY - tempXY;
		int parTotDist = ((parDist.x < 0) ? -parDist.x : parDist.x) + ((parDist.y < 0) ? -parDist.y : parDist.y);

		//Make sure not popped
		if (!tempParticle->isFired() && !tempParticle->onGround()) {
			particlesLeft++;

			//If first or closer
			if (closestParticle == -1 || parTotDist < closestTotDist) {
				closestParticle = i;
				closestTotDist = parTotDist;
			}
		}
	}

	//If particle found
	if (closestParticle != -1) {
		//Play sound
		if (snd_Shoot != NULL) {
			Mix_PlayChannel(EFFECTS, snd_Shoot, 0);
		}

		//Pop off
		Particle *tempParticle = particles->at(closestParticle);
		b2Joint *popJoint = tempParticle->fire();
		world->DestroyJoint(popJoint);

		//Apply force
		b2Body *parBody = tempParticle->getBody();
		b2Vec2 parXY = parBody->GetPosition();
		parXY *= M2P;

		parBody->SetLinearVelocity(b2Vec2(0, 0));
		parBody->ApplyForce(direction, parXY, true);

		//printf("Particles left: %i!\n", particlesLeft - 1);
	}
	else {
		//printf("No particles left!\n");
	}

	//Update particles left
	numParticles = ((particlesLeft) ? particlesLeft - 1 : particlesLeft);

	return closestParticle;
}

void World::spawnCharacter() {
	//Helper variables for creating particles
	float degreeStep = 360 / numParticles;
	int posX = 400;
	int posY = 300;
	int fieldRadius = 50;
	float circleRadius = 0.1;
	float pi = 3.14159265;

	//Add central body part for joints
	playerBody = addMainChar(posX, posY, 0.3, true, 1);
	playerColor = b2Vec3(1, 1, 1);

	//Create particles
	for (float d = 0; d < 360; d += degreeStep) {
		float xTurn = cos(d * pi / 180.0F);
		float yTurn = sin(d * pi / 180.0F);
		int dX = (xTurn * fieldRadius);
		int dY = (yTurn * fieldRadius);
		float roll = randomRange(30, 100) / 100.0;
		dX *= roll;
		dY *= roll;

		b2Body *newBody = addCircle(posX + dX, posY + dY, circleRadius, -1);
		particles->push_back(new Particle());
		particles->back()->setBody(newBody);
	}
}

int World::getParticlesLeft() {
	return numParticles;
}

void World::storeParticles() {
	//Clear particles not fired
	int ant = particles->size();
	for (int i = 0; i < ant; i++) {
		Particle *tempParticle = particles->at(i);
		//If not new particle
		if (!(tempParticle->onGround() && !tempParticle->isFired())) {
			//Remove joint
			b2Joint *popJoint = tempParticle->fire();
			if (popJoint != NULL) {
				world->DestroyJoint(popJoint);
			}

			//Remove body
			b2Body *popBody = tempParticle->getBody();
			world->DestroyBody(popBody);

			//Remove particle
			particles->erase(particles->begin() + i--);
			ant--;
			delete tempParticle;
		}
	}

	//Move particles to storage
	numStored = numParticles;
	numParticles = 0;

	//Spawn allocated number of particles

}

void World::retriveParticles() {
	Puzzle *puzzle = puzzles->at(puzzleId);
	int bonusParticles = puzzle->getBonus();
	int totalParticles = numStored + bonusParticles;

	if (totalParticles) {
		//Spawn stored particles
		b2Vec2 playerPos = playerBody->GetPosition();
		playerPos *= M2P;
		playerPos.x += camOffX;
		playerPos.y += camOffY;
		spawnGroundParticles(totalParticles, playerPos, 30);

		//Regained stored particles
		numStored = 0;
	}
}

bool World::gameOver() {
	return lost;
}

int World::getScore() {
	return score;
}



//Random wall functions
void World::cleanWalls() {
	//Go through all walls and clean everything to far behind ball
	b2Vec2 fieldPos = playerBody->GetPosition();
	float fieldX = fieldPos.x * M2P;

	//Joint all particles
	int ant = platforms->size();
	for (int i = 0; i < ant; i++) {
		b2Body *tempBody = platforms->at(i);
		b2Vec2 tempPlatform = tempBody->GetPosition();
		tempPlatform *= M2P;
		float wallX = tempPlatform.x + WALLSIZE;

		if (wallX < 0) {
			b2Vec3 color = platformColors->at(i);

			//Update puzzle exitId
			puzzles->at(puzzleId)->updateExitId(i);

			//Remove body
			platforms->erase(platforms->begin() + i);
			platformColors->erase(platformColors->begin() + i);
			world->DestroyBody(tempBody);
			i--;
			ant--;
		}
	}
}

void World::cleanParticles() {
	int ant = particles->size();
	for (int i = 0; i < ant; i++) {
		Particle *tempParticle = particles->at(i);
		b2Vec2 tempPos = tempParticle->getBody()->GetPosition();
		tempPos *= M2P;

		//If not connected to mainbody
		if (tempParticle->isFired() || tempParticle->onGround()) {
			//If touching backborder
			if (tempPos.x < DEATH_WALL_SPACE + DEATHWALL_SIZE) {
				//Remove body
				b2Body *popBody = tempParticle->getBody();
				world->DestroyBody(popBody);

				//Remove particle
				particles->erase(particles->begin() + i);
				ant--;
				delete tempParticle;
			}
		}
	}
}

void World::spawnRandomWalls() {
	int numWalls = randomRange(1, 3);
	int maxWallSize = FLOOR / (numWalls + 1);
	bool posY[3] = {false, false, false};

	for (int i = 0; i < numWalls; i++) {
		//choose unpicked random position
		int newPosY = -1;
		while (newPosY == -1) {
			int pos = randomRange(0, 2);
			if (!posY[pos]) {
				newPosY = pos;
				posY[pos] = true;
			}
		}

		//Choose random length
		int h = randomRange(WALLSIZE, maxWallSize);

		//Find chosen length
		int y;
		switch (newPosY) {
			case 0: {y = ROOF + (h / 2); break;}
			case 1: {y = (FLOOR / 2); break;}
			case 2: {y = FLOOR - (h / 2); break;}
		}

		int offX = (-camOffX) + spawnX;
		int offY = (-camOffY) + y;

		//All data collected, create platform
		platforms->push_back(addRect(offX, offY, WALLSIZE, h, false, 1));
		platformColors->push_back(COLOR_SOLID);
	}

	//printf("New x%i (SpawnX: %i)\n", numWalls, spawnX);
	spawnX += WALL_DISTANCE;
}