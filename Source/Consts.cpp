#include "Consts.h"



int randomRange(int min, int max) {
	return (min + (rand() % (max - min + 1)));
}