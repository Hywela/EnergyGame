#include "Window.h"


Window::Window()
{
	SetupSDL();
	SetupOGL();
	
}

Window::~Window()
{
}

void Window::RenderObject(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

	glViewport(0, 0, screenwidth, screenheight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat) (screenwidth / screenheight), 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-1, -1, -5);
	

	glDisableClientState(GL_COLOR_ARRAY);
	glEnable(GL_DITHER);
	SDL_GL_SwapWindow(window);

}
void Window::Render(int x, int y, float yAngle, float xAngle){


	float const win_aspect = (float) screenwidth / (float) screenheight;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_LIGHTING);

	
	glShadeModel(GL_SMOOTH);
	
		
	SolidSphere *spher;
	spher = new SolidSphere(1, 24, 48);
	gluPerspective(20.0f, (GLfloat) (screenwidth / screenheight), 0.1f, 100.0f);
	gluLookAt( 0, 0, 0, /* look from camera XYZ */ 10,10, 10, /* look at the origin */ 0, 1, 0); /* positive Y up vector */
	std::cout << "x " << x;
	glTranslatef(0,0, 0);
	glRotatef(yAngle, 0.0f, 0.0f, 0.0f);

	spher->draw(10, 10, 10, yAngle);
	//
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	

	SDL_GL_SwapWindow(window);
	delete spher;
}
void Window::SetupSDL(){
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "Couldnt init SDL2! SDL_Error: " << SDL_GetError() << std::endl;
	}

	window = SDL_CreateWindow("First SDL2 OGL App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenwidth, screenheight, flags);
	context = SDL_GL_CreateContext(window);

}
void Window::SetupOGL(){
	// Show some information about the OpenGL verion and graphics card (for debugging)
	std::cout << ::glGetString(GL_VENDOR) << std::endl;
	std::cout << ::glGetString(GL_RENDERER) << std::endl;
	std::cout << ::glGetString(GL_VERSION) << std::endl;

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
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
	}

}
