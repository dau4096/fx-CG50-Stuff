#ifndef UTILS_H
#define UTILS_H

#include "constants.h"



namespace utils {


Snake createSnake(vec2 startPoint);
Snake movement(Snake snakeInstance, int direction, vec2 applePos);

int randomInRange(int min, int max);

}

#endif