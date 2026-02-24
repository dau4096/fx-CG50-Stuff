#ifndef UTILS_H
#define UTILS_H


#include "shared.h"


//// GENERAL MATHS ////
//Fast inverse square root (Quake III Arena), but with added directive and renamed to f_invsqrt to fit naming scheme.
float f_invsqrt( float number );


//Inlined because they're so simplistic.
//Int funcs
int i_abs(int v);
int i_min(int a, int b);
int i_max(int a, int b);
int i_clamp(int v, int mi, int ma);
int i_sign(int v);

//Float funcs
float f_abs(float v);
float f_min(float a, float b);
float f_max(float a, float b);
float f_clamp(float v, float mi, float ma);
int f_sign(float v);
float f_floor(float v);
float f_ceil(float v);
float f_round(float v);
float f_fract(float v);
float f_sqrt(float v);


//Linear interps
float f_lerp(float a, float b, float t);
Vec2_t v2_lerp(Vec2_t a, Vec2_t b, float t);
Vec3_t v3_lerp(Vec3_t a, Vec3_t b, float t);


//Trig : Works in degrees.
float i_sin(int angle);
float i_cos(int angle);
float f_sin(float angle);
float f_cos(float angle);
int f_atan2_int(int y, int x);
//// GENERAL MATHS ////




//// Vec2_t MATHS ////
Vec2_t v2_add(Vec2_t a, Vec2_t b); 		// +
Vec2_t v2_sub(Vec2_t a, Vec2_t b); 		// -
Vec2_t v2_mul(Vec2_t a, float b);  		// ×
Vec2_t v2_div(Vec2_t a, float b);  		// ÷
float v2_dot(Vec2_t a, Vec2_t b);  		// •
float v2_lenSQ(Vec2_t v);          		//Returns length^2
float v2_len(Vec2_t v);            		//Includes sqrt [^]
Vec2_t v2_normalise(Vec2_t v);     		//Returns normalised v_hat
float v2_distance(Vec2_t a, Vec2_t b);  //Distance between 2 points
Vec2_t v2_normalVec(Vec2_t v);			//Simplistic 2D normal.
Vec2_t v2_min(Vec2_t a, Vec2_t b);		//Min of X/Y
Vec2_t v2_max(Vec2_t a, Vec2_t b);		//Max of X/Y
//// Vec2_t MATHS ////




//// Vec3_t MATHS ////
Vec3_t v3_add(Vec3_t a, Vec3_t b);		// +
Vec3_t v3_sub(Vec3_t a, Vec3_t b);		// -
Vec3_t v3_mul(Vec3_t a, float b);		// ×
Vec3_t v3_div(Vec3_t a, float b);		// ÷
float v3_dot(Vec3_t a, Vec3_t b);		// •
float v3_lenSQ(Vec3_t v);				//Returns length^2
float v3_len(Vec3_t v);					//Includes sqrt [^]
Vec3_t v3_normalise(Vec3_t v);			//Returns normalised v_hat
float v3_distance(Vec3_t a, Vec3_t b);	//Distance between 2 points
Vec3_t v3_normalVec(Vec3_t v);			//Simplistic 2D normal, passes Z through.
Vec3_t v3_min(Vec3_t a, Vec3_t b);		//Min of X/Y
Vec3_t v3_max(Vec3_t a, Vec3_t b);		//Max of X/Y
//// Vec3_t MATHS ////



//// VecN_t CASTING ////
Vec2_t v2_fromV3(Vec3_t v3); //Vec2_t(v3.xy);
Vec3_t v3_fromV2(Vec2_t v2); //Vec3_t(v2.xy, 0.0f);
Vec3_t v3_fromV2_alt(Vec2_t v2, float s);
//// VecN_t CASTING ////




//// MISC ////
LineDef_t createLineDef_t(unsigned int vStart, unsigned int vEnd, unsigned int sFront, unsigned int sBack);
Sector_t createSector_t(float hFloor, float hCeil, unsigned int* lns, unsigned int nLns);
//// MISC ////



//// DISPLAY GENERIC ////
color_t toRGB565(unsigned char r, unsigned char g, unsigned char b); //Convert R/G/B 8b to RGB565 16b
color_t v3_toRGB565(Vec3_t colourRGB); //[^] but takes in a Vec3_t. Better for colour maths.

static inline void d_fill(color_t fill) { //Fill the entire display with a colour
	Bdisp_AllClr_VRAM(); //Clear VRAM.
	Bdisp_Fill_VRAM(fill, 3);
}
static inline void d_update() { //Update the display.
	Bdisp_PutDisp_DD(); //Push VRAM to screen
}

int d_drawPixel(Vec2_t position, color_t colour); //Draw a singular pixel into VRAM

int d_drawHorizontalLine(Vec2_t start, int length, color_t colour); //Draws a horizontal line into VRAM. length can be negative to go left.
int d_drawVerticalLine(Vec2_t start, int length, color_t colour); //Draws a vertical line into VRAM. length can be negative to go upward.


void d_printBuf(char* buf);
void i_printXY(int v, Vec2_t pos);
void i_print(int v);
void f_printXY(int v, int decimals, Vec2_t pos);
void f_print(float v, int decimals); //Value, number of decimals.
void d_resetPrintLN();
//// DISPLAY GENERIC ////


#endif
