extern "C" {
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
}

#include <stdlib.h>
#include "constants.h"


namespace utils {

//Only for utils::movement() to use.
Snake shiftSegments(Snake snakeInstance) {
	int offset = 1;
	if (snakeInstance.state == EATING) {
		offset = 2; //If just eaten an apple;
		snakeInstance.state = ACTIVE;
	}

	for (int idx = snakeInstance.length-offset; idx > 0; idx++) {
		snakeInstance.segments[idx+1] = snakeInstance.segments[idx];
	}

	return snakeInstance;
}


float fmod(float numerator, float divisor) {
	if (divisor == 0.0f) return 0.0f;
	int divis = static_cast<int>(numerator / divisor);
	return numerator - (divisor * divis);
}


Snake createSnake(vec2 startPoint) {
	Snake snakeInstance = Snake();
	snakeInstance.segments[0] = startPoint;
	snakeInstance.length = 1;
	snakeInstance.state = ACTIVE; //Active snake.
	return snakeInstance;
}

Snake movement(Snake snakeInstance, int direction, vec2 applePos) {
	vec2 headPos = snakeInstance.segments[0];
	snakeInstance = shiftSegments(snakeInstance);
	if (snakeInstance.length == 252) {
		snakeInstance.state = WIN; //You win :)
		return snakeInstance;
	}

	switch (direction) {
		case UP:
			headPos.y += 1;
			break;
		case DOWN:
			headPos.y -= 1;
			break;
		case LEFT:
			headPos.x -= 1;
			break;
		case RIGHT:
			headPos.x += 1;
			break;
	}


	//Allow moving off the edges to the other side
	headPos.x = fmod((headPos.x + gridSize.x), gridSize.x);
	headPos.y = fmod((headPos.y + gridSize.y), gridSize.y);


	for (int idx = 1; idx < snakeInstance.length; idx++) {
		vec2 thisSegmentPos = snakeInstance.segments[idx];
		if (headPos.x == thisSegmentPos.x && headPos.y == thisSegmentPos.y) {
			snakeInstance.state = LOSE; //You crashed :(
			return snakeInstance;
		}
	}


	snakeInstance.segments[0] = headPos;
	if (headPos.x == applePos.x && headPos.y == applePos.y) {
		snakeInstance.length += 1;
		snakeInstance.state = EATING; //Eating
		snakeInstance.segments[snakeInstance.length-1] = snakeInstance.segments[snakeInstance.length-2];
	}

	return snakeInstance;
}


int randomInRange(int min, int max) {
	return min + rand() % (max - min + 1);
}



}