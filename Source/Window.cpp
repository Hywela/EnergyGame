#include "Window.h"


Window::Window(int w, int h) {
	inQueue = new InputQueue();
	renderQueue = new RenderQue();
	screenheight = h;
	screenwidth = w;
	minWidth = w;
	minHeight = h;

	flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;// | SDL_WINDOW_FULLSCREEN;
	running = true;
	isFullscreen = false;

	ren = new Render(screenheight, screenwidth, inQueue, renderQueue);

	//SetupOGL();
	//render = new thread(&Render::mainLoop, new Render(screenheight, screenwidth, inQueue ,renderQueue));

	setupWorld();
}

Window::~Window() {

}

void Window::mainLoop() {	
	while (running) {
		timer = SDL_GetTicks();
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
					InputData click;
					click.mouseClick(e.button.x, e.button.y);
					inQueue->push(click);
					break;
				}
			}
		}
	
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
}

void Window::setupWorld() {
	world = new World(screenwidth, screenheight, inQueue, renderQueue);
	worldSimulation = new thread(&World::checkForInput, world);
	//world->setupWorld();
}
