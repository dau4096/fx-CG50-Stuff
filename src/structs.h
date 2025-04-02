#ifndef STRUCTS_H
#define STRUCTS_H

extern "C" {
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
}

#include <stdlib.h>
#include <stdint.h>



enum States {
	S_INACTIVE, S_ACTIVE,
	S_EATING,
	S_LOSE, S_WIN,
};


struct vec2 {
	int x, y;

	constexpr vec2() : x(0.0f), y(0.0f) {}
	constexpr vec2(float x, float y)
		: x(static_cast<int>(x)), y(static_cast<int>(y)) {}
};


struct vec3 {
	float x, y, z;

	constexpr vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	constexpr vec3(float x, float y, float z)
		: x(x), y(y), z(z) {}
};


struct Snake {
	vec2 segments[21 * 12]; //21*12
	int length;
	States state;
	Snake() : length(0), state(S_INACTIVE) {}
};


#endif