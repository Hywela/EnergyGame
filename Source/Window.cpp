#include "Window.h"



Window::Window(int w, int h) {
	screenheight = h;
	screenwidth = w;

	flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	running = true;

	SetupSDL();
	SetupOGL();
	SetupWorld();
	timer = SDL_GetTicks();
}

Window::~Window() {

}
void Window:: threading(){

	
}
void Window::mainLoop() {
	while (running) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_MOUSEBUTTONDOWN: {
					world->applyForce(e.button.x, e.button.y);
					break;
				}
			}
		}

		int fps = (1000 / 30) - (timer - SDL_GetTicks());

		world->step();  //update  dt:Number, velocityIterations:int, positionIterations:in // steps true the world
		Render();

		SDL_Delay(fps);
		timer = SDL_GetTicks();
	}
}

void Window::SetupSDL() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Couldnt init SDL2! SDL_Error: " << SDL_GetError() << endl;
	}

	window = SDL_CreateWindow("First SDL2 OGL App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenwidth, screenheight, flags);
	context = SDL_GL_CreateContext(window);
}

void Window::SetupOGL() {
	// Show some information about the OpenGL verion and graphics card (for debugging)
	cout << ::glGetString(GL_VENDOR) << endl;
	cout << ::glGetString(GL_RENDERER) << endl;
	cout << ::glGetString(GL_VERSION) << endl;

	//Initialize clear color
	glClearColor(0.f, 0.f, 0.f, 1.f);

	//Initialize the depth buffer stuff
	glClearDepth(1.0f);                             // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Test To Do

	glShadeModel(GL_SMOOTH);                        // Enable Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective 

	//Initialize the lights
	GLfloat LightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };  // Ambient Light Values
	GLfloat LightDiffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };  // Diffuse Light Values
	GLfloat LightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Light Position
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);      // Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);      // Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);    // Position The Light
	glEnable(GL_LIGHT1);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	//Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
	}
}

void Window::Render() {
	//Clear buffer
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

	//Redraw
	RenderOrtho();

	//Present buffer
	SDL_GL_SwapWindow(window);
}

void Window::RenderOrtho() {
	// Prepare for ortho rendering:
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screenwidth, 0, screenheight);
	glScalef(1, -1, 1);
	glTranslatef(0, -screenheight, 0);
	glMatrixMode(GL_MODELVIEW);

	//Draw player
	world->updateWorld();

	// Disable GUI rendering:
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
}

void Window::CheckKeyEvent(SDL_Event e) {
	switch (e.key.keysym.sym) {
		case SDLK_ESCAPE: {
			running = false;
			break;
		}
	}
}

void Window::SetupWorld() {
	world = new World(screenwidth, screenheight);
	//worldSimulation = new thread(&World::setupWorld, world->setupWorld());

	world->setupWorld();
}

World* Window::GetWorld() {
	return world;
}
