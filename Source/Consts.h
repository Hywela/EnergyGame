#pragma once
#include <random>
#include <Box2D\Box2D.h>



const float M2P = 30;
const float P2M = 1 / M2P;
const b2Vec3 COLOR_SOLID = b2Vec3(0.7, 0.7, 0.7);
const b2Vec3 COLOR_UNLIT = b2Vec3(0.7, 0, 0);
const b2Vec3 COLOR_LIT = b2Vec3(0, 1, 0);
const b2Vec3 COLOR_BLACK = b2Vec3(0.1, 0.1, 0.1);

int randomRange(int min, int max);