#pragma once
#include "Platform.h"
#include "Consts.h"
using namespace std;



struct PartData {
	int x, y, w, h, group;
	bool dynamic, door;
	b2Vec3 color;
};

class Puzzle {
private:
	float spawnX;
	int tasksTotal, tasksDone;
	vector <PartData> parts;
	int entranceId, exitId;
	bool activated;
	int bonusParticles;
	int challengeParticles;
	float scale;

public:
	Puzzle();
	~Puzzle();

	vector <PartData> getParts();
	bool isPlayerInside(b2Vec2 position);
	void setSpawn(int offX);
	void setExit(int id);
	PartData getEntrance();
	int getSpawn();
	void setParts(vector <PartData> parts);
	bool isDone();
	int deleteExit();
	void taskDone();
	bool isActivated();
	void shiftOrginX(float add);
	void updateExitId(int deletedId);
	bool cameraAtCenter(b2Vec2 position);
	void setChallenge(int particles);
	void setBonus(int particles);
	int getChallenge();
	int getBonus();
	bool hasFailed();
	void setScale(int screenW);
};

