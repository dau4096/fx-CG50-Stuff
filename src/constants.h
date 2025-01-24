#ifndef CONSTANTS_H
#define CONSTANTS_H

enum Directions {
	INVALID = -1,
	UP, DOWN,
	LEFT, RIGHT
};

enum States {
	INACTIVE, ACTIVE,
	EATING,
	LOSE, WIN
};


struct vec2 {
	float x, y;

	constexpr vec2() : x(0.0f), y(0.0f) {}
	constexpr vec2(float x, float y)
		: x(x), y(y) {}
};

struct Snake {
	vec2 segments[252]; //21*12
	int length, state;
	Snake() : length(0), state(INVALID) {}
};


constexpr vec2 gridSize = vec2(21, 12);
constexpr bool debugSprites = false;

#endif