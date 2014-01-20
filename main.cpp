#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <SDL_image.h>
//tull og fanteri test
//Note this code is currently just placeholder code.

// ----------------------------------  D E C L A R A C T I O N S   ------------------------------------


// HELPER VARIABLE DECLARATIONS
int screenheight = 600;
int screenwidth = 800;
int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
bool running = true;
bool mouseclick = false;
int timer;

// For slider
int sliderPosx = 0;
int sliderPosy = 0;
int sliderWidth = 25;
int sliderHeight = 50;
int selectVariable = 0;
float cube1Rot = 0.0f;
float cube2Rot = 0.0f;
float cube1RotAmount = 0.0f;
float cube2RotAmount = 0.0f;

// FUNCTION DECLARATIONS
void SetupSDL();
void SetupOGL();
void Render();
void RenderScene();
void RenderGUI();
void CheckKeyEvent(SDL_Event e);
void CheckMouseButtonEvent(SDL_Event e);
void MoveSlider(int x, int y);
bool PressedSlider(int x, int y);
void HandleSliderMoved(SDL_Event e);
void RenderPrep();
void RenderCube();


// GLOBAL OBJECTS
SDL_Window* window;
SDL_GLContext context;
SDL_Event e;


// Object data:
GLubyte pixel[] = { 0, 0, 0, 1 };

// Color picker data:

// Red cube
GLfloat cc1[] = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
// Green Cube
GLfloat cc2[] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
//slider:
static GLfloat sliderverts[] = { 0.0, 0.0, 0.0,
0.0, sliderHeight, 0.0,
sliderWidth, sliderHeight, 0.0,
sliderWidth, 0.0, 0.0 };

static GLfloat slidercolors[] = { 1.0f, 1.0f, 1.0f,  //red
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,  //green
1.0f, 1.0f, 1.0f };

// Cube:

static GLfloat cubevertices[] = {
	-0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5,  //v0, v1, v2, v3   front
	0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5,  //v1, v5, v6, v2   right
	-0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5,  //v3, v2, v6, v7   top
	-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5,  //v0, v3, v7, v4   left
	-0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5,  //v0, v1, v5, v4   bottom
	-0.5, -0.5, -0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5   //v4, v5, v6, v7   back
};
//All the vertices on the same quad share the same normal - to give us flat shading
static GLfloat cubenormals[] = {
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,    //front
	1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,    //right
	0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    //top
	-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,    //left
	0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,   //bottom
	0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f    //back
};

int main(int argc, char *argv[])
{

	SetupSDL();
	SetupOGL();
	timer = SDL_GetTicks();
	while (running)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_KEYDOWN:
			{
								CheckKeyEvent(e);
								break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{

										RenderPrep();
										mouseclick = PressedSlider(e.button.x, e.button.y);
										std::cout << "Pressed button" << std::endl;
										glReadPixels(e.button.x, screenheight - e.button.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

										if (pixel[0] == 255)
										{
											selectVariable = 1;
										}
										else if (pixel[1] == 255)
										{
											selectVariable = 2;
										}
										break;
			}
			case SDL_MOUSEBUTTONUP:
			{
									  mouseclick = false;
									  std::cout << "Released button" << std::endl;
									  break;
			}
			default:
			{
					   break;
			}
			}
		}

		if (mouseclick == true)
		{
			MoveSlider(e.button.x, e.button.y);
			HandleSliderMoved(e);
		}
		Render();
		cube1Rot += cube1RotAmount;
		cube2Rot += cube2RotAmount;
		SDL_Delay((1000 / 30) - (timer - SDL_GetTicks()));
		timer = SDL_GetTicks();
	}
	return 0;
}


// ------------------------------------   D E F I N I T I O N S   ------------------------------------







// ----------------------------------   P R E P A R A T I O N S   ---------------------------------------
void SetupSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "Couldnt init SDL2! SDL_Error: " << SDL_GetError() << std::endl;
	}

	window = SDL_CreateWindow("First SDL2 OGL App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenwidth, screenheight, flags);
	context = SDL_GL_CreateContext(window);
	
}

void SetupOGL()
{
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








// ---------------------------------   R E N D E R   --------------------------------------
void RenderPrep()
{
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

	glDisable(GL_DITHER);
	glTranslatef(1, 0, 0);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, cc1);
	glRotatef(cube1Rot, 0, 1, 0);
	RenderCube();
	glDisableClientState(GL_COLOR_ARRAY);

	glTranslatef(2, 0, 0);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, cc2);
	glRotatef(cube2Rot, 0, 1, 0);
	RenderCube();

	glDisableClientState(GL_COLOR_ARRAY);
	glEnable(GL_DITHER);

	//SDL_GL_SwapWindow(window);

}
void Render()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

	RenderGUI();
	RenderScene();


	SDL_GL_SwapWindow(window);

}

void RenderScene()
{
	glViewport(0, 0, screenwidth, screenheight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat) (screenwidth / screenheight), 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Position the light in world space
	static GLfloat LightPosition[] = { 0.0f, 0.0f, 4.0f, 1.0f }; // Light Position
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);    // Position The Light
	glEnable(GL_LIGHTING);      // Turn on Lighting

	glTranslatef(-1, -1, -5);

	glTranslatef(1, 0, 0);
	glRotatef(cube1Rot, 0, 1, 0);
	RenderCube();

	glTranslatef(2, 0, 0);
	glRotatef(cube2Rot, 0, 1, 0);
	RenderCube();


	glDisable(GL_LIGHTING);

}

void RenderGUI()
{
	// Prepare for GUI rendering:
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screenwidth, 0, screenheight);
	glScalef(1, -1, 1);		// Reverse Y axis
	glTranslatef(0, -screenheight, 0);
	glMatrixMode(GL_MODELVIEW);

	glTranslatef(sliderPosx, sliderPosy, 0);
	// Draw the GUI

	//Tell OpenGL about our vertex and colour data
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, sliderverts);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, slidercolors);

	//draw quad
	glDrawArrays(GL_QUADS, 0, 4);

	//restore the state GL back
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// Disable GUI rendering:
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);

	// Disable GUI rendering:
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);

}


void RenderCube()
{

	//Tell OpenGL about our vertex and normal data
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, cubevertices);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, cubenormals);

	//draw the Cube
	glDrawArrays(GL_QUADS, 0, 24);

	//restore the state GL back
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void HandleSliderMoved(SDL_Event e)
{
	// Do what you want to do in the scene
	if (selectVariable == 1)
	{
		cube1RotAmount = e.button.x / 100;
	}
	else if (selectVariable == 2)
	{
		cube2RotAmount = e.button.x / 100;

	}
}




// -----------------------------------   E V E N T S   ---------------------------------------

void CheckKeyEvent(SDL_Event e)
{
	switch (e.key.keysym.sym)
	{
	case SDLK_q:
	{
				   running = false;
				   break;
	}
	default:
	{
			   break;
	}
	}
}


bool PressedSlider(int x, int y)
{
	if ((x < (sliderPosx + sliderWidth)) &&
		(x > sliderPosx) &&
		(y < (sliderPosy + sliderHeight)) &&
		(y >(sliderPosy))
		)
	{
		return true;
	}
	return false;
}


void MoveSlider(int x, int y)
{
	sliderPosx = x;
}
