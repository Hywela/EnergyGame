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
}

Puzzle::~Puzzle() {

}

vector <PartData> Puzzle::getParts() {
	return parts;
}

bool Puzzle::isPlayerInside(b2Vec2 position) {
	bool found = false;

	if (position.x >= spawnX + (700 / scale)) {
		found = true;
		activated = true;
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

	if (position.x >= spawnX + (900 / scale)) {
		found = true;
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
	return false;
}

void Puzzle::setScale(int screenW) {
	scale = 1920.0 / screenW;
}