#include "Puzzle.h"



Puzzle::Puzzle() {
	entranceId = -1;
	exitId = -1;
	spawnX = 0;
	tasksTotal = 0;
	tasksDone = 0;
	activated = true;
	bonusParticles = 0;
	challengeParticles = 0;
	scale = 1;
	time = -1;
	timeChallenge = -1;
	par = 0;
	parX = 0;
	parY = 0;
}

Puzzle::~Puzzle() {

}

vector <PartData> Puzzle::getParts() {
	return parts;
}

bool Puzzle::isPlayerInside(b2Vec2 position) {
	bool found = false;

	if (scale == 1) {
		if (position.x >= spawnX + (600 / scale)) {
			found = true;
			activated = true;
			ticksLastSec = SDL_GetTicks();

			//If time challenge
			if (time < 0 && timeChallenge > 0) {
				//Start timer
				time = 0;
			}
		}
	}
	else {
		if (position.x >= spawnX + (450 / scale)) {
			found = true;
			activated = true;
			ticksLastSec = SDL_GetTicks();

			//If time challenge
			if (time < 0 && timeChallenge > 0) {
				//Start timer
				time = 0;
			}
		}
	}

	return found;
}

void Puzzle::setSpawn(int offX) {
	spawnX = offX;
	activated = false;
}

void Puzzle::setExit(int id) {
	exitId = id;
}

PartData Puzzle::getEntrance() {
	return parts[entranceId];
}

int Puzzle::getSpawn() {
	return spawnX;
}

void Puzzle::setParts(vector <PartData> parts) {
	this->parts = parts;

	//Look for special parts
	for (int i = 0; i < parts.size(); i++) {
		if (parts[i].door && entranceId == -1) {
			entranceId = i;
		}

		b2Vec3 c = parts[i].color;
		if (c.x == COLOR_UNLIT.x && c.y == COLOR_UNLIT.y && c.z == COLOR_UNLIT.z) {
			tasksTotal++;
		}
	}
}

bool Puzzle::isDone() {
	bool done = false;

	if (tasksTotal) {
		if (tasksDone == tasksTotal) {
			done = true;

			//Reset task
			tasksDone = 0;
			time = -1;
		}
	}

	return done;
}

int Puzzle::deleteExit() {
	int id = exitId;

	//Reset data
	exitId = -1;
	spawnX = 0;

	return id;
}

void Puzzle::taskDone() {
	tasksDone++;
}

bool Puzzle::isActivated() {
	return activated;
}

void Puzzle::shiftOrginX(float add) {
	spawnX += add;
}

void Puzzle::updateExitId(int deletedId) {
	if (deletedId < exitId) {
		exitId--;
	}
}

bool Puzzle::cameraAtCenter(b2Vec2 position) {
	bool found = false;

	if (scale == 1) {
		if (position.x >= spawnX + (1410 / scale)) {
			found = true;
		}
	}
	else {
		if (position.x >= spawnX + (1200 / scale)) {
			found = true;
		}
	}

	return found;
}

void Puzzle::setChallenge(int particles) {
	challengeParticles = particles;
}

void Puzzle::setBonus(int particles) {
	bonusParticles = particles;
}

int Puzzle::getChallenge() {
	return challengeParticles;
}

int Puzzle::getBonus() {
	int bonus = 0;

	if (!hasFailed()) {
		bonus = bonusParticles;
	}

	return bonus;
}

bool Puzzle::hasFailed() {
	bool fail = false;

	//If timechallenge and out of time
	if ((timeChallenge > 0 && time >= timeChallenge) || time == -2) {
		//Reset task
		tasksDone = 0;
		time = -2;

		fail = true;
	}

	return fail;
}

void Puzzle::setScale(int screenW) {
	scale = 1920.0 / screenW;
	this->screenW = screenW;
}

void Puzzle::setTime(int t) {
	timeChallenge = t;
}

int Puzzle::getTimeLeft() {
	int t = -1;

	if (time >= 0) {
		t = (timeChallenge - time);
		if (!time) {
			t -= 1;	
		}
	}

	return t;
}

void Puzzle::progressUpdate() {
	//Updates puzzle while player is solving it

	//If started time puzzle
	Uint32 ticks = SDL_GetTicks();
	if (time >= 0 && time < timeChallenge && ticks >= ticksLastSec + 1000) {
		//Countdown time left
		time++;
		ticksLastSec = ticks;
	}
}

void Puzzle::setParticleSpawn(int p, int x, int y) {
	par = p;
	parX = x;
	parY = y;
}

void Puzzle::getParticleSpawn(int &p, int &x, int &y) {
	p = par;
	x = parX;
	y = parY;
}