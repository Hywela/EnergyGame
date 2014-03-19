#include "Window.h"



Window::Window(int w, int h) {
	inQueue = new InputQueue();
	renderQueue = new RenderQue();
	screenheight = h;
	screenwidth = w;
	minWidth = w;
	minHeight = h;
	//getf = &Window::menueLoop;
	inGame = false;
	flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;// | SDL_WINDOW_FULLSCREEN;
	running = true;
	isFullscreen = false;
	leftMouseClick = &Window::menueLeftMouseClick;
	loopType = &Window::menueLoop;
	
	ren = new Render(screenheight, screenwidth, inQueue, renderQueue);

	//SetupOGL();
	//render = new thread(&Render::mainLoop, new Render(screenheight, screenwidth, inQueue ,renderQueue));
	
	buildMenue();

}

Window::~Window() {

}

void Window::mainLoop() {	
	

	while (running) {
		timer = SDL_GetTicks();
		checkForMouseInput();

	
		int fps = (1000 / 30) - (timer - SDL_GetTicks());
		(this->*loopType)();
		
		SDL_Delay(fps);
		timer = SDL_GetTicks();
	}
}

void Window::checkForMouseInput(){

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_KEYDOWN: {
				if (e.key.keysym.sym == SDLK_q) {
					running = false;
				}
				else if (e.key.keysym.sym == SDLK_TAB) {
								  //ToggleFullscreen();
				}
			break;
			}
			case SDL_MOUSEBUTTONDOWN: {
				(this->*leftMouseClick)();
			break;
			}
		}
	}
}

void Window::gameLoop() {
	int fps = (1000 / 30) - (timer - SDL_GetTicks());
	int puzzlesSolved = 0;
	int particlesLeft = 0;

	string fpsStr = "";
	string puzStr = "";
	string parStr = "";

	if (world) {
		fpsStr = "FPS: " + to_string(fps);
		puzStr = "Solved: " + to_string(world->getPuzzlesSolved());
		parStr = "Particles: " + to_string(world->getParticlesLeft());
	}
	InputData step(1);
	//cout <<"\n fps: " <<fps;
	inQueue->push(step); //world->checkForInput();

	InputData updateWorld(2);
	inQueue->push(updateWorld);
	ren->mainLoop(fpsStr, puzStr, parStr);
	SDL_Delay(fps);
	timer = SDL_GetTicks();
}
void Window::gameLeftMouseClick() {
	InputData click;
	click.mouseClick(e.button.x, e.button.y);
	inQueue->push(click);
}
void Window::menueLeftMouseClick() {
	switch (ren->menueMouseHoverCheck(e.button.x, e.button.y)) {
	case 1: {
				cout << "play clicked";
				leftMouseClick = &Window::gameLeftMouseClick;
				loopType = &Window::gameLoop;
				startWorld();
	break;
	}
	case 2: {
				cout << "2";
	break;
	}
	default:{
	break;
		}
	}
}
void Window::menueLoop(){
	ren->mainMenue();
}
void Window::startWorld() {
	world = new World(screenwidth, screenheight, inQueue, renderQueue);
	worldSimulation = new thread(&World::checkForInput, world);
	//world->setupWorld();
}
void Window::buildMenue(){
	ren->pushBackMenueObj(50, 50, "Play");
	ren->pushBackMenueObj(50, 100, "Settings");
	ren->pushBackMenueObj(50, 150, "Quit");
}

