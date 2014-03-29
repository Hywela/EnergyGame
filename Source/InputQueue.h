#pragma once
#include <queue>
#include <mutex>
#include "InputData.h"
using namespace std;



class InputQueue {
private:
	queue <InputData> items;
	mutex accessMutex;
	
public:
	InputQueue();
	~InputQueue();
	void push(InputData item);
	InputData pop();
	int getSize();
	void clear();
};
