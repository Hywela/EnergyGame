#include "Puzzle.h"



Puzzle::Puzzle() {
	entranceId = -1;
	exitId = -1;
	spawnX = 0;
	tasksTotal = 0;
	tasksDone = 0;
	activated = true;
}

Puzzle::~Puzzle() {

}

vector <PartData> Puzzle::getParts() {
	return parts;
}

bool Puzzle::isPlayerInside(b2Vec2 position) {
	bool found = false;

	if (position.x >= spawnX + 700) {
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

void Puzzle::shiftOrginX(int add) {
	spawnX += add;
}

void Puzzle::updateExitId(int deletedId) {
	if (deletedId < exitId) {
		exitId--;
	}
}

bool Puzzle::cameraAtCenter(b2Vec2 position) {
	bool found = false;

	if (position.x >= spawnX + 900) {
		found = true;
	}

	return found;
}