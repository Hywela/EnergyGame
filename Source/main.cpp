#include "Window.h"
#include "Input.h"
#include <thread>



int main(int argc, char *argv[]) {
	Window *mainWindow = new Window(); //new Window(1366, 786);



	mainWindow->mainLoop();
	return 0;
}