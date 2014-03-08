#pragma once



class InputData {
private:
	int clickType;
	int clickX;
	int clickY;
public:
	InputData();
	InputData(int type);
	InputData(int type, int x, int y);
	void mouseClick(int x, int y);
	int getType();
	int getX();
	int getY();
	~InputData();
};

