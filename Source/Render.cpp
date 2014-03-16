#include "Render.h"

void Render::setContext(SDL_Window* window, SDL_GLContext context){
	this->window = window;
	this->context = context;
	
}
Render::Render(int h, int w, InputQueue *que ,RenderQue *rque){
	TTF_Init();
	font = TTF_OpenFont("./Font/COMICATE.ttf", 42);
	screenheight = h;
	screenwidth = w;
	minWidth = w;
	minHeight = h;
	renderQueue = rque;
	renderNow = false;
	shutDown = false;
	inQueue = que;
	int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;// | SDL_WINDOW_FULLSCREEN;
	setUpSDL(flags);
	setUpOGL();
}
//not sure if i need it TODO:::
void Render::setQue(InputQueue *que){
	inQueue = que;
}
void Render::mainLoop(string fps){
//	while (!shutDown){
		//renderThis();

	
	bool end = false;

			while (!end){
				RenderData input = renderQueue->pop();
				//printf("pop item : %i", input.getType());

				switch (input.getType()) {
				case 0: {	//type 0 == mouse click
							input.test();
							//drawSquare(input.points,input.getCenter(), input.getAngle(), input.getColor());
							break;
				}

				case 1: {	//type 0 == mouse click
							input.circle();
						//	drawCircle(input.getCenter(), input.getAngle(), input.getRadius(), input.getColor());
							break;
				}

				case 2: {	//type 0 == mouse click
						
							if (!renderNow){
								renderThis();
								render();
								startRendering();		

							}
							else if (renderNow){
								glEnable(GL_TEXTURE_2D);
								glEnable(GL_BLEND);

								glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

								renderText(font, 255, 255, 255, 100, 40, 0, fps);

								glDisable(GL_TEXTURE_2D);
								glDisable(GL_BLEND);
								renderThis();
								endRendering();
								SDL_GL_SwapWindow(window);
								end = true;
							}
							break;
				}
					
				}
				}

}
void Render::render() {
	//Clear buffer
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

}
void Render::setUpOGL() {
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

void Render::renderOrtho() {

	// Prepare for ortho rendering:
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screenwidth, 0, screenheight);
	glScalef(1, -1, 1);
	glTranslatef(0, -screenheight, 0);
	glMatrixMode(GL_MODELVIEW);
	//world->checkForInput();
	//Draw player
	
	InputData updateWorld(2);
	inQueue->push(updateWorld);

	// Disable GUI rendering:
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
}
void Render::setUpSDL(int flags) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Couldnt init SDL2! SDL_Error: " << SDL_GetError() << endl;
	}

	SDL_Rect screenSize = SDL_Rect();
	SDL_GetDisplayBounds(0, &screenSize);
	cout << "Screen resolution is (" << screenSize.w << "x" << screenSize.h << ")\n";
	maxWidth = screenSize.w;
	maxHeight = screenSize.h;

	window = SDL_CreateWindow("First SDL2 OGL App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenwidth, screenheight, flags);
	context = SDL_GL_CreateContext(window);
}
void Render::renderThis(){
	renderNow = !renderNow;
}
void Render::drawSquare(b2Vec2* points, b2Vec2 center, float angle, b2Vec3 color) {
	vector <float> vertices;
	vector <float> colors;

	//Create rectangle vertices
	for (int i = 0; i < 4; i++) {
		vertices.push_back(points[i].x * M2P);
		vertices.push_back(points[i].y * M2P);
		colors.push_back(color.x);
		colors.push_back(color.y);
		colors.push_back(color.z);
	}

	//Move to position of the object
	glTranslatef(center.x * M2P, center.y * M2P, 0);
	glRotatef(angle * 180.0 / M_PI, 0, 0, 1);

	//Enable required buffers
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, &vertices.front());
	glColorPointer(3, GL_FLOAT, 0, &colors.front());

	//draw the Cube
	glDrawArrays(GL_QUADS, 0, vertices.size() / 2);

	//restore the state GL back
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	//Move back
	glTranslatef(-center.x * M2P, -center.y * M2P, 0);
}
void Render::startRendering(){
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screenwidth, 0, screenheight);
	glScalef(1, -1, 1);
	glTranslatef(0, -screenheight, 0);
	glMatrixMode(GL_MODELVIEW);
}
void Render::endRendering(){
	// Disable GUI rendering:
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
}
void Render::drawCircle(b2Vec2 center, float angle, float radius, b2Vec3 color){
	vector <float> vertices;
	vector <float> colors;

	//Push central point
	vertices.push_back(0);
	vertices.push_back(0);
	colors.push_back(color.x);
	colors.push_back(color.y);
	colors.push_back(color.z);

	//Creat points for circle (fan around center)
	for (float i = 0.0; i <= 360; i += 360.0 / 30) {
		float thisX = (cos(i * M_PI / 180.0) * (radius)) * M2P;
		float thisY = (sin(i * M_PI / 180.0) * (radius)) * M2P;

		//Center
		vertices.push_back(thisX);
		vertices.push_back(thisY);
		colors.push_back(color.x);
		colors.push_back(color.y);
		colors.push_back(color.z);
	}

	//Move to position of the object
	glTranslatef(center.x * M2P, center.y * M2P, 0);
	glRotatef(angle * 180.0 / M_PI, 0, 0, 1);

	//Enable required buffers
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//Fill buffers with draw data
	glVertexPointer(2, GL_FLOAT, 0, &vertices.front());
	glColorPointer(3, GL_FLOAT, 0, &colors.front());

	//draw the Cube
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);

	//restore the state GL back
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	//Move back
	glTranslatef(-center.x * M2P, -center.y * M2P, 0);
}
/*
RenderQue* Render::getQue(){

	return renderQueue;
}
//Render::Render()
{
	
	printf("\n Constructor Called");

}*/
Render::~Render()
{
}
//Render *Render::instance;
void Render::renderText(const TTF_Font *Font, const GLubyte& R, const GLubyte& G, const GLubyte& B,
	const double& X, const double& Y, const double& Z, const std::string& Text)
{
	/*Create some variables.*/
	SDL_Color Color = { R, G, B };
	SDL_Surface *Message = TTF_RenderText_Blended(const_cast<TTF_Font*>(Font), Text.c_str(), Color);
	unsigned Texture = 0;

	/*Generate an OpenGL 2D texture from the SDL_Surface*.*/
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Message->w, Message->h, 0, GL_BGRA,
		GL_UNSIGNED_BYTE, Message->pixels);

	/*Draw this texture on a quad with the given xyz coordinates.*/
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex3d(X, Y, Z);
	glTexCoord2d(1, 0); glVertex3d(X + Message->w, Y, Z);
	glTexCoord2d(1, 1); glVertex3d(X + Message->w, Y + Message->h, Z);
	glTexCoord2d(0, 1); glVertex3d(X, Y + Message->h, Z);
	glEnd();

	/*Clean up.*/
	glDeleteTextures(1, &Texture);
	SDL_FreeSurface(Message);
}