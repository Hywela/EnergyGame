#include "Render.h"

Render::Render(Init *init){
	this->init = init;
	init->SDL();
	init->OpenGL();
	cameraX = 0;
	cameraY = 0;

	//loop = &Render::mainMenu;
	menuObjects = new vector<button>;
	pauseObjects = new vector<button>;
	TTF_Init();
	font = TTF_OpenFont("./Font/helvetica-neue-lt-com-25-ultra-light.ttf", 42);
	menuFont = TTF_OpenFont("./Font/helvetica-neue-lt-com-25-ultra-light.ttf", 100);
	screenHeight = init->getScreenHeight();
	screenWidth = init->getScreenWidth();
	renderNow = false;
	shutDown = false;
	

	platformVBO = new PlatformVBO();
	particleVBO = new ParticleVBO();
	backgroundVBO = new PlatformVBO(); 
	pauseVBO = new PlatformVBO();	
	mainCharParticleVBO = new ParticleVBO();
	b2Vec2 vx[4];
	vx[0].x = 0;
	vx[0].y = 0;

	vx[1].x = screenWidth;
	vx[1].y = 0;

	vx[2].x = screenWidth;
	vx[2].y = screenHeight;

	vx[3].x = 0;
	vx[3].y = screenHeight;

	backgroundVBO->pushBackground(vx, b2Vec2(screenWidth / 2, screenHeight / 2), b2Vec3(0, 0, 0));


	//geoShader = new Shader("./Shaders/main_shader.vert", "./Shaders/main_shader.frag","./Shaders/main_shader.geom" );
	shader = new Shader("./Shaders/platformShader.vert", "./Shaders/platformShader.frag");
	//geoShader = new Shader("./Shaders/platformShader.vert", "./Shaders/platformShader.frag");
	
	lightColor = glGetUniformLocation(*shader->GetShaderProgram(), "lightColor");
	mUniformscreenHeight = glGetUniformLocation(*shader->GetShaderProgram(), "screenHeight");
	lightAttenuation = glGetUniformLocation(*shader->GetShaderProgram(), "lightAttenuation");
	radius = glGetUniformLocation(*shader->GetShaderProgram(), "radius");
	numLigth = glGetUniformLocation(*shader->GetShaderProgram(), "numLigth");
	
	
	
	
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
//	mUniform = glGetUniformLocation(*shader->GetShaderProgram(), "time");
}
Render::~Render()
{
	delete font;
	delete init;

}
void Render::pauseLoop(){
	render();
	startRendering();

	pauseVBO->draw();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	
	for (int i = 0; i < pauseObjects->size(); i++){
		renderText(menuFont, pauseObjects->at(i).color, pauseObjects->at(i).posX, pauseObjects->at(i).posY, 0, pauseObjects->at(i).tekst);
	}

	endRendering();
	SDL_GL_SwapWindow(init->window);


}
void Render::mainLoop(string fps, string puz, string par){

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	glUseProgram(*shader->GetShaderProgram());	
	backgroundVBO->draw();
	platformVBO->draw();

	glUniform1i(numLigth, particleVBO->getCenterSize());

	glUniform2f(glGetUniformLocation(*shader->GetShaderProgram(), "MainCharLightpos"), mainCharParticleVBO->getMainCenter().x,
		mainCharParticleVBO->getMainCenter().y);
	glUniform2fv(glGetUniformLocation(*shader->GetShaderProgram(), "lightpos"), particleVBO->getCenterSize(), particleVBO->getCenter());
	glUniform2fv(glGetUniformLocation(*shader->GetShaderProgram(), "platformLightpos"), platformVBO->getCenterSize(), platformVBO->getCenter());
	//glUniform2f(glGetUniformLocation(*shader->GetShaderProgram(), "lightpos"), mainCharParticleVBO->getCenter().x, mainCharParticleVBO->getCenter().y);
	glUniform3f(lightColor, 1, 1, 0.81);
	glUniform1f(mUniformscreenHeight, screenHeight);
	glUniform3f(lightAttenuation, -1, 1, 1);
	glUniform1f(radius,40);


	//particleVBO->draw();



	//particleVBO->setUniforms(lightColor, mUniformscreenHeight, lightAttenuation, radius, lightpos, screenHeight, shader);
		
	//



	glUseProgram(0);
	glDisable(GL_BLEND);

	particleVBO->draw();
	mainCharParticleVBO->draw();

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		int textX = 20;
		int textY = 20;

		if (fps != "") {
			renderText(font, MENU_COLOR_NORMAL, textX, textY, 0, fps);
			textY += 50;
		}
		if (puz != "") {
			renderText(font, MENU_COLOR_NORMAL, textX, textY, 0, puz);
			textY += 50;
		}
		if (par != "") {
			renderText(font, MENU_COLOR_NORMAL, textX, textY, 0, par);
			textY += 50;
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		
		endRendering();
		SDL_GL_SwapWindow(init->window);
		

}

void Render::render() {
	//Clear buffer
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	//gluLookAt(0, 50,0,   0,0,0,   0,1,0);
}
void Render::renderOrtho() {
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, 0, screenHeight);
	//glScalef(1, -1, 1);
	//glTranslatef(0, -screenHeight, 0);
	glMatrixMode(GL_MODELVIEW);
}

void Render::renderThis(){
	renderNow = !renderNow;
}

void Render::startRendering(){
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glLoadIdentity();	
	gluOrtho2D(cameraX, screenWidth, cameraY, screenHeight);
	//glViewport(0, 0, screenWidth, screenHeight);

	glScalef(1, -1, 1);
	glTranslatef(0, -screenHeight, 0);
	glMatrixMode(GL_MODELVIEW);

}

void Render::endRendering(){
	// Disable GUI rendering:
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
}

void Render::mainMenu(string fps){

	render();
	startRendering();	

	pauseVBO->draw();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	renderText(menuFont, MENU_COLOR_NORMAL , 100, 600, 0, fps);
	for (int i = 0; i < menuObjects->size(); i++){
		renderText(menuFont, menuObjects->at(i).color, menuObjects->at(i).posX, menuObjects->at(i).posY, 0, menuObjects->at(i).tekst);
	}

	endRendering();
	SDL_GL_SwapWindow(init->window);
	/*
	GLfloat lightpos[] = { 0.5, 1.0, 1., 0.0 };
	glUseProgram(*shader->GetShaderProgram());
	GLint LightPos = glGetUniformLocation(*shader->GetShaderProgram(), "LightPos");
	glProgramUniform3f(*shader->GetShaderProgram(), LightPos, lightpos[0], lightpos[1], lightpos[2]);
	if (mUniformTexture != -1) {
		glUniform1i(mUniformTexture, 0);
	}
	glUseProgram(0);*/
}
//Render *Render::instance;
void Render::renderText(const TTF_Font *Font, SDL_Color Color,
	const double& X, const double& Y, const double& Z, const std::string& Text)
{
	/*Create some variables.*/
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
void Render::pushBackMenuObj(int posX, int posY, string tekst ){

	button but;
	but.color = MENU_COLOR_NORMAL;
	but.posX = posX;
	but.posY = posY;
	but.tekst = tekst;
	int offsetX = 700;
	int offsetY = 100;

	but.buttonBox[0].x = posX;
	but.buttonBox[0].y = posY;

	but.buttonBox[1].x = posX + offsetX;
	but.buttonBox[1].y = posY;

	but.buttonBox[2].x = posX + offsetX;
	but.buttonBox[2].y = posY + offsetY;

	but.buttonBox[3].x = posX;
	but.buttonBox[3].y = posY + offsetY;
	menuObjects->push_back(but);
	
}
void Render::pushBackPauseObj(int posX, int posY, string tekst){

	button but;
	but.color = MENU_COLOR_NORMAL;
	but.posX = posX;
	but.posY = posY;
	but.tekst = tekst;
	int offsetX = 700;
	int offsetY = 100;

	but.buttonBox[0].x = posX;
	but.buttonBox[0].y = posY;

	but.buttonBox[1].x = posX + offsetX;
	but.buttonBox[1].y = posY;

	but.buttonBox[2].x = posX + offsetX;
	but.buttonBox[2].y = posY + offsetY;

	but.buttonBox[3].x = posX;
	but.buttonBox[3].y = posY + offsetY;
	pauseObjects->push_back(but);
}

int Render::menuMouseClickCheck(int x, int y){
	for (int i = 0; i < menuObjects->size(); i++){
		if(menuObjects->at(i).buttonBox[0].x <= x && menuObjects->at(i).buttonBox[2].y >= y){
			if (menuObjects->at(i).buttonBox[2].x >= x && menuObjects->at(i).buttonBox[0].y <= y){
				cout << " ddd";
				return i;
			}

		}
	}
	return NULL;
}
int Render::pauseMouseClickCheck(int x, int y){
	for (int i = 0; i < pauseObjects->size(); i++){
		if (pauseObjects->at(i).buttonBox[0].x <= x && pauseObjects->at(i).buttonBox[2].y >= y){

			if (pauseObjects->at(i).buttonBox[2].x >= x && pauseObjects->at(i).buttonBox[0].y <= y){
				

				return i;
			}

		}
	}
	return NULL;
}
void Render::menuMouseHoverCheck(int x, int y, int type){
	vector<button> *temp = NULL;

	if (type == 0)
		temp = menuObjects;
	else if (type == 1)
		temp = pauseObjects;

	for (int i = 0; i < temp->size(); i++){
		temp->at(i).color = MENU_COLOR_NORMAL;
		if (temp->at(i).buttonBox[0].x <= x && temp->at(i).buttonBox[2].y >= y){
		
			if (temp->at(i).buttonBox[2].x >= x && temp->at(i).buttonBox[0].y <= y){
				temp->at(i).color = MENU_COLOR_HOVER;
				

			}
		
		}
	}
}
void Render::setCameraDirectionX(int offsett){
	cameraX += offsett;
	screenWidth += offsett;
}
void Render::zerOutCamera(){
	cameraX = 0;
	screenWidth = 1920;
}
Init* Render::getInit(){
	return init;
}
PlatformVBO* Render::getPlatformVBO(){
	return platformVBO;
}
ParticleVBO* Render::getParticleVBO(){
	return particleVBO;
}
ParticleVBO* Render::getMainCharParticleVBO(){
	return mainCharParticleVBO;
}