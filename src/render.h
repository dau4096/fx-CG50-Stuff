#ifndef RENDER_H
#define RENDER_H

extern "C" {
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
}

#include <stdlib.h>
#include <stdint.h>
#include "structs.h"
#include "constants.h"
using namespace std;
using namespace utils;


namespace render {

void drawSnake(Snake snakeInstance, int direction);
void drawApple(vec2 applePos);

void loseScreen(int score);
void winScreen(int score);

void drawVLine(int x, int y, int length, unsigned short colour);
void drawHLine(int x, int y, int length, unsigned short colour);

}

#endif