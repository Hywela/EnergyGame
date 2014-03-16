#pragma once
#include <queue>
#include <mutex>
#include "RenderData.h"

using namespace std;



class RenderQue {
private:
	queue <RenderData> items;
	mutex accessMutex;

public:
	RenderQue();
	~RenderQue();
	void push(RenderData item);
	RenderData pop();
	int getSize();
	void clear();
};
