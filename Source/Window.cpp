#include "Window.h"



Window::Window() {
	inQueue = new InputQueue();
	renderQueue = new RenderQue();
	running = true;
	isFullscreen = false;
	leftMouseClick = &Window::menuLeftMouseClick;
	loopType = &Window::menuLoop;
	 int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;// | SDL_WINDOW_FULLSCREEN;
	ren = new Render(new Init( flags), inQueue, renderQueue);

	//SetupOGL();
	//render = new thread(&Render::mainLoop, new Render(screenheight, screenwidth, inQueue ,renderQueue));
	
	buildMenu();

	//fps test variables
	fps_lasttime = SDL_GetTicks(); //the last recorded time.
	fps_current = 0; //the current FPS.
	fps_frames = 0; //frames passed since the last recorded fps.

}

Window::~Window() {

}

void Window::mainLoop() {	

	while (running) {
	
		checkForMouseInput();

		(this->*loopType)();

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
				if (e.key.keysym.sym == SDLK_t) {
					loopType = &Window::menuLoop;
					leftMouseClick = &Window::menuLeftMouseClick;
					delete world;

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
	int fps = (1000 / 30) - (timer - SDL_GetTicks()) -20;
	int puzzlesSolved = 0;
	int particlesLeft = 0;

	string fpsStr = "";
	string puzStr = "";
	string parStr = "";

	if (world) {
		fpsStr = "FPS: " + to_string(fps_current);
		puzStr = "Solved: " + to_string(world->getPuzzlesSolved());
		parStr = "Particles: " + to_string(world->getParticlesLeft());
	}

	world->step();

	ren->render();
	ren->startRendering();
	world->updateWorld();
	ren->mainLoop(fpsStr, puzStr, parStr);

	SDL_Delay(fps);
	//Fps test start
	fps_frames++;
	if (fps_lasttime < SDL_GetTicks() - 1.0 * 1000)
	{
		fps_lasttime = SDL_GetTicks();
		fps_current = fps_frames;
		fps_frames = 0;
	}
	//Fps test end

}
void Window::gameLeftMouseClick() {
	//InputData click;
	//click.mouseClick(e.button.x, e.button.y);
	//inQueue->push(click);
	world->applyForce(e.button.x, e.button.y);
}
void Window::menuLeftMouseClick() {
	switch (ren->menuMouseClickCheck(e.button.x, e.button.y)) {
	case 1: {
			//	cout << "play clicked";
				//ren->zerOutCamera();
				leftMouseClick = &Window::gameLeftMouseClick;
				startWorld();
				loopType = &Window::gameLoop;
	break;
	}
	case 2: {
				cout << "2";
			
	break;
	}
	case 3: {
				cout << "3";
				
				break;
	}
	default:{
	break;
		}
	}
}
void Window::menuLoop(){
	string fpsStr = "FPS: " + to_string(fps_current);

	int fps = (1000 / 30) - (timer - SDL_GetTicks());
	//(this->*loopType)();
	ren->mainMenu(fpsStr);
	//	SDL_Delay(fps);
	timer = SDL_GetTicks();

	fps_frames++;
	if (fps_lasttime < SDL_GetTicks() - 1.0 * 1000)
	{
		fps_lasttime = SDL_GetTicks();
		fps_current = fps_frames;
		fps_frames = 0;
	}
}
void Window::startWorld() {
	world = new World(ren->getInit()->getScreenWidth(), ren->getInit()->getScreenHeight(), ren->getPlatformVBO(), ren->getParticleVBO()
		, ren->getMainCharParticleVBO());
	//worldSimulation = new thread(&World::checkForInput, world);
	//world->setupWorld();
}
void Window::buildMenu(){
	int fontSizeOffsett = 250;
	int screenW = ren->getInit()->getScreenWidth()/3;
	int screenH = ren->getInit()->getScreenHeight() / 20 + fontSizeOffsett;
	ren->pushBackMenuObj(screenW, screenH*0.5, "Play");
	ren->pushBackMenuObj(screenW, screenH*1, "Settings");
	ren->pushBackMenuObj(screenW, screenH*1.5, "Quit");
	
}

