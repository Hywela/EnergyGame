#pragma once
#include <random>
#include <Box2D\Box2D.h>
#include <SDL.h>
#include <string>
using namespace std;



//Box2D variables
const float M2P = 30;
const float P2M = 1 / M2P;
//Meny const
const SDL_Color MENU_COLOR_HOVER = { 255, 255, 0 };
const SDL_Color MENU_COLOR_NORMAL = { 0, 255, 255 };
const SDL_Color HIGHSCORE_COLOR_NORMAL = { 255, 255, 255 };
const SDL_Color HIGHSCORE_COLOR_NEW = { 0, 220, 0 };
enum menu {MENU_NONE, MENU_MAIN, MENU_PAUSE, MENU_SETTINGS, MENU_HIGHSCORE};
//World variables
const b2Vec3 COLOR_SOLID = b2Vec3(0.7, 0.7, 0.7);
const b2Vec3 COLOR_UNLIT = b2Vec3(0.7, 0, 0);
const b2Vec3 COLOR_LIT = b2Vec3(0, 1, 0);
const b2Vec3 COLOR_BLACK = b2Vec3(0.1, 0.1, 0.1);
const int ROOF = 0;
const int FLOOR = 600;
const int WALLSIZE = 10;
const float START_CAMERASPEED = 2;
const float CAMERASPEED_INCREASE = 0;
const int START_WALLS = 10;
const int WALL_INCREASE = 4;
const int WALL_DISTANCE = 100;
const int START_PARTICLES = 20;
const int INCREDIBLY_LONG = 10000000;
const int DEATH_WALL_SPACE = 100;
const int PICKUP_RANGE = 50;
const int DEATHWALL_SIZE = 30;
const int CAMERA_SPEEDUP_RANGE = 800;
const int HIGHSCORES = 5;
//Audio variables
const int AUDIO_RATE = 22050;
const Uint16 AUDIO_FORMAT = AUDIO_S16; /* 16-bit stereo */
enum SOUND_CHANNEL {BG_MUSIC, EFFECTS, AUDIO_CHANNELS};
const int AUDIO_BUFFERS = 2048;
const string DIR_MUSIC = "Audio/Music/";
const string DIR_EFFECTS = "Audio/Effects/";
const string DATA_FILE = "GameData.txt";

int randomRange(int min, int max);