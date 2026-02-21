#ifndef UTILS_H
#define UTILS_H


#include <stdint.h>
#include <fxcg/display.h>


#include "trigLUT.h" //Contains the trig L.U.T. for integer degrees 0-90*




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


typedef struct {Vec2_t start, dir, end;} Line_t;

typedef struct {
	Vec2_t start, end;
	color_t colour;
	int valid;
} Wall_t;

typedef struct {
	Vec2_t position; //2D Position
	float yaw; //View yaw angle
	float FOV; //Field of view, Degrees
	float maxDistance; //Maximum view distance
} Camera_t;
//// STRUCTS ////




//// GENERAL MATHS ////
//Fast inverse square root (Quake III Arena), but with added inline directive and renamed to f_invsqrt to fit naming scheme.
inline float f_invsqrt( float number )
{
	int32_t i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( int32_t * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}


//Inlined because they're so simplistic.
//Int funcs
inline int i_abs(int v) {return (v>0.0f) ? v : -v;}
inline int i_min(int a, int b) {return (a<b) ? a : b;}
inline int i_max(int a, int b) {return (a>b) ? a : b;}
inline int i_clamp(int v, int mi, int ma) {return i_max(mi, i_min(v, ma));}
inline int i_sign(int v) {return (v>0.0f) - (v<0.0f); /* Using strange C-"bool" maths. 1 if >0, 0 if =0, -1 if <0. */}

//Float funcs
inline float f_abs(float v) {return (v>0.0f) ? v : -v;}
inline float f_min(float a, float b) {return (a<b) ? a : b;}
inline float f_max(float a, float b) {return (a>b) ? a : b;}
inline float f_clamp(float v, float mi, float ma) {return f_max(mi, f_min(v, ma));}
inline int f_sign(float v) {return (v>0.0f) - (v<0.0f); /* Using strange C-"bool" maths. 1 if >0, 0 if =0, -1 if <0. */}
inline float f_floor(float v) {
	int i = (int)(v);
	return ((v < 0.0f) && (v != (float)(i))) ? (float)(i - 1) : (float)i;
}
inline float f_ceil(float v) {
	int i = (int)(v);
	return ((v > 0.0f) && (v != (float)(i))) ? (float)(i + 1) : (float)i;
}
inline float f_round(float v) {return f_floor(v + 0.5f);}
inline float f_fract(float v) {return v - f_floor(v);}
inline float f_sqrt(float v) {
	if (v < EPSILON) {return 0.0f; /* No 0/negative values. */}
	return v * f_invsqrt(v); //x * x^-½ = x^½. It works out. Still looks a bit odd though.
}


//Linear interps
inline float f_lerp(float a, float b, float t) {return a + t*(b-a);}
Vec2_t v2_lerp(Vec2_t a, Vec2_t b, float t) {
	return createVec2_t(
		f_lerp(a.x, b.x, t), f_lerp(a.y, b.y, t)
	);
}
Vec3_t v3_lerp(Vec3_t a, Vec3_t b, float t) {
	return createVec3_t(
		f_lerp(a.x, b.x, t), f_lerp(a.y, b.y, t), f_lerp(a.z, b.z, t)
	);
}


//Trig : Works in degrees.
float i_sin(int angle) {
	//Wrap angle to [0, 360)
	int a = angle % 360;
	if (a < 0) {a += 360;}

	int quad = a / 90;  //[0-3]
	int idx  = a % 90;  //[0-89] deg

	//If quad is odd, use mirrored index
	int lut_index = idx ^ ((quad & 1) ? 89 : 0);

	//negative if (quad >= 2)
	int sign = ((quad & 2) ? -1 : 1);
	return sign * sin_LUT[lut_index]; //read table for this sin value
}
float i_cos(int angle)   {return i_sin(angle + 90);}
float f_sin(float angle) { return i_sin((int)f_round(angle)); }
float f_cos(float angle) { return i_cos((int)f_round(angle)); }
//// GENERAL MATHS ////




//// Vec2_t MATHS ////
Vec2_t v2_add(Vec2_t a, Vec2_t b) {return createVec2_t(a.x+b.x, a.y+b.y);}
Vec2_t v2_sub(Vec2_t a, Vec2_t b) {return createVec2_t(a.x-b.x, a.y-b.y);}
Vec2_t v2_mul(Vec2_t a, float b) {return createVec2_t(a.x*b, a.y*b);}
Vec2_t v2_div(Vec2_t a, float b) {
	if (f_abs(b) < EPSILON) {return emptyVec2_t();}
	return createVec2_t(a.x/b, a.y/b);
}
float v2_dot(Vec2_t a, Vec2_t b) {return (a.x*b.x) + (a.y*b.y);}
float v2_lenSQ(Vec2_t v) {return v2_dot(v, v);}
float v2_len(Vec2_t v) {
	float l = v2_lenSQ(v);
	if (f_abs(l) < EPSILON) {return 0.0f;}
	return l * f_invsqrt(l); //x * x^-½ = x^½. It works out. Still looks a bit odd though.
}
Vec2_t v2_normalise(Vec2_t v) {
	float l = v2_len(v);
	if (f_abs(l) < EPSILON) {return emptyVec2_t();}
	return v2_div(v, l);
}
float v2_distance(Vec2_t a, Vec2_t b) {return v2_len(v2_sub(a, b));}
Vec2_t v2_normalVec(Vec2_t v) {return createVec2_t(-v.y, v.x); /* Always the same direction, doesn't matter for this use-case. */}
//// Vec2_t MATHS ////




//// Vec3_t MATHS ////
Vec3_t v3_add(Vec3_t a, Vec3_t b) {return createVec3_t(a.x+b.x, a.y+b.y, a.z+b.z);}
Vec3_t v3_sub(Vec3_t a, Vec3_t b) {return createVec3_t(a.x-b.x, a.y-b.y, a.z-b.z);}
Vec3_t v3_mul(Vec3_t a, float b) {return createVec3_t(a.x*b, a.y*b, a.z*b);}
Vec3_t v3_div(Vec3_t a, float b) {
	if (f_abs(b) < EPSILON) {return emptyVec3_t();}
	return createVec3_t(a.x/b, a.y/b, a.z/b);
}
float v3_dot(Vec3_t a, Vec3_t b) {
	return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
}
float v3_lenSQ(Vec3_t v) {return v3_dot(v, v);}
float v3_len(Vec3_t v) {
	float l = v3_lenSQ(v);
	if (f_abs(l) < EPSILON) {return 0.0f;}
	return l * f_invsqrt(l); //x * x^-½ = x^½. It works out. Still looks a bit odd though.
}
Vec3_t v3_normalise(Vec3_t v) {
	float l = v3_len(v);
	if (f_abs(l) < EPSILON) {return emptyVec3_t();}
	return v3_div(v, l);
}
float v3_distance(Vec3_t a, Vec3_t b) {return v3_len(v3_sub(a, b));}
//// Vec3_t MATHS ////



//// VecN_t CASTING ////
inline Vec2_t v2_fromV3(Vec3_t v) {return createVec2_t(v.x, v.y);}
inline Vec3_t v3_fromV2(Vec2_t v) {return createVec3_t(v.x, v.y, 0.0f);}
//// VecN_t CASTING ////




//// MISC ////
inline Line_t ray(Vec2_t start, Vec2_t dir, float len) {
	Line_t ln;
	ln.start = start;
	ln.end = v2_add(
		v2_mul(dir, len), start
	);
	ln.dir = dir;
	return ln;
}
//// MISC ////



//// DISPLAY GENERIC ////
color_t toRGB565(unsigned char r, unsigned char g, unsigned char b) {
	return (
		((r & 0xF8) << 8) | //R (5b)
		((g & 0xFC) << 3) | //G (6b)
		((b & 0xF8) >> 3)   //B (5b)
	);
}
color_t v3_toRGB565(Vec3_t colourRGB) {
	return toRGB565(
		(unsigned char)(colourRGB.x), (unsigned char)(colourRGB.y), (unsigned char)(colourRGB.z)
	);
}


inline void clearDisplay(color_t fill) {Bdisp_Fill_VRAM(fill, 3);}

inline int drawPixel(Vec2_t position, color_t colour) {
	//Returns success/fail.
	color_t* VRAM = (color_t*)GetVRAMAddress(); //Get VRAM start
	unsigned int index = ((unsigned int)(position.y) * LCD_WIDTH_PX) + (unsigned int)(position.x);
	if (index >= LCD_WIDTH_PX*LCD_HEIGHT_PX) {return FALSE;}
	*(VRAM+index) = colour;
	return TRUE;
}


int drawHorizontalLine(Vec2_t start, int length, color_t colour) {
	if (length == 0) {return FALSE;}
	color_t* VRAM = (color_t*)GetVRAMAddress(); //Get VRAM start
	int x0 = (int)start.x; int y = (int)start.y;

	//return if row outside screen
	if ((y < 0) || (y >= LCD_HEIGHT_PX)) {return FALSE;}

	//negative length must be accounted for
	if (length < 0) {
		x0 += length;
		length = -length;
	}

	//Clamp to screen width
	int x1 = f_min(x0 + length, LCD_WIDTH_PX);
	if (x0 < 0) x0 = 0;
	if (x0 >= x1) {return FALSE;}

	unsigned int index = y * LCD_WIDTH_PX + x0;
	for (int i=0; i<length; i++) {VRAM[index + i] = colour;}

	return TRUE;
}


int drawVerticalLine(Vec2_t start, int length, color_t colour) {
	if (length == 0) {return FALSE;}
	color_t* VRAM = (color_t*)GetVRAMAddress(); //Get VRAM start

	int y = (int)(start.y);
	int endY = y + length; //Length can be negative to go upward. Assumes down by default.
	int iLo = f_min((int)start.y, endY); int iHi = f_max((int)start.y, endY);
	int count = iHi - iLo + 1;

	color_t* ptr = VRAM + (iLo*LCD_WIDTH_PX) + (int)(start.x);
	for (int i=0; i<count; i++) {
	    *ptr = colour;
	    ptr += LCD_WIDTH_PX;
	}
	return TRUE;
}
//// DISPLAY GENERIC ////


#endif
