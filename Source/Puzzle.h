#pragma once
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
	int camOffX;
	int tasksTotal, tasksDone;
	vector <PartData> parts;
	int entranceId, exitId;
	bool activated;
	int bonusParticles;
	int challengeParticles;
	int time, timeChallenge;
	int par, parX, parY;
	Uint32 ticksLastSec;

public:
	Puzzle();
	~Puzzle();

	vector <PartData> getParts();
	bool isPlayerInside(b2Vec2 position);
	void setSpawn(float spawnX, int camOffX);
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
	void setTime(int t);
	int getTimeLeft();
	void progressUpdate();
	void setParticleSpawn(int p, int x, int y);
	void getParticleSpawn(int &p, int &x, int &y);
};

