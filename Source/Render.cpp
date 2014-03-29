#include "Render.h"

Render::Render(Init *init, InputQueue *que ,RenderQue *rque){
	this->init = init;
	init->SDL();
	init->OpenGL();
	cameraX = 0;
	cameraY = 0;
	//loop = &Render::mainMenue;
	menueObjects = new 	vector<button>;
	TTF_Init();
	font = TTF_OpenFont("./Font/helvetica-neue-lt-com-25-ultra-light.ttf", 42);
	menueFont = TTF_OpenFont("./Font/helvetica-neue-lt-com-25-ultra-light.ttf", 100);
	screenHeight = init->getScreenHeight();
	screenWidth = init->getScreenWidth();
	renderQueue = rque;
	renderNow = false;
	shutDown = false;
	inQueue = que;
	platformVBO = new PlatformVBO();
	particleVBO = new ParticleVBO();
	backgroundVBO = new PlatformVBO(); 
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

	mainCharParticleVBO = new ParticleVBO();
	//geoShader = new Shader("./Shaders/main_shader.vert", "./Shaders/main_shader.frag","./Shaders/main_shader.geom" );
	shader = new Shader("./Shaders/platformShader.vert", "./Shaders/platformShader.frag");
	geoShader = new Shader("./Shaders/platformShader.vert", "./Shaders/platformShader.frag");
	
	lightColor = glGetUniformLocation(*shader->GetShaderProgram(), "lightColor");
	mUniformscreenHeight = glGetUniformLocation(*shader->GetShaderProgram(), "screenHeight");
	lightAttenuation = glGetUniformLocation(*shader->GetShaderProgram(), "lightAttenuation");
	radius = glGetUniformLocation(*shader->GetShaderProgram(), "radius");
	numLigth = glGetUniformLocation(*shader->GetShaderProgram(), "numLigth");
	
	
	
	
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
//	mUniform = glGetUniformLocation(*shader->GetShaderProgram(), "time");
}

//not sure if i need it TODO:::
void Render::setQue(InputQueue *que){
	inQueue = que;
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

	//mainCharParticleVBO->draw();

	//particleVBO->setUniforms(lightColor, mUniformscreenHeight, lightAttenuation, radius, lightpos, screenHeight, shader);
		
	//



	glUseProgram(0);
	glDisable(GL_BLEND);

	particleVBO->draw();


		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		int textX = 20;
		int textY = 20;

		if (fps != "") {
			renderText(font, 255, 255, 255, textX, textY, 0, fps);
			textY += 50;
		}
		if (puz != "") {
			renderText(font, 255, 255, 255, textX, textY, 0, puz);
			textY += 50;
		}
		if (par != "") {
			renderText(font, 255, 255, 255, textX, textY, 0, par);
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
void Render::mainMenue(string fps){

	//particleVBO->pushBack(b2Vec2(20, 20), 0, 1, b2Vec3(0,255,255));

	render();
	startRendering();	
	
	/*
	glBlendFunc(GL_ONE, GL_ONE);
	glUseProgram(*shader->GetShaderProgram());
	
		glUniform2f(lightpos, 25, 25);
		glUniform3f(lightColor, 0, 255,255);
		glUniform1f(mUniformscreenHeight, screenHeight);
		glUniform3f(lightAttenuation, 1, 1, 0);
		glUniform1f(radius, 10);

		
	glUseProgram(0);*/
	





	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	renderText(menueFont, 0, 255, 255, 100, 600, 0,fps);
	for (int i = 0; i < menueObjects->size(); i++){
		renderText(menueFont, 255, 255, 255, menueObjects->at(i).posX, menueObjects->at(i).posY, 0, menueObjects->at(i).tekst);
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
void Render::pushBackMenueObj(int posX, int posY, string tekst){

	button but;

	but.posX = posX;
	but.posY = posY;
	but.tekst = tekst;

	b2Vec2 vx[4];
	vx[0].x = 0;
	vx[0].y = 0;

	vx[1].x = posX+20;
	vx[1].y = 0;

	vx[2].x = posX+20;
	vx[2].y = posY+20;

	vx[3].x = 0;
	vx[3].y = posY+20;
	menueObjects->push_back(but);
	platformVBO->pushBack(vx, b2Vec2(posX + 20, posY + 20), 0, b2Vec3(0, 255, 255));
}
int Render::menueMouseClickCheck(int x, int y){
	for (int i = 0; i < menueObjects->size(); i++){
		if (menueObjects->at(i).posX <= x && menueObjects->at(i).posY >= y){
				return i;
				
		}
	}
	return NULL;
}
void Render::menueMouseHoverCheck(int x, int y){
	for (int i = 0; i < menueObjects->size(); i++){
		if (menueObjects->at(i).posX <= x && menueObjects->at(i).posY >= y){


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