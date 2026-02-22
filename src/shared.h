/* shared.h */
#ifndef SHARED_H
#define SHARED_H





//// CONSTANTS ////
//Maths
#define EPSILON 1e-5f
#define PI 3.141597

//Logic
#define TRUE 1
#define FALSE 0

//Misc
#define MAX_WALLS 8u
//// CONSTANTS ////



//// STRUCTS ////
//2D positions
typedef struct {float x, y;} Vec2_t;
inline Vec2_t createVec2_t(float x, float y) {
	Vec2_t v;
	v.x=x; v.y=y;
	return v;
}
inline Vec2_t emptyVec2_t() {return createVec2_t(0.0f, 0.0f);}

//3D positions
typedef struct {float x, y, z;} Vec3_t;
inline Vec3_t createVec3_t(float x, float y, float z) {
	Vec3_t v;
	v.x=x; v.y=y; v.z=z;
	return v;
}
inline Vec3_t emptyVec3_t() {return createVec3_t(0.0f, 0.0f, 0.0f);}


typedef struct {
	Vec2_t start, dir, end;
	float length;
} Line_t;

typedef struct {
	Vec2_t start, end;
	Vec3_t colour;
	int valid;
} Wall_t;

typedef struct {
	Vec2_t position; //2D Position
	int yaw; //View yaw angle
	int FOV; //Field of view, Degrees
	float maxDistance; //Maximum view distance
} Camera_t;
//// STRUCTS ////




#endif