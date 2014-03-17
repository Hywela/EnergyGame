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
	int spawnX;
	int tasksTotal, tasksDone;
	vector <PartData> parts;
	int entranceId, exitId;
	bool activated;

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
	void shiftOrginX(int add);
	void updateExitId(int deletedId);
	bool cameraAtCenter(b2Vec2 position);
};

