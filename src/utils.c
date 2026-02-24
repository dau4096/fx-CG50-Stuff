/* utils.c */

#include <stdint.h>
#include <fxcg/misc.h>
#include <fxcg/display.h>


#include "trigLUT.h" //Contains the trig L.U.T. for integer degrees 0-90*
#include "shared.h" //Constants & Structs



//// GENERAL MATHS ////
//Fast inverse square root (Quake III Arena), but with added directive and renamed to f_invsqrt to fit naming scheme.
float f_invsqrt( float number )
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
int i_abs(int v) {return (v>0.0f) ? v : -v;}
int i_min(int a, int b) {return (a<b) ? a : b;}
int i_max(int a, int b) {return (a>b) ? a : b;}
int i_clamp(int v, int mi, int ma) {return i_max(mi, i_min(v, ma));}
int i_sign(int v) {return (v>0.0f) - (v<0.0f); /* Using strange C-"bool" maths. 1 if >0, 0 if =0, -1 if <0. */}

//Float funcs
float f_abs(float v) {return (v>0.0f) ? v : -v;}
float f_min(float a, float b) {return (a<b) ? a : b;}
float f_max(float a, float b) {return (a>b) ? a : b;}
float f_clamp(float v, float mi, float ma) {return f_max(mi, f_min(v, ma));}
int f_sign(float v) {return (v>0.0f) - (v<0.0f); /* Using strange C-"bool" maths. 1 if >0, 0 if =0, -1 if <0. */}
float f_floor(float v) {
	int i = (int)(v);
	return ((v < 0.0f) && (v != (float)(i))) ? (float)(i - 1) : (float)i;
}
float f_ceil(float v) {
	int i = (int)(v);
	return ((v > 0.0f) && (v != (float)(i))) ? (float)(i + 1) : (float)i;
}
float f_round(float v) {return f_floor(v + 0.5f);}
float f_fract(float v) {return v - f_floor(v);}
float f_sqrt(float v) {
	if (v < EPSILON) {return 0.0f; /* No 0/negative values. */}
	return v * f_invsqrt(v); //x * x^-½ = x^½. It works out. Still looks a bit odd though.
}


//Linear interps
float f_lerp(float a, float b, float t) {return a + t*(b-a);}
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
	//Wrap to 360 deg
	int a = angle % 360;
	if (a < 0) {a += 360;}

	int quad = a / SINE_LUT_SIZE; //[0-3] quadrant
	int idx  = a % SINE_LUT_SIZE; //[0-89] angle


	int lut_index;
	int sign;
	switch (quad) {
		case 0: {lut_index = idx;        sign =  1; break;} //[0-90] deg
		case 1: {lut_index = 89 - idx;   sign =  1; break;} //[90-180] deg
		case 2: {lut_index = idx;        sign = -1; break;} //[180-270] deg
		case 3: {lut_index = 89 - idx;   sign = -1; break;} //[270-360] deg
	}

	return sign * sin_LUT[lut_index];
}
float i_cos(int angle)   {return i_sin(angle + 90);}
float f_sin(float angle) {return i_sin((int)f_round(angle));}
float f_cos(float angle) {return i_cos((int)f_round(angle));}

int f_atan2_int(int y, int x) {
	//Find atan2(y,x) in int degrees.
	int absY = abs(y);
	int absX = abs(x);

	if ((absX == 0) && (absY == 0)) {return 0;}

	//Find value in Look-Up-Table
	int angle;
	if (absX > absY) {
		//Read directly
		int idx = (absY << ATAN_LUT_BITS) / absX;
		if (idx >= ATAN_LUT_SIZE) idx = ATAN_LUT_SIZE - 1;
		angle = atan_LUT[idx];
	} else {
		//Read and modify value (not first 45 deg)
		int idx = (absX << ATAN_LUT_BITS) / absY;
		if (idx >= ATAN_LUT_SIZE) idx = ATAN_LUT_SIZE - 1;
		angle = 90 - atan_LUT[idx];
	}

	if ((x >= 0) && (y >= 0)) {return angle;}
	else if ((x < 0) && (y >= 0)) {return 180 - angle;}
	else if ((x < 0) && (y < 0)) {return 180 + angle;}
	else {return 360 - angle;}
}
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
Vec2_t v2_min(Vec2_t a, Vec2_t b) {return createVec2_t(f_min(a.x, b.x), f_min(a.y, b.y));}
Vec2_t v2_max(Vec2_t a, Vec2_t b) {return createVec2_t(f_max(a.x, b.x), f_max(a.y, b.y));}
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
Vec3_t v3_normalVec(Vec3_t v) {return createVec3_t(-v.y, v.x, v.z); /* In 2D, acts like v2_normalVec but with added Z. */}
Vec3_t v3_min(Vec3_t a, Vec3_t b) {return createVec3_t(f_min(a.x, b.x), f_min(a.y, b.y), f_min(a.z, b.z));}
Vec3_t v3_max(Vec3_t a, Vec3_t b) {return createVec3_t(f_max(a.x, b.x), f_max(a.y, b.y), f_max(a.z, b.z));}
//// Vec3_t MATHS ////



//// VecN_t CASTING ////
Vec2_t v2_fromV3(Vec3_t v3) {return createVec2_t(v3.x, v3.y);}
Vec3_t v3_fromV2(Vec2_t v2) {return createVec3_t(v2.x, v2.y, 0.0f);}
Vec3_t v3_fromV2_alt(Vec2_t v2, float s) {return createVec3_t(v2.x, v2.y, s);}
//// VecN_t CASTING ////




//// MISC ////
LineDef_t createLineDef_t(unsigned int vStart, unsigned int vEnd, unsigned int sFront, unsigned int sBack) {
	LineDef_t linedef;

	linedef.vStart = vStart;
	linedef.vEnd = vEnd;
	linedef.frontSector = sFront;
	linedef.backSector = sBack;

	return linedef;
}

Sector_t createSector_t(float hFloor, float hCeil, unsigned int* lns, unsigned int nLns) {
	Sector_t sector;

	sector.floorHeight = hFloor;
	sector.ceilingHeight = hCeil;
	sector.lineDefs = lns;
	sector.numLineDefs = nLns;

	return sector;
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



int d_drawPixel(Vec2_t position, color_t colour) {
	//Returns success/fail.
	color_t* VRAM = (color_t*)GetVRAMAddress(); //Get VRAM start
	unsigned int index = ((unsigned int)(position.y) * LCD_WIDTH_PX) + (unsigned int)(position.x);
	if (index >= LCD_WIDTH_PX*LCD_HEIGHT_PX) {return FALSE;}
	*(VRAM+index) = colour;
	return TRUE;
}


int d_drawHorizontalLine(Vec2_t start, int length, color_t colour) {
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


int d_drawVerticalLine(Vec2_t start, int length, color_t colour) {
	if (length == 0) {return FALSE; /* Nothing to draw. */}

	//Clamp X to screen range
	int x = (int)(start.x);
	if ((x < 0) || (x >= LCD_WIDTH_PX)) {return FALSE; /* offscreen */}

	//Handle negative length.
	int y0 = (int)(start.y);
	int y1 = y0 + length;

	//Ensure [y0 <= y1], swap otherwise.
	if (y0 > y1) {
		int tmp = y0;
		y0 = y1;	y1 = tmp;
	}

	//Clamp Y to screen range
	if ((y1 < 0) || (y0 >= LCD_HEIGHT_PX)) {return FALSE; /* offscreen */}
	if (y0 < 0) {y0 = 0;}
	if (y1 >= LCD_HEIGHT_PX) {y1 = LCD_HEIGHT_PX - 1;}

	//get pointer to VRAM
	color_t* VRAM = (color_t*)(GetVRAMAddress());
	color_t* ptr = VRAM + (y0 * LCD_WIDTH_PX) + x;

	//Draw in a loop
	for (int y=y0; y<=y1; y++) {
		*ptr = colour;
		ptr += LCD_WIDTH_PX; //increment by one row
	}

	return TRUE;
}




int i_countDigits(int v) {
	if (v == 0) {return 1;}

	int count = 0;
	if (v < 0) {
		count++; //for neg sign
		v = -v;
	}

	while (v > 0) {
		v /= 10;
		count++;
	}

	return count;
}


static unsigned char lnIdx = 0u;
#define CHAR_WIDTH 20
#define MAX_PRINT_LINES 8
void d_printBuf(char* buf) {
	int x = 1; //1px from the left
	lnIdx = (lnIdx+1) % MAX_PRINT_LINES;
	PrintXY(x, lnIdx, buf, TEXT_MODE_NORMAL, TEXT_COLOR_BLACK);
}

//Prints int at some given position
void i_printXY(int v, Vec2_t pos) {
	unsigned char buf[12];
	itoa(v, buf);
	locate_OS(pos.x, pos.y);
	Print_OS(buf, 0, 0);
}

//Automatically prints int using an incrementing line index.
void i_print(int v) {
	lnIdx = (lnIdx+1) % MAX_PRINT_LINES;
	i_printXY(v, createVec2_t(1, (float)(lnIdx)));
}


//Includes leading 0s
void printPaddedInt(int v, int digits, Vec2_t pos) {
	unsigned char buf[16];
	itoa(v, buf);

	int len = 0;
	while (buf[len] != 0) {len++;}

	int pad = digits - len;
	int x = pos.x;

	//Show the 0s
	while ((pad--) > 0) {
		locate_OS(x, pos.y);
		Print_OS("0", 0, 0);
		x += CHAR_WIDTH;
	}

	locate_OS(x, pos.y);
	Print_OS(buf, 0, 0);
}

//Prints float at some given position
void f_printXY(float v, int decimals, Vec2_t pos) {
	int x = pos.x;

	//Handle negative values
	int isNegative = 0;
	if (v < 0) {
		isNegative = 1;
		v = -v;
	}

	int vWhole = (int)(v);
	float vFrac = v - vWhole;

	//display minus sign if needed
	if (isNegative) {
		locate_OS(x, pos.y);
		Print_OS("-", 0, 0);
		x++;
	}

	//display whole part
	i_printXY(vWhole, createVec2_t(x, pos.y));

	int wholeDigits = i_countDigits(vWhole);
	x += wholeDigits;

	//display decimal point
	locate_OS(x, pos.y);
	Print_OS(".", 0, 0);
	x++;

	//make fractional whole (to be cast to int)
	for (int i=0; i<decimals; i++) {vFrac *= 10.0f;}
	//display fractional part
	printPaddedInt((int)(vFrac), decimals, createVec2_t(x, pos.y));
}

//Automatically prints float using an incrementing line index.
void f_print(float v, int decimals) {
	lnIdx = (lnIdx+1) % MAX_PRINT_LINES;
	f_printXY(v, decimals, createVec2_t(1, (float)(lnIdx)));
}

void d_resetPrintLN() {lnIdx = 0u;}
//// DISPLAY GENERIC ////
