#ifndef STRUCTS_H
#define STRUCTS_H

struct vec2 {
	float x, y;

	constexpr vec2() : x(0.0f), y(0.0f) {}
	constexpr vec2(float x, float y)
		: x(x), y(y) {}
};

struct vec3 {
	float x, y, z;

	constexpr vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	constexpr vec3(float x, float y, float z)
		: x(x), y(y), z(z) {}
};

#endif