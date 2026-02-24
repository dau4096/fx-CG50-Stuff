/* shared.h */
#ifndef SHARED_H
#define SHARED_H





//// CONSTANTS ////
//Maths
#define EPSILON 1e-5f
#define PI 3.141597


//Memory
#define SINE_LUT_SIZE 90
#define ATAN_LUT_BITS 6
#define ATAN_LUT_SIZE 64

#define MAX_VERTICES 128u
#define MAX_LINEDEFS 64u
#define MAX_SECTORS 16u


//Logic
#define TRUE 1
#define FALSE 0


//Misc
#define TURN_SPD 5     /* 5 deg per turn */
#define MOVE_SPD 0.25f /* 0.25 units per press */
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
	unsigned int vStart; //Start vertex ID
	unsigned int vEnd;   //End vertex ID
	int frontSector;  //Sector ID this LineDef_t belongs to
	int backSector;   //-1 if solid wall, else index of neighboring sector
} LineDef_t;

typedef struct {
	float floorHeight;
	float ceilingHeight;
	unsigned int* lineDefs;   //array of IDs to Linedef_t[] array (like model indices)
	unsigned int numLineDefs; //Length of ID array
} Sector_t;

typedef struct {
	Vec3_t position; //3D Position
	int yaw; //View yaw angle
	int FOV; //Field of view, Degrees
	float maxDistance; //Maximum view distance
} Camera_t;
//// STRUCTS ////




#endif