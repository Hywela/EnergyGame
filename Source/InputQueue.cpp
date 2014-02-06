#include "InputQueue.h"
using namespace std;

InputQueue::InputQueue(){
}


InputQueue::~InputQueue(){
}

void InputQueue::push(InputData item){
	//Add item at the back
	lock_guard<mutex> locker(accessMutex);
	items.push(item);
}

InputData InputQueue::pop(){
	//Remove item from the stack
	InputData itemPoped; 	//Return value to let the user know if an item was poped of the stack
	lock_guard<mutex> locker(accessMutex);
	if (items.size() > 0){ 		//Check to see if there is items left on the stack
		itemPoped = items.front();
		items.pop();
	}
	return itemPoped; 	//return true if an item was poped off or false if the stack was already empty
}

//return the current size of the stack
int InputQueue::getSize(){
	lock_guard<mutex> locker(accessMutex);
	int size = items.size();
	return size;
}

//Clear the queue
void InputQueue::clear(){
	lock_guard<mutex> locker(accessMutex);
	queue<InputData> empty;  		//Make empty queue
	swap(items, empty);		//Swap current the cuurently used stack with the new empty one
}