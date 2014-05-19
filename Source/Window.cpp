#include "Window.h"

Window::Window() {
	running = true;
	isFullscreen = false;
	paused = false;
	leftMouseClick = &Window::menuLeftMouseClick;
	loopType = &Window::menuLoop;
	showDebug = false;

	gravityX = gravityY = 0;
	grav = "Off";

	int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;// | SDL_WINDOW_FULLSCREEN;

	ren = new Render(new Init( flags));
	tempOFFON = false;
	buildMenu();

	r1 = 255; g1 = 255; b1 = 255;
	r2 = 255; g2 = 255; b2 = 255;
	nextColor(r1, g1, b1, col1, true);
	nextColor(r2, g2, b2, col2, true);
	ren->setLightColor(r2, g2, b2);

	//fps test variables
	fps_lasttime = SDL_GetTicks(); //the last recorded time.
	fps_current = 34; //the current FPS.
	fps_frames = 0; //frames passed since the last recorded fps.
	inGame = false;
	scoreFinal = -1;
	scorePos = -1;

	//Open audio channels
	if (Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_BUFFERS)) {
		printf("Unable to open audio!\n");
	}

	//Initialze scores
	for (int i = 0; i < HIGHSCORES; i++) {
		highScores.push_back(-1);
	}

	world = NULL;
	musicVolume = 0;
	Mix_VolumeMusic(musicVolume);
	effectVolume = 0;
	Mix_Volume(EFFECTS, effectVolume);

	string sndPath = DIR_EFFECTS + "Death.wav";
	snd_Lost = Mix_LoadWAV(sndPath.c_str());

	loadData();
}

Window::~Window() {

}

void Window::mainLoop() {
	currentMenu = MENU_MAIN;

	while (running) {
		checkForMouseInput();
		(this->*loopType)();
		timer = SDL_GetTicks();
	}

	Mix_CloseAudio();
	saveData();
	delete ren;
}

void Window::checkForMouseInput(){

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_MOUSEMOTION:{
			if (inGame && tempOFFON)
				ren->setMousePointLigth(e.button.x, e.button.y);
			if (currentMenu) {
				ren->menuMouseHoverCheck(e.button.x, e.button.y, currentMenu);
			}
			break;
		}//End Case()
		case SDL_KEYDOWN: {
			if (e.key.keysym.sym == SDLK_t) {
				tempOFFON = !tempOFFON;
			}
			if (e.key.keysym.sym == SDLK_ESCAPE) {
				if (paused) {  
					resumeGame();
				}
				else if (inGame) {
					pauseGame();
				}
			}

			if (e.key.keysym.sym == SDLK_RETURN) {
				endGame();
			}
			if (e.key.keysym.sym == SDLK_F1) {
				//Toggle debug data
				showDebug = !showDebug;
			}
			break;
		}//End Case()
		case SDL_MOUSEBUTTONDOWN: {
			(this->*leftMouseClick)();
			break;
		}//End Case()
	}//End Switch()
	}//End while()
}

void Window::gameLoop() {
	int fps = (1000 / 30) - (timer - SDL_GetTicks()) -20;
	int puzzlesSolved = 0;
	int particlesLeft = 0;

	string fpsStr = "";
	string puzStr = "";
	string parStr = "";
	string scoStr = "";
	string timeStr = "";
	string cSpeedStr = "";

	if (world) {
		scoStr = "Score: " + to_string(world->getScore());
		parStr = "Particles: " + to_string(world->getParticlesLeft());
		puzStr = "Solved: " + to_string(world->getPuzzlesSolved());

		if (showDebug) {
			fpsStr = "FPS: " + to_string(fps_current);
			cSpeedStr = "Camera Speed: " + to_string(world->getCameraSpeed());
		}

		int t = world->getPuzzleTimeLeft();
		if (t >= 0) {
			t = ceil(t);
			timeStr = "Time Left: " + to_string(t);
		}
	}

	//Fps test end
	
	fps_frames++;
	if (fps_lasttime < SDL_GetTicks() - 1.0 * 1000)
	{
		fps_lasttime = SDL_GetTicks();
		fps_current = fps_frames;
		fps_frames = 0;
	}

	world->step(fps_current);	
	world->updateWorld();

	ren->render();
	ren->startRendering();


	Uint32 ticks = SDL_GetTicks();
	if (ticks >= lastUpdate + (1000.0 / WORLD_UPDATE_FPS)) {
		//Update at constant rate for camera and timers
		world->updateFixed();
		lastUpdate = ticks;
	}

	ren->mainLoop(fpsStr, puzStr, parStr, scoStr, timeStr, cSpeedStr);

	//SDL_Delay(fps);
	//Fps test start
	if (world->gameOver()) {
		endGame();
	}

}
void Window::gameLeftMouseClick() {
	world->applyForce(e.button.x, e.button.y);
}
void Window::menuLeftMouseClick() {
	switch (ren->menuMouseClickCheck(e.button.x, e.button.y)) {
		case 1: {
			//New game
			newGame();
			break;
		}
		case 2: {
			//Settings
			showSettings();
			break;
		}
		case 3: {
			//Highscore list
			showHighscores();
			break;
		}
		case 4: {
			//Quit
					
				//	ren->pushOrClearPopupMenu(0, e.button.x, e.button.y);
			running = false;
		
			break;
		}
		default:{
			break;
		}
	}
}
void Window::pauseLeftMouseClick() {
	switch (ren->pauseMouseClickCheck(e.button.x, e.button.y)) {
		case 1: {
			resumeGame();
			fps_lasttime = SDL_GetTicks();
			break;
		}
		case 2: {
			restartGame();
			break;
		}
		case 3: {
			exitGame();
			break;
		}
		default:{
			break;
		}
	}
}
void Window::scoreLeftMouseClick() {
	switch (ren->scoreMouseClickCheck(e.button.x, e.button.y)) {
		case 1: {
			//Retry
			scoreFinal = -1;
			scorePos = -1;
			restartGame();
			break;
		}
		case 2: {
			//Main menu
			scoreFinal = -1;
			scorePos = -1;
			exitGame();
			break;
		}
		default:{
			break;
		}
	}
}
void Window::settingsLeftMouseClick() {

	switch (ren->settingsMouseClickCheck(e.button.x, e.button.y)) {
		case 1: {
			//Music volume
			musicVolume += 5;
			if (musicVolume > 100) {
				musicVolume = 0;
			}
			Mix_VolumeMusic(musicVolume);
			break;
		}
		case 2: {
			//Effects volume
			effectVolume += 5;
			if (effectVolume > 100) {
				effectVolume = 0;
			}
			Mix_Volume(EFFECTS, effectVolume);
			break;
		}
		case 3: {
			//Main menu
			exitGame();
			break;
		}
		case 4: {
			//Wall color
			nextColor(r1, g1, b1, col1);
			break;
		}
		case 5: {
			//Orb color
			nextColor(r2, g2, b2, col2);
			ren->setLightColor(r2, g2, b2);
			break;
		}
		case 6: {
			//Randomize light
			r1 = randomRange(0, 1) * 255;
			g1 = randomRange(0, 1) * 255;
			b1 = randomRange(0, 1) * 255;
			r2 = randomRange(0, 1) * 255;
			g2 = randomRange(0, 1) * 255;
			b2 = randomRange(0, 1) * 255;
			nextColor(r1, g1, b1, col1);
			nextColor(r2, g2, b2, col2);
			ren->setLightColor(r2, g2, b2);
			break;
		}
		case 7: {
			//Gravity mode
			nextGravity();
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

}
void Window::pauseLoop(){
	string fpsStr = "FPS: " + to_string(fps_current);

	int fps = (1000 / 30) - (timer - SDL_GetTicks());
	//(this->*loopType)();
	ren->pauseLoop();
	//	SDL_Delay(fps);
	
}
void Window::scoreLoop() {
	string fpsStr = "FPS: " + to_string(fps_current);
	int fps = (1000 / 30) - (timer - SDL_GetTicks());

	ren->scoreLoop(highScores, scoreFinal, scorePos, inGame);

	timer = SDL_GetTicks();
	fps_frames++;

	if (fps_lasttime < SDL_GetTicks() - 1.0 * 1000) {
		fps_lasttime = SDL_GetTicks();
		fps_current = fps_frames;
		fps_frames = 0;
	}
}
void Window::settingsLoop() {
	string fpsStr = "FPS: " + to_string(fps_current);
	int fps = (1000 / 30) - (timer - SDL_GetTicks());

	ren->settingsLoop(musicVolume, effectVolume, col1, col2, grav);

	timer = SDL_GetTicks();
	fps_frames++;

	if (fps_lasttime < SDL_GetTicks() - 1.0 * 1000) {
		fps_lasttime = SDL_GetTicks();
		fps_current = fps_frames;
		fps_frames = 0;
	}
}
void Window::startWorld() {
	inGame = true;
	world = new World(ren->getInit()->getScreenWidth(), ren->getInit()->getScreenHeight(), ren->getPlatformVBO(), ren->getParticleVBO()
		, ren->getMainCharParticleVBO());
	//worldSimulation = new thread(&World::checkForInput, world);
	//world->setupWorld();
	world->setParticleColor(r1, g1, b1);
	world->setGravity(gravityX, gravityY);
}
void Window::buildMenu(){
	int screenW = ren->getInit()->getScreenWidth();
	int screenH = ren->getInit()->getScreenHeight();
	float scale = 1080.0 / screenH;
	int fontSizeOffsett = 250 / scale;

	int offX = screenW / 3;
	int offY = screenH / 20 + fontSizeOffsett;

	//Main menu objects
	ren->pushBackMenuObj(offX, offY * 0.75, "Play");
	ren->pushBackMenuObj(offX, offY * 1.1, "Settings");
	ren->pushBackMenuObj(offX, offY * 1.45, "Highscores");
	ren->pushBackMenuObj(offX, offY * 2.5, "Quit");
	ren->pushBackMenuObj(offX, offY * 0.25, "Energy Zone");

	//Pause objects
	ren->pushBackPauseObj(offX, offY * 0.75, "Resume");
	ren->pushBackPauseObj(offX, offY * 2.15, "Restart");
	ren->pushBackPauseObj(offX, offY * 2.5, "Main Menu");
	
	int scoreLeft = 80;
	int scoreRight = (screenW / 2) + 80;
	int scoreTop = offY * 0.5;

	//Highscore objects
	ren->pushBackScoreTxt(scoreRight + (offX * 0.5), scoreTop, "Score: 0");
	int highscoreY = scoreTop;
	ren->pushBackScoreTxt(scoreLeft, highscoreY, "HIGHSCORES");
	for (int i = 0; i < HIGHSCORES; i++) {
		ren->pushBackScoreTxt(scoreLeft, highscoreY + (offY * 0.3 * (i + 1)), "1: 1337");
	}
	ren->pushBackScoreBtn(scoreRight + (offX * 0.5), scoreTop + (offY * 0.3), "Retry?");
	ren->pushBackScoreBtn(scoreRight + (offX * 0.5), scoreTop + (offY * 2.5), "Main Menu");

	//Settings objects
	ren->pushBackSettingsBtn(scoreLeft, scoreTop + (offY * -0.25), "Music volume: 0");
	ren->pushBackSettingsBtn(scoreLeft, scoreTop + (offY * 0.05), "Effects volume: 0");
	ren->pushBackSettingsBtn(scoreRight + (offX * 0.5), scoreTop + (offY * 2.5), "Back");

	string c1 = "Orb Color: " + col1;
	string c2 = "Orb Light Color: " + col2;

	ren->pushBackSettingsBtn(scoreLeft, scoreTop + (offY * 0.95), c1);
	ren->pushBackSettingsBtn(scoreLeft, scoreTop + (offY * 1.25), c2);
	ren->pushBackSettingsBtn(scoreLeft, scoreTop + (offY * 1.55), "Randomize colors");

	ren->pushBackSettingsBtn(scoreLeft, scoreTop + (offY * 2.15), "Gravity: Off");
}
void Window::resumeGame() {
	paused = false;
	leftMouseClick = &Window::gameLeftMouseClick;
	loopType = &Window::gameLoop;
	Mix_ResumeMusic();
	currentMenu = MENU_NONE;
}
void Window::restartGame() {
	paused = false;
	delete world;
	newGame();
}
void Window::exitGame() {
	paused = false;
	inGame = false;
	if (world != NULL) {
		delete world;
		world = NULL;
	}
	leftMouseClick = &Window::menuLeftMouseClick;
	loopType = &Window::menuLoop;
	currentMenu = MENU_MAIN;
}
void Window::pauseGame() {
	paused = true;
	loopType = &Window::pauseLoop;
	leftMouseClick = &Window::pauseLeftMouseClick;
	Mix_PauseMusic();
	currentMenu = MENU_PAUSE;
}
void Window::showHighscores() {
	paused = true;
	loopType = &Window::scoreLoop;
	leftMouseClick = &Window::scoreLeftMouseClick;
	Mix_PauseMusic();
	currentMenu = MENU_HIGHSCORE;
}
void Window::newGame() {
	startWorld();
	leftMouseClick = &Window::gameLeftMouseClick;
	loopType = &Window::gameLoop;
	currentMenu = MENU_NONE;
	showDebug = false;
	lastUpdate = 0;
}
void Window::showSettings() {
	leftMouseClick = &Window::settingsLeftMouseClick;
	loopType = &Window::settingsLoop;
	currentMenu = MENU_SETTINGS;
}
void Window::endGame() {
	//Play death sound
	if (snd_Lost != NULL) {
		Mix_PlayChannel(EFFECTS, snd_Lost, 0);
	}

	//Get score
	scoreFinal = world->getScore();
	//Check if new highscore
	for (int i = 0; (i < HIGHSCORES && scorePos == -1); i++) {
		if (scoreFinal > highScores[i]) {
			scorePos = i;
			highScores.insert(highScores.begin() + i, scoreFinal);
			highScores.erase(highScores.begin() + HIGHSCORES);
		}
	}

	//Open highscore screen
	showHighscores();
}
void Window::saveData() {
	ofstream file(DATA_FILE);

	//Save settings
	file << "MusicVolume: " << musicVolume
		 << "\nEffectsVolume: " << effectVolume
		 << "\nOrbColor: " << r1 << " " << g1 << " " << b1
		 << "\nOrbLightColor: " << r2 << " " << g2 << " " << b2
		 << "\nGravity: " << gravityX << " " << gravityY << endl;

	//Save highscores
	file << "Highscores:\n";
	for (int i = 0; i < HIGHSCORES; i++) {
		file << highScores[i] << endl;
	}

	file.close();
}
void Window::loadData() {
	ifstream file(DATA_FILE);
	char buffer[64];
	string arg;

	if (file.is_open()) {
		while (!file.eof()) {
			file >> buffer;
			arg = buffer;

			if (arg == "MusicVolume:") {
				file >> musicVolume;
				Mix_VolumeMusic(musicVolume);
			}

			if (arg == "EffectsVolume:") {
				file >> effectVolume;
				Mix_Volume(EFFECTS, effectVolume);
			}

			if (arg == "OrbColor:") {
				file >> r1 >> g1 >> b1;
				nextColor(r1, g1, b1, col1, true);
			}

			if (arg == "OrbLightColor:") {
				file >> r2 >> g2 >> b2;
				nextColor(r2, g2, b2, col2, true);
				ren->setLightColor(r2, g2, b2);
			}

			if (arg == "Gravity:") {
				file >> gravityX >> gravityY;
				nextGravity(true);
			}

			if (arg == "Highscores:") {
				highScores.clear();
				for (int i = 0; i < HIGHSCORES; i++) {
					int newScore;
					file >> newScore;
					highScores.push_back(newScore);
				}
			}
		}
	}
}
void Window::nextColor(int &r, int &g, int &b, string &str, bool current) {
	if (r == 0 && g == 0 && b == 255) {
		if (current) {
			str = "Blue";
		}
		else {
			r = 0; g = 255; b = 0;
			str = "Green";
		}
	}
	else if (r == 0 && g == 255 && b == 0) {
		if (current) {
			str = "Green";
		}
		else {
			r = 255; g = 0; b = 0;
			str = "Red";
		}
	}
	else if (r == 255 && g == 0 && b == 0) {
		if (current) {
			str = "Red";
		}
		else {
			r = 0; g = 255; b = 255;
			str = "Cyan";
		}
	}
	else if (r == 0 && g == 255 && b == 255) {
		if (current) {
			str = "Cyan";
		}
		else {
			r = 255; g = 0; b = 255;
			str = "Magenta";
		}
	}
	else if (r == 255 && g == 0 && b == 255) {
		if (current) {
			str = "Magenta";
		}
		else {
			r = 255; g = 255; b = 0;
			str = "Yellow";
		}
	}
	else if (r == 255 && g == 255 && b == 0) {
		if (current) {
			str = "Yellow";
		}
		else {
			r = 255; g = 255; b = 255;
			str = "White";
		}
	}
	else {
		if (current) {
			str = "White";
		}
		else {
			r = 0; g = 0; b = 255;
			str = "Blue";
		}
	}
}
void Window::nextGravity(bool current) {
	if (gravityX == 0 && gravityY == 0) {
		if (current) {
			grav = "Off";
		}
		else {
			gravityX = 0; gravityY = 1;
			grav = "Up";
		}
	}
	else if (gravityX == 0 && gravityY == 1) {
		if (current) {
			grav = "Up";
		}
		else {
			gravityX = 0; gravityY = -1;
			grav = "Down";
		}
	}
	else if (gravityX == 0 && gravityY == -1) {
		if (current) {
			grav = "Down";
		}
		else {
			gravityX = -1; gravityY = 0;
			grav = "Left";
		}
	}
	else if (gravityX == -1 && gravityY == 0) {
		if (current) {
			grav = "Left";
		}
		else {
			gravityX = 1; gravityY = 0;
			grav = "Right";
		}
	}
	else {
		if (current) {
			grav = "Right";
		}
		else {
			gravityX = 0; gravityY = 0;
			grav = "Off";
		}
	}
}
