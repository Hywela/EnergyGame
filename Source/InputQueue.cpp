#include "InputQueue.h"



InputQueue::InputQueue() {

}

InputQueue::~InputQueue() {

}

void InputQueue::push(InputData item) {
	//Add item at the back
	lock_guard<mutex> locker(accessMutex);
	items.push(item);
}

//Remove item from the stack
//Return value to let the user know if an item was poped of the stack
InputData InputQueue::pop() {
	InputData itemPoped; 
	lock_guard<mutex> locker(accessMutex);

	//Check to see if there is items left on the stack
	if (items.size() > 0) {
		itemPoped = items.front();
		items.pop();
	}

	//return true if an item was poped off or false if the stack was already empty
	return itemPoped; 	
}

//return the current size of the stack
int InputQueue::getSize() {
	lock_guard<mutex> locker(accessMutex);
	int size = items.size();
	return size;
}

//Clear the queue
void InputQueue::clear() {
	lock_guard<mutex> locker(accessMutex);

	//Make empty queue
	queue<InputData> empty;

	//Swap current the cuurently used stack with the new empty one
	swap(items, empty);
}