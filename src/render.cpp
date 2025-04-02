extern "C" {
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
}

#include <stdint.h>
#include "structs.h"
#include "constants.h"
#include "utils.h"
using namespace std;
using namespace utils;

//Direction - 0: Up, 1: Down, 2: Left, 3: Right
//Textures/Graphics should fit within 17x17px
//Grid is 21*12


namespace render{




inline void  drawVLine(int x, int y, int length, unsigned short colour) {
	for (int offset = 0; offset <= length; offset++) {
		drawPixel(x, y + offset, colour);
	}
}

inline void  drawHLine(int x, int y, int length, unsigned short colour) {
	for (int offset = 0; offset <= length; offset++) {
		drawPixel(x + offset, y, colour);
	}
}

inline vec2  pixelPos(vec2 gridPosition) {
	return vec2(gridPosition.x * 17, (gridPosition.y * 17)); //24px vertical offset for top-bar.
}

void head(vec2 position, int direction) {
	vec2 screenPosition = pixelPos(position);
	unsigned short red = 0xC800, black = 0x0000, green = 0x04A0, blue = 0x051C;


	switch (direction) {
		case D_UP:
			drawVLine(screenPosition.x+3, screenPosition.y+8, 5, green);
			drawVLine(screenPosition.x+4, screenPosition.y+5, 9, green);
			for (int x = 0; x < 7; x++) {
				drawVLine(screenPosition.x+5+x, screenPosition.y+4, 10, green);
			}
			drawVLine(screenPosition.x+12, screenPosition.y+5, 9, green);
			drawVLine(screenPosition.x+13, screenPosition.y+8, 5, green);
			

			//Deets :)
			drawVLine(screenPosition.x+8, screenPosition.y+2, 1, red);
			drawPixel(screenPosition.x+7, screenPosition.y+1, red);
			drawPixel(screenPosition.x+9, screenPosition.y+1, red);
			drawPixel(screenPosition.x+6, screenPosition.y+8, black);
			drawPixel(screenPosition.x+10, screenPosition.y+8, black);
			break;


		case D_DOWN:
			drawVLine(screenPosition.x+3, screenPosition.y+4, 5, green);
			drawVLine(screenPosition.x+4, screenPosition.y+3, 9, green);
			for (int x = 0; x < 7; x++) {
				drawVLine(screenPosition.x+5+x, screenPosition.y+3, 10, green);
			}
			drawVLine(screenPosition.x+12, screenPosition.y+3, 9, green);
			drawVLine(screenPosition.x+13, screenPosition.y+4, 5, green);
			

			//Deets :)
			drawVLine(screenPosition.x+8, screenPosition.y+14, 1, red);
			drawPixel(screenPosition.x+7, screenPosition.y+16, red);
			drawPixel(screenPosition.x+9, screenPosition.y+16, red);
			drawPixel(screenPosition.x+6, screenPosition.y+9, black);
			drawPixel(screenPosition.x+10, screenPosition.y+9, black);
			break;


		case D_LEFT:
			drawHLine(screenPosition.x+7, screenPosition.y+3, 5, green);
			drawHLine(screenPosition.x+4, screenPosition.y+4, 9, green);
			for (int y = 0; y < 7; y++) {
				drawHLine(screenPosition.x+3, screenPosition.y+5+y, 10, green);
			}
			drawHLine(screenPosition.x+4, screenPosition.y+12, 9, green);
			drawHLine(screenPosition.x+7, screenPosition.y+13, 5, green);
			

			//Deets :)
			drawHLine(screenPosition.x+1, screenPosition.y+8, 1, red);
			drawPixel(screenPosition.x, screenPosition.y+9, red);
			drawPixel(screenPosition.x, screenPosition.y+7, red);
			drawPixel(screenPosition.x+8, screenPosition.y+6, black);
			drawPixel(screenPosition.x+8, screenPosition.y+10, black);
			break;


		case D_RIGHT:
			drawHLine(screenPosition.x+4, screenPosition.y+3, 5, green);
			drawHLine(screenPosition.x+3, screenPosition.y+4, 9, green);
			for (int y = 0; y < 7; y++) {
				drawHLine(screenPosition.x+3, screenPosition.y+5+y, 10, green);
			}
			drawHLine(screenPosition.x+3, screenPosition.y+12, 9, green);
			drawHLine(screenPosition.x+4, screenPosition.y+13, 5, green);
			

			//Deets :)
			drawHLine(screenPosition.x+14, screenPosition.y+8, 1, red);
			drawPixel(screenPosition.x+16, screenPosition.y+9, red);
			drawPixel(screenPosition.x+16, screenPosition.y+7, red);
			drawPixel(screenPosition.x+9, screenPosition.y+6, black);
			drawPixel(screenPosition.x+9, screenPosition.y+10, black);
			break;



	}
}



void drawDir(vec2 screenPosition, int direction, unsigned short colour) {
	//Direction to connect to (up = connect on top side of tile)
	switch (direction) {
		case D_UP:
			drawVLine(screenPosition.x + 8, screenPosition.y+5, 11, colour);
			for (int x = 0; x < 3; x++) {
				drawVLine(screenPosition.x+x+5, screenPosition.y+6+(2-x), (8+x), colour);
				drawVLine(screenPosition.x+x+9, screenPosition.y+6+x, 8+(2-x), colour);
			}
			break;


		case D_DOWN:
			drawVLine(screenPosition.x + 8, screenPosition.y, 11, colour);
			for (int x = 0; x < 3; x++) {
				drawVLine(screenPosition.x+x+5, screenPosition.y, 8+x, colour);
				drawVLine(screenPosition.x+x+9, screenPosition.y, 8+(2-x), colour);
			}
			break;


		case D_LEFT:
			drawHLine(screenPosition.x, screenPosition.y+8, 11, colour);
			for (int y = 0; y < 3; y++) {
				drawHLine(screenPosition.x, screenPosition.y+y+5, 8+y, colour);
				drawHLine(screenPosition.x, screenPosition.y+y+9, 8+(2-y), colour);
			}
			break;


		case D_RIGHT:
			drawHLine(screenPosition.x+5, screenPosition.y+8, 11, colour);
			for (int y = 0; y < 3; y++) {
				drawHLine(screenPosition.x+6+(2-y), screenPosition.y+y+5, (8+y), colour);
				drawHLine(screenPosition.x+6+y, screenPosition.y+y+9, 8+(2-y), colour);
			}
			break;

	}
}


void body(vec2 position, int startDir, int endDir, int idx) { //Draw body connecting 2 directions.
	vec2 screenPosition = pixelPos(position);
	unsigned short colour = (idx % 2 == 0) ? 0x0640 : 0x04A0; //Alternating shades of green for visual interest

	drawDir(screenPosition, startDir, colour);
	drawDir(screenPosition, endDir, colour);
}

void tail(vec2 position, int direction, int idx) {
	vec2 screenPosition = pixelPos(position);
	unsigned short colour = (idx % 2 == 0) ? 0x0640 : 0x04A0;

	int lengths[7] = {
		5, 9, 10, 10, 10, 9, 5
	};

	switch (direction) {
		case D_UP:
			for (int x = 0; x < 7; x++) {
				drawVLine(screenPosition.x+5+x, screenPosition.y, lengths[x], colour);
			}
			break;

		case D_DOWN:
			for (int x = 0; x < 7; x++) {
				drawVLine(screenPosition.x+5+x, screenPosition.y+16-lengths[x], lengths[x], colour);
			}
			break;

		case D_LEFT:
			for (int y = 0; y < 7; y++) {
				drawHLine(screenPosition.x+16-lengths[y], screenPosition.y+5+y, lengths[y], colour);
			}
			break;

		case D_RIGHT:
			for (int y = 0; y < 7; y++) {
				drawHLine(screenPosition.x, screenPosition.y+5+y, lengths[y], colour);
			}
			break;
	}
}



inline int  getDir(vec2 thisSegment, vec2 otherSegment) {
    if (thisSegment.y < otherSegment.y) { // Connect Up
        return D_UP;
    } else if (thisSegment.y > otherSegment.y) { // Connect Down
        return D_DOWN;
    } else if (thisSegment.x > otherSegment.x) { // Connect Left
        return D_LEFT;
    } else if (thisSegment.x < otherSegment.x) { // Connect Right
        return D_RIGHT;
    } else if (thisSegment.x == 0 && otherSegment.x == 20) { // X overlap, via right
        return D_LEFT;
    } else if (thisSegment.x == 20 && otherSegment.x == 0) { // Other direction X
        return D_RIGHT;
    } else if (thisSegment.y == 0 && otherSegment.y == 20) { // Y overlap, via bottom
        return D_UP;
    } else if (thisSegment.y == 20 && otherSegment.y == 0) { // Other direction Y
        return D_DOWN;
    } else {
        return D_INVALID;
    }
}



void drawApple(vec2 applePos) { //Apple graphic here.
	vec2 screenPosition = pixelPos(applePos);
	unsigned short green = 0x04A0, brown = 0xBBCA, darkOrange = 0xFBE4, lightOrange = 0xFE41;

	//Leaf
	drawPixel(screenPosition.x+4, screenPosition.y+2, green);
	drawVLine(screenPosition.x+5, screenPosition.y+1, 1, green);
	drawVLine(screenPosition.x+6, screenPosition.y+1, 1, green);
	drawPixel(screenPosition.x+7, screenPosition.y+2, green);

	//Stalk
	drawVLine(screenPosition.x+8, screenPosition.y+2, 1, brown);
	drawPixel(screenPosition.x+9, screenPosition.y+1, brown);

	//Shine
	drawHLine(screenPosition.x+4, screenPosition.y+8, 1, lightOrange);
	drawPixel(screenPosition.x+5, screenPosition.y+7, lightOrange);
	drawPixel(screenPosition.x+6, screenPosition.y+6, lightOrange);

	//Circle
	drawVLine(screenPosition.x+2, screenPosition.y+8, 2, darkOrange);
	drawVLine(screenPosition.x+3, screenPosition.y+6, 6, darkOrange);
	drawVLine(screenPosition.x+4, screenPosition.y+9, 4, darkOrange);
	drawVLine(screenPosition.x+5, screenPosition.y+9, 4, darkOrange);
	drawVLine(screenPosition.x+6, screenPosition.y+8, 6, darkOrange);
	drawVLine(screenPosition.x+7, screenPosition.y+7, 7, darkOrange);
	drawPixel(screenPosition.x+4, screenPosition.y+6, darkOrange);
	drawHLine(screenPosition.x+4, screenPosition.y+5, 3, darkOrange);
	drawHLine(screenPosition.x+6, screenPosition.y+4, 1, darkOrange);
	drawVLine(screenPosition.x+8, screenPosition.y+4, 10, darkOrange);
	drawVLine(screenPosition.x+9, screenPosition.y+4, 10, darkOrange);
	drawVLine(screenPosition.x+10, screenPosition.y+4, 10, darkOrange);
	drawVLine(screenPosition.x+11, screenPosition.y+5, 8, darkOrange);
	drawVLine(screenPosition.x+12, screenPosition.y+5, 8, darkOrange);
	drawVLine(screenPosition.x+13, screenPosition.y+6, 6, darkOrange);
	drawVLine(screenPosition.x+14, screenPosition.y+8, 2, darkOrange);
}



void drawSnake(Snake snakeInstance, int direction) {
	/*
	if (false) { //Test sprites.
		//Head Row
		head(vec2(0, 0), UP);
		head(vec2(1, 0), DOWN);
		head(vec2(2, 0), LEFT);
		head(vec2(3, 0), RIGHT);

		//"Apple" (Orange)
		drawApple(vec2(4, 0));

		//Tail Row
		tail(vec2(0, 1), UP, 0);
		tail(vec2(1, 1), DOWN, 0);
		tail(vec2(2, 1), LEFT, 0);
		tail(vec2(3, 1), RIGHT, 0);

		//Directions
		drawDir(pixelPos(vec2(0, 2)), UP, 0x0640);
		drawDir(pixelPos(vec2(1, 2)), DOWN, 0x0640);
		drawDir(pixelPos(vec2(2, 2)), LEFT, 0x0640);
		drawDir(pixelPos(vec2(3, 2)), RIGHT, 0x0640);

		//Body Permutations (IDX0)
		body(vec2(0, 3), UP, DOWN, 0);
		body(vec2(1, 3), UP, LEFT, 0);
		body(vec2(2, 3), UP, RIGHT, 0);
		body(vec2(3, 3), DOWN, LEFT, 0);
		body(vec2(4, 3), DOWN, RIGHT, 0);
		body(vec2(5, 3), LEFT, RIGHT, 0);
		//Body Permutations (IDX1)
		body(vec2(0, 4), UP, DOWN, 1);
		body(vec2(1, 4), UP, LEFT, 1);
		body(vec2(2, 4), UP, RIGHT, 1);
		body(vec2(3, 4), DOWN, LEFT, 1);
		body(vec2(4, 4), DOWN, RIGHT, 1);
		body(vec2(5, 4), LEFT, RIGHT, 1);
		return;
	}
	*/


	//Draw given snakeInstance.
	int headDir;
	if (snakeInstance.length <= 1) {
		switch (direction) { //Opposite direction.
			case D_UP:
				headDir = D_DOWN;
			case D_DOWN:
				headDir = D_UP;
			case D_LEFT:
				headDir = D_RIGHT;
			case D_RIGHT:
				headDir = D_LEFT;
			default:
				headDir = D_INVALID;
		}
	} else {
		headDir = getDir(snakeInstance.segments[0], snakeInstance.segments[1]);
		if (headDir == D_INVALID) return; //HeadDir is invalid somehow.
	}
	drawDir(pixelPos(snakeInstance.segments[0]), headDir, 0x0640);
	head(snakeInstance.segments[0], direction);

	if (snakeInstance.length <= 1) {return;} //Only head draws only head.

	for (int idx = 1; idx < snakeInstance.length-1; idx++) {
		if (snakeInstance.length <= 2) {break;}
		vec2 segmentPos = snakeInstance.segments[idx];
		vec2 prevPos = snakeInstance.segments[idx-1];
		if (idx+1 >= snakeInstance.length) break;
		vec2 nextPos = snakeInstance.segments[idx+1];

		int startDir = getDir(segmentPos, prevPos);
		int endDir = getDir(segmentPos, nextPos);
		if (startDir == D_INVALID || endDir == D_INVALID) continue; //Overlapping somehow, should never happen. Proceed to next segment.

		body(segmentPos, startDir, endDir, idx);
	}

	vec2 tailPos = snakeInstance.segments[snakeInstance.length - 1];
	vec2 beforeTailPos = snakeInstance.segments[snakeInstance.length - 2];

	int tailDir = getDir(beforeTailPos, tailPos);
	if (tailDir == D_INVALID) return; //Invalid in some way, should never happen. Just exit if so.
	tail(tailPos, tailDir, snakeInstance.length-1);
}






void loseScreen(int score) {
	//Shows score (length) too.
	printTXT("  You crashed! :(", 1);
	//printINT(score); //DO NOT USE. CRASHES.
}

void winScreen(int score) {
	//Win screen! Hard to get..
	printTXT("  You win! :)", 1);
	//printINT(score); //DO NOT USE. CRASHES.
}

}