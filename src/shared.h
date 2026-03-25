/* shared.h */
#ifndef SHARED_H
#define SHARED_H


#include <stdint.h>



//// CONSTANTS ////
//Maths
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



//// STRUCTS AND TYPES ////
//Fixed point "type"
typedef int32_t fixed_t;
#define FIX_SHIFT 16 								  /* Number of decimal bits */
#define FIX_ZERO 0x00000000							  /* Zero in fixed-point */
#define FIX_ONE   (1 << FIX_SHIFT)					  /* One in fixed-point */

#define INT_TO_FIX(x)   ((fixed_t)((x) << FIX_SHIFT)) /* Integer to fixed-point */
#define FIX_TO_INT(x)   ((int)((x) >> FIX_SHIFT))     /* Fixed-point to integer */

#define FLOAT_TO_FIX(x) ((fixed_t)((x) * FIX_ONE))    /* Floating-point to fixed-point */
#define FIX_TO_FLOAT(x) ((float)(x) / FIX_ONE)		  /* Fixed-point to floating-point */


#define EPSILON FLOAT_TO_FIX(1e-5f)


//2D positions
typedef struct {fixed_t x, y;} Vec2_t;
static Vec2_t createVec2_t(fixed_t x, fixed_t y) {
	Vec2_t v;
	v.x=x; v.y=y;
	return v;
}
static Vec2_t emptyVec2_t() {return createVec2_t(FIX_ZERO, FIX_ZERO);}

//3D positions
typedef struct {fixed_t x, y, z;} Vec3_t;
static Vec3_t createVec3_t(fixed_t x, fixed_t y, fixed_t z) {
	Vec3_t v;
	v.x=x; v.y=y; v.z=z;
	return v;
}
static Vec3_t emptyVec3_t() {return createVec3_t(FIX_ZERO, FIX_ZERO, FIX_ZERO);}


typedef struct {
	unsigned int vStart; //Start vertex ID
	unsigned int vEnd;   //End vertex ID
	int frontSector;  //Sector ID this LineDef_t belongs to
	int backSector;   //-1 if solid wall, else index of neighboring sector
} LineDef_t;

typedef struct {
	fixed_t floorHeight;
	fixed_t ceilingHeight;
	unsigned int* lineDefs;   //array of IDs to Linedef_t[] array (like model indices)
	unsigned int numLineDefs; //Length of ID array
} Sector_t;

typedef struct {
	Vec3_t position; //3D Position
	int yaw; //View yaw angle
	int FOV; //Field of view, Degrees
	fixed_t maxDistance; //Maximum view distance
} Camera_t;
//// STRUCTS AND TYPES ////




#endif