#include "Init.h"

Init::Init( Uint32 flgs) : screenWidth(0), 
						   screenHeight(0),
						   flags(flgs)
{};

void Init::OpenGL()
{
	// Show some information about the OpenGL verion and graphics card (for debugging)
	std::cout << ::glGetString(GL_VENDOR) << "\n";
	std::cout << ::glGetString(GL_RENDERER) << "\n";
	std::cout << ::glGetString(GL_VERSION) << "\n";
	glViewport(0, 0, screenWidth, screenHeight);
	gluPerspective(0.0f, (GLfloat) (screenWidth / screenHeight), 5.0f, 100);
	//Initialize clear color
	glClearColor(0.f, 0.f, 0.f, 1.f);

	//Initialize the depth buffer stuff
	glClearDepth(1.0f);                             // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Test To Do

	//glShadeModel(GL_SMOOTH);                        // Enable Smooth Shading
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

	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		std::cout << "Error happened when starting Glew." << std::endl;
	}
	if (glewIsSupported("GL_VERSION_3_3"))
		printf("Ready for OpenGL 3.3\n");
	else {
		printf("OpenGL 3.3 not supported\n");
		exit(1);
	}
};

void Init::SDL()
{
	SDL_Rect screenSize = SDL_Rect();
	
	-SDL_GetDisplayBounds(0, &screenSize);
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "Couldnt init SDL2! SDL_Error: " << SDL_GetError() << std::endl;
	}

	SDL_DisplayMode mode[6];
	for (int i = 0; i < SDL_GetNumVideoDisplays() &&
		i < 6; ++i) {
		SDL_GetCurrentDisplayMode(i, &mode[i]);
	}
	screenHeight = mode[0].h/2;
	screenWidth = mode[0].w/2;
	if (screenHeight <= 0){
		screenHeight = 800;
		screenWidth = 600;
	}

	window		= SDL_CreateWindow("Energy Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, flags);
	glContext	= SDL_GL_CreateContext(window);
	
};
int Init::getScreenHeight(){
	return screenHeight;
}
int Init::getScreenWidth(){
	return screenWidth;
}
int Init::CleanUp()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
};