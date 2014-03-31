#include "Window.h"

Window::Window() {

	running = true;
	isFullscreen = false;
	paused = false;
	leftMouseClick = &Window::menuLeftMouseClick;
	loopType = &Window::menuLoop;

	int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;// | SDL_WINDOW_FULLSCREEN;

	ren = new Render(new Init( flags));

	buildMenu();

	//fps test variables
	fps_lasttime = SDL_GetTicks(); //the last recorded time.
	fps_current = 0; //the current FPS.
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
			if (inGame)
				ren->setMousePointLigth(e.button.x, e.button.y);
			if (currentMenu) {
				ren->menuMouseHoverCheck(e.button.x, e.button.y, currentMenu);
			}
			break;
		}//End Case()
		case SDL_KEYDOWN: {
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

	if (world) {
		fpsStr = "FPS: " + to_string(fps_current);
		puzStr = "Solved: " + to_string(world->getPuzzlesSolved());
		parStr = "Particles: " + to_string(world->getParticlesLeft());
		scoStr = "Score: " + to_string(world->getScore());
	}

	world->step();

	ren->render();
	ren->startRendering();
	world->updateWorld();
	ren->mainLoop(fpsStr, puzStr, parStr, scoStr);

	SDL_Delay(fps);
	//Fps test start
	fps_frames++;
	if (fps_lasttime < SDL_GetTicks() - 1.0 * 1000)
	{
		fps_lasttime = SDL_GetTicks();
		fps_current = fps_frames;
		fps_frames = 0;
	}
	//Fps test end

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
	//	SDL_Delay(fps);
	timer = SDL_GetTicks();

	fps_frames++;
	if (fps_lasttime < SDL_GetTicks() - 1.0 * 1000)
	{
		fps_lasttime = SDL_GetTicks();
		fps_current = fps_frames;
		fps_frames = 0;
	}
}
void Window::pauseLoop(){
	string fpsStr = "FPS: " + to_string(fps_current);

	int fps = (1000 / 30) - (timer - SDL_GetTicks());
	//(this->*loopType)();
	ren->pauseLoop();
	//	SDL_Delay(fps);
	timer = SDL_GetTicks();

	fps_frames++;
	if (fps_lasttime < SDL_GetTicks() - 1.0 * 1000)
	{
		fps_lasttime = SDL_GetTicks();
		fps_current = fps_frames;
		fps_frames = 0;
	}
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

	ren->settingsLoop(musicVolume, effectVolume);

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
}
void Window::buildMenu(){
	int fontSizeOffsett = 250;
	int screenW = ren->getInit()->getScreenWidth()/3;
	int screenH = ren->getInit()->getScreenHeight() / 20 + fontSizeOffsett;

	//Main menu objects
	ren->pushBackMenuObj(screenW, screenH*0.5, "Play");
	ren->pushBackMenuObj(screenW, screenH*1, "Settings");
	ren->pushBackMenuObj(screenW, screenH*1.5, "Highscores");
	ren->pushBackMenuObj(screenW, screenH*2.5, "Quit");

	//Pause objects
	ren->pushBackPauseObj(screenW, screenH*0.5, "Resume");
	ren->pushBackPauseObj(screenW, screenH * 1, "Restart");
	ren->pushBackPauseObj(screenW, screenH*1.5, "Main Menu");
	
	int scoreLeft = 80;
	int scoreRight = ren->getInit()->getScreenWidth() / 2 + 80;
	int scoreTop = screenH * 0.5;

	//Highscore objects
	ren->pushBackScoreTxt(scoreRight, scoreTop, "Score: 0");
	int highscoreY = scoreTop;
	ren->pushBackScoreTxt(scoreLeft, highscoreY, "HIGHSCORES");
	for (int i = 0; i < HIGHSCORES; i++) {
		ren->pushBackScoreTxt(scoreLeft, highscoreY + (screenH * 0.4 * (i + 1)), "1: 1337");
	}
	ren->pushBackScoreBtn(scoreRight, scoreTop + (screenH * 1.5), "Retry?");
	ren->pushBackScoreBtn(scoreRight, scoreTop + (screenH * 2), "Main Menu");

	//Settings objects
	ren->pushBackSettingsBtn(scoreLeft, scoreTop, "Music volume: 0");
	ren->pushBackSettingsBtn(scoreLeft, scoreTop + (screenH * 0.5), "Effects volume: 0");
	ren->pushBackSettingsBtn(scoreRight + (screenW * 0.75), screenH * 2.5, "Back");



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
}
void Window::showSettings() {
	leftMouseClick = &Window::settingsLeftMouseClick;
	loopType = &Window::settingsLoop;
	currentMenu = MENU_SETTINGS;
}
void Window::endGame() {
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
		<< "\nEffectsVolume: " << effectVolume << endl;

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

	if (file) {
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
