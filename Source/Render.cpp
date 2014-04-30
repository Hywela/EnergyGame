#include "Render.h"

Render::Render(Init *init){
	this->init = init;
	init->SDL();
	init->OpenGL();
	cameraX = 0;
	cameraY = 0;
	ledR = ledG = ledB = 1;

	//loop = &Render::mainMenu;
	menuObjects = new vector<button>;
	pauseObjects = new vector<button>;
	scoreButtons = new vector<button>;
	scoreTexts = new vector<button>;
	settingsButtons = new vector<button>;
	//popupButtons = new vector<button>;
	TTF_Init();
	screenHeight = init->getScreenHeight();
	screenWidth = init->getScreenWidth();
	float scale = 1080.0 / screenHeight;
	font = TTF_OpenFont("./Font/CaviarDreams.ttf", 42 / scale);
	menuFont = TTF_OpenFont("./Font/CaviarDreams.ttf", 60 / scale);
	popupFont = TTF_OpenFont("./Font/CaviarDreams.ttf", 20 / scale);
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
	
	//pauseVBO->pushBackground(vx, b2Vec2(screenHeight / 2, screenWidth / 2), b2Vec3(0, 0, 255));
	setBackgroundSquare(0, 0, screenWidth, screenHeight, b2Vec3(0, 0, 0), pauseVBO);
	setBackgroundSquare(0, 0, screenWidth, screenHeight, b2Vec3(0, 255, 255), backgroundVBO);
	shader = new Shader("./Shaders/ligthShader.vert", "./Shaders/ligthShader.frag");
	platformShader =  new Shader("./Shaders/platformShader.vert", "./Shaders/platformShader.frag");
	colorShader = new Shader("./Shaders/colorShader.vert", "./Shaders/colorShader.frag");
	setUniforms(shader);
	
	
}
Render::~Render()
{

	//delete popupButtons;
	delete menuObjects;
	delete pauseObjects;
	delete scoreButtons;
	delete scoreTexts;
	delete settingsButtons;
	delete platformVBO;
	delete backgroundVBO;
	delete pauseVBO;
	delete particleVBO;
	delete mainCharParticleVBO;
	delete shader;
	delete colorShader;
	delete platformShader;
	delete init;

	//Delete fonts not working

}
void Render::setUniforms(Shader *tempShader){
	litLightColor = glGetUniformLocation(*tempShader->GetShaderProgram(), "litLightColor");
	unlitLightColor = glGetUniformLocation(*tempShader->GetShaderProgram(), "unlitLightColor");
	mUniformscreenHeight = glGetUniformLocation(*tempShader->GetShaderProgram(), "screenHeight");
	lightAttenuation = glGetUniformLocation(*tempShader->GetShaderProgram(), "lightAttenuation");
	radius = glGetUniformLocation(*tempShader->GetShaderProgram(), "radius");
	//mUniformTexture= glGetUniformLocation(*shader->GetShaderProgram(), "myTexture");
	numLigth = glGetUniformLocation(*tempShader->GetShaderProgram(), "particleNumLigth");
	platformNumLitLigth = glGetUniformLocation(*tempShader->GetShaderProgram(), "platformNumLitLigth");
	platformNumUnlitLigth = glGetUniformLocation(*tempShader->GetShaderProgram(), "platformNumUnlitLigth");

}
void Render::pauseLoop(){
	render();
	startRendering();

	colorShading();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < pauseObjects->size(); i++){
		renderText(menuFont, pauseObjects->at(i).color, pauseObjects->at(i).posX, pauseObjects->at(i).posY, 0, pauseObjects->at(i).tekst);
	}

	endRendering();
	SDL_GL_SwapWindow(init->window);
}
void Render::scoreLoop(vector <int> scores, int scoreFinal, int scorePos, bool inGame) {
	render();
	startRendering();

	colorShading();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Only render if opened after a ended game
	scoreButtons->at(0).disabled = true;
	scoreButtons->at(1).tekst = "Back";
	scoreTexts->at(0).color = HIGHSCORE_COLOR_NEW;

	if (inGame) {
		//Render text
		scoreTexts->at(0).tekst = "Score: " + to_string(scoreFinal);
		renderText(menuFont, scoreTexts->at(0).color, scoreTexts->at(0).posX, scoreTexts->at(0).posY, 0, scoreTexts->at(0).tekst);

		//Set appropriate name for menu button
		scoreButtons->at(1).tekst = "Main Menu";

		//Render "retry" button
		scoreButtons->at(0).disabled = false;
		renderText(menuFont, scoreButtons->at(0).color, scoreButtons->at(0).posX, scoreButtons->at(0).posY, 0, scoreButtons->at(0).tekst);
	}

	for (int i = 1; i < scoreTexts->size(); i++) {
		//If highscore list element
		if (i >= 2 && i < 2 + HIGHSCORES) {
			//Update scoretext
			if (scores[i - 2] >= 0) {
				scoreTexts->at(i).tekst = to_string(i - 1) + ": " + to_string(scores[i - 2]);
			}
			else {
				scoreTexts->at(i).tekst = to_string(i - 1) + ": ---";
			}

			if ((i - 2) == scorePos) {
				//Highlight new highscore
				scoreTexts->at(i).color = HIGHSCORE_COLOR_NEW;
			}
			else {
				//Set normal color to others
				scoreTexts->at(i).color = HIGHSCORE_COLOR_NORMAL;
			}
		}

		//Render text
		renderText(menuFont, scoreTexts->at(i).color, scoreTexts->at(i).posX, scoreTexts->at(i).posY, 0, scoreTexts->at(i).tekst);
	}

	//Render "goto main menu" button
	renderText(menuFont, scoreButtons->at(1).color, scoreButtons->at(1).posX, scoreButtons->at(1).posY, 0, scoreButtons->at(1).tekst);

	endRendering();
	SDL_GL_SwapWindow(init->window);
}
void Render::settingsLoop(int musVol, int effVol, string col1, string col2, string grav) {
	render();
	startRendering();

	colorShading();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Update volume button
	settingsButtons->at(0).tekst = "Music volume: " + to_string(musVol) + "%";
	settingsButtons->at(1).tekst = "Effects volume: " + to_string(effVol) + "%";

	settingsButtons->at(3).tekst = "Orb Color: " + col1;
	settingsButtons->at(4).tekst = "Orb Light Color: " + col2;

	settingsButtons->at(6).tekst = "Gravity: " + grav;

	for (int i = 0; i < settingsButtons->size(); i++) {
		//Render text
		renderText(menuFont, settingsButtons->at(i).color, settingsButtons->at(i).posX, settingsButtons->at(i).posY, 0, settingsButtons->at(i).tekst);
	}

	endRendering();
	SDL_GL_SwapWindow(init->window);
}

void Render::mainLoop(string fps, string puz, string par, string sco, string tim, string csp){

	
	gameLoopShading();
	//mainLoopShading(colorShader, 1);
	particleVBO->draw();
	
	//mainCharParticleVBO->draw();

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		int textX = 20;
		int textY = 20;

		if (fps != "") {
			renderText(font, MENU_COLOR_NORMAL, 20, screenHeight - 50, 0, fps);
		}
		if (puz != "") {
			renderText(font, MENU_COLOR_NORMAL, textX, textY, 0, puz);
			textY += 50;
		}
		if (par != "") {
			renderText(font, MENU_COLOR_NORMAL, textX, textY, 0, par);
			textY += 50;
		}
		if (sco != "") {
			renderText(font, MENU_COLOR_NORMAL, textX, textY, 0, sco);
			textY += 50;
		}
		if (csp != "") {
			renderText(font, MENU_COLOR_NORMAL, textX, textY, 0, csp);
			textY += 50;
		}
		if (tim != "") {
			int timeX = (screenWidth / 2) - ((tim.size() / 2) * 20);
			int timeY = 40;
			renderText(font, MENU_COLOR_NORMAL, timeX, timeY, 0, tim);
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		
		endRendering();
		SDL_GL_SwapWindow(init->window);
		
}
void Render::gameLoopShading(){
	
	GLint uniform_location;
	glUseProgram(*shader->GetShaderProgram());
	
	uniform_location = glGetUniformLocationARB(*shader->GetShaderProgram(), "tex");
	glUniform1iARB(uniform_location, 0);
	glBindAttribLocation(*shader->GetShaderProgram(), VBO_VERTEX, "vertex");
	glBindAttribLocation(*shader->GetShaderProgram(), VBO_COLOR, "color");
	glBindAttribLocation(*shader->GetShaderProgram(), VBO_TEXCORD, "texCoord");

	glUniform1i(numLigth, particleVBO->getCenterSize());
	glUniform1i(platformNumLitLigth, platformVBO->getCenterLitSize());
	glUniform1i(platformNumUnlitLigth, platformVBO->getCenterUnlitSize());

	glUniform2f(glGetUniformLocation(*shader->GetShaderProgram(), "MainCharLightpos"), mainCharParticleVBO->getMainCenter().x,
		mainCharParticleVBO->getMainCenter().y);
	//glUniform2f(glGetUniformLocation(*shader->GetShaderProgram(), "mousePointerLigthpos"), mousePointer.x, mousePointer.y);
	glUniform2fv(glGetUniformLocation(*shader->GetShaderProgram(), "lightpos"), particleVBO->getCenterSize(), particleVBO->getCenter());

	glUniform2fv(glGetUniformLocation(*shader->GetShaderProgram(), "litPlatformLightpos"), platformVBO->getCenterLitSize(), platformVBO->getCenterLit());
	glUniform2fv(glGetUniformLocation(*shader->GetShaderProgram(), "unlitPlatformLightpos"), platformVBO->getCenterUnlitSize(), platformVBO->getCenterUnlit());
	//glUniform2f(glGetUniformLocation(*shader->GetShaderProgram(), "lightpos"), mainCharParticleVBO->getCenter().x, mainCharParticleVBO->getCenter().y);
	glUniform3f(litLightColor, 55, 208, 81);
	glUniform3f(unlitLightColor, 255, 0, 0);
	glUniform1f(mUniformscreenHeight, screenHeight);
	glUniform3f(lightAttenuation, 1, 1, 1);
	glUniform1f(radius, 5);
	glUniform3f(glGetUniformLocation(*shader->GetShaderProgram(), "particleLightColor"), 255-ledR, 255-ledG, 255-ledB);
	
	//if (i == 0)
	backgroundVBO->draw(true);
	//else
	

	glUseProgram(0);
	
	glUseProgram(*platformShader->GetShaderProgram());

	uniform_location = glGetUniformLocationARB(*platformShader->GetShaderProgram(), "tex");
	glUniform1iARB(uniform_location, 0);
	glBindAttribLocation(*platformShader->GetShaderProgram(), VBO_VERTEX, "vertex");
	glBindAttribLocation(*platformShader->GetShaderProgram(), VBO_COLOR, "color");
	glBindAttribLocation(*platformShader->GetShaderProgram(), VBO_TEXCORD, "texCoord");

	glUniform1i(glGetUniformLocation(*platformShader->GetShaderProgram(), "particleNumLigth"), particleVBO->getCenterSize());
	glUniform1i(glGetUniformLocation(*platformShader->GetShaderProgram(), "platformNumLitLigth"), platformVBO->getCenterLitSize());
	glUniform1i(glGetUniformLocation(*platformShader->GetShaderProgram(), "platformNumUnlitLigth"), platformVBO->getCenterUnlitSize());

	glUniform2f(glGetUniformLocation(*platformShader->GetShaderProgram(), "MainCharLightpos"), mainCharParticleVBO->getMainCenter().x,
		mainCharParticleVBO->getMainCenter().y);
	//glUniform2f(glGetUniformLocation(*shader->GetShaderProgram(), "mousePointerLigthpos"), mousePointer.x, mousePointer.y);
	glUniform2fv(glGetUniformLocation(*platformShader->GetShaderProgram(), "lightpos"), particleVBO->getCenterSize(), particleVBO->getCenter());

	glUniform2fv(glGetUniformLocation(*platformShader->GetShaderProgram(), "litPlatformLightpos"), platformVBO->getCenterLitSize(), platformVBO->getCenterLit());
	glUniform2fv(glGetUniformLocation(*platformShader->GetShaderProgram(), "unlitPlatformLightpos"), platformVBO->getCenterUnlitSize(), platformVBO->getCenterUnlit());
	//glUniform2f(glGetUniformLocation(*shader->GetShaderProgram(), "lightpos"), mainCharParticleVBO->getCenter().x, mainCharParticleVBO->getCenter().y);
	glUniform3f(glGetUniformLocation(*platformShader->GetShaderProgram(), "litLightColor"), 55, 208, 81);
	glUniform3f(glGetUniformLocation(*platformShader->GetShaderProgram(), "unlitLightColor"), 255, 0, 0);
	glUniform1f(glGetUniformLocation(*platformShader->GetShaderProgram(), "screenHeight"), screenHeight);
	glUniform3f(glGetUniformLocation(*platformShader->GetShaderProgram(), "lightAttenuation"), 1, 1, 1);
	glUniform1f(glGetUniformLocation(*platformShader->GetShaderProgram(), "radius"), 50);



	platformVBO->drawTexture();


	glUseProgram(0);
	

}
void Render::colorShading(){
	glUseProgram(*colorShader->GetShaderProgram());
	glBindAttribLocation(*shader->GetShaderProgram(), VBO_VERTEX, "vertex");
	glBindAttribLocation(*shader->GetShaderProgram(), VBO_COLOR, "color");
	pauseVBO->draw(false);
	glUseProgram(0);
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

	colorShading();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	renderText(font, MENU_COLOR_NORMAL, 20, screenHeight - 50, 0, fps);
	for (int i = 0; i < menuObjects->size(); i++){
		renderText(menuFont, menuObjects->at(i).color, menuObjects->at(i).posX, menuObjects->at(i).posY, 0, menuObjects->at(i).tekst);
	}

	endRendering();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
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
	glBindTexture(GL_TEXTURE_2D, 0);
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

	but.disabled = false;
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

	but.disabled = false;
	pauseObjects->push_back(but);
}
void Render::pushBackScoreBtn(int posX, int posY, string tekst){
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

	but.disabled = false;
	scoreButtons->push_back(but);
}
void Render::pushBackScoreTxt(int posX, int posY, string tekst){
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

	but.disabled = false;
	scoreTexts->push_back(but);
}
void Render::pushBackSettingsBtn(int posX, int posY, string tekst){
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

	but.disabled = false;
	settingsButtons->push_back(but);
}

int Render::menuMouseClickCheck(int x, int y){
	for (int i = 0; i < menuObjects->size(); i++){
		if(menuObjects->at(i).buttonBox[0].x <= x && menuObjects->at(i).buttonBox[2].y >= y){
			if (menuObjects->at(i).buttonBox[2].x >= x && menuObjects->at(i).buttonBox[0].y <= y){
				if (!menuObjects->at(i).disabled) {
					menuObjects->at(i).color = MENU_COLOR_NORMAL;
					return i + 1;
				}
			}

		}
	}
	return 0;
}
int Render::pauseMouseClickCheck(int x, int y){
	for (int i = 0; i < pauseObjects->size(); i++){
		if (pauseObjects->at(i).buttonBox[0].x <= x && pauseObjects->at(i).buttonBox[2].y >= y){
			if (pauseObjects->at(i).buttonBox[2].x >= x && pauseObjects->at(i).buttonBox[0].y <= y){
				if (!pauseObjects->at(i).disabled) {
					pauseObjects->at(i).color = MENU_COLOR_NORMAL;
					return i + 1;
				}
			}
		}
	}

	return 0;
}
int Render::scoreMouseClickCheck(int x, int y){
	for (int i = 0; i < scoreButtons->size(); i++) {
		if (scoreButtons->at(i).buttonBox[0].x <= x && scoreButtons->at(i).buttonBox[2].y >= y) {
			if (scoreButtons->at(i).buttonBox[2].x >= x && scoreButtons->at(i).buttonBox[0].y <= y) {
				if (!scoreButtons->at(i).disabled) {
					scoreButtons->at(i).color = MENU_COLOR_NORMAL;
					return i + 1;
				}
			}
		}
	}

	return 0;
}
int Render::settingsMouseClickCheck(int x, int y){
	for (int i = 0; i < settingsButtons->size(); i++) {
		if (settingsButtons->at(i).buttonBox[0].x <= x && settingsButtons->at(i).buttonBox[2].y >= y) {
			if (settingsButtons->at(i).buttonBox[2].x >= x && settingsButtons->at(i).buttonBox[0].y <= y) {
				if (!settingsButtons->at(i).disabled) {
					settingsButtons->at(i).color = MENU_COLOR_NORMAL;
					return i + 1;
				}
			}
		}
	}

	return 0;
}
void Render::menuMouseHoverCheck(int x, int y, int type){
	vector<button> *temp = NULL;

	switch (type) {
		case MENU_MAIN: {temp = menuObjects; break;}
		case MENU_PAUSE: {temp = pauseObjects; break;}
		case MENU_SETTINGS: {temp = settingsButtons; break;}
		case MENU_HIGHSCORE: {temp = scoreButtons; break;}
		default: {break;}
	}

	if (temp != NULL) {
		for (int i = 0; i < temp->size(); i++){
			temp->at(i).color = MENU_COLOR_NORMAL;

			if (temp->at(i).buttonBox[0].x <= x && temp->at(i).buttonBox[2].y >= y){
				if (temp->at(i).buttonBox[2].x >= x && temp->at(i).buttonBox[0].y <= y){
					temp->at(i).color = MENU_COLOR_HOVER;
				}
			}
		}
	}
}
void Render::setBackgroundSquare(int posX, int posY, int offsetX, int offsetY , b2Vec3 color , PlatformVBO *tempVBO){

	b2Vec2 temp[4];

	temp[0].x = posX;
	temp[0].y = posY;

	temp[1].x = posX + offsetX;
	temp[1].y = posY;

	temp[2].x = posX + offsetX;
	temp[2].y = posY + offsetY;

	temp[3].x = posX;
	temp[3].y = posY + offsetY;

	tempVBO->pushBackground(temp, b2Vec2(screenHeight / 2, screenWidth / 2), color);
}
void Render::setMousePointLigth(int x, int y){
	mousePointer.x = x;
	mousePointer.y = y;
}
void Render::pushOrClearPopupMenu(int type, int x, int y ){
	pauseVBO->clear();
	if (type == 0){
		setBackgroundSquare(0, 0, screenWidth, screenHeight, b2Vec3(0, 0, 0), pauseVBO);
		setBackgroundSquare(x, y, 200, 200, b2Vec3(0, 255, 255), pauseVBO);
	}else
		setBackgroundSquare(0, 0, screenWidth, screenHeight, b2Vec3(0, 0, 0), pauseVBO);
}
void Render::setCameraDirectionX(int offsett){
	cameraX += offsett;
	screenWidth += offsett;
}
void Render::zeroOutCamera(){
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
void Render::setLightColor(int r, int g, int b) {
	ledR = r;
	ledG = g;
	ledB = b;
}
