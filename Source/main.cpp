#include "Window.h"
#include <thread>



int main(int argc, char *argv[]) {


	Window *mainWindow = new Window(); //new Window(1366, 786);

	//thread *inputThread;
	//inputThread = new thread(&Input::inputLoop, Input(mainWindow));

	mainWindow->mainLoop();
	delete mainWindow;
	return 0;
}