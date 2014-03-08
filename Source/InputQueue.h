#pragma once
#include <queue>
#include <mutex>
#include "InputData.h"
using namespace std;



class InputQueue {
private:
	queue <int> items;
	mutex accessMutex;

public:
	InputQueue();
	~InputQueue();
	void push(int item);
	int pop();
	int getSize();
	void clear();
};
