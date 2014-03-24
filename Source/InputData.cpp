#include "InputData.h"


InputData::InputData() {
}

InputData::InputData(int type) {
	clickType = type;
}

InputData::InputData(int type, int x, int y) {
	clickType = type;
	clickX = x;
	clickY = y;
}

InputData::~InputData() {

}

void InputData::mouseClick(int x, int y){
	clickType = 0;
	clickX = x;
	clickY = y;
}

int InputData::getType(){
	return clickType;
}



int InputData::getX(){
	return clickX;
}

int InputData::getY(){
	return clickY;
}