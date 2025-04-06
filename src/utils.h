#ifndef UTILS_H
#define UTILS_H

extern "C" {
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
}

#include <stdlib.h>
#include <stdint.h>
#include "structs.h"
#include "constants.h"
using namespace std;


constexpr float sinLUT[360] = {0.0, 0.0175, 0.0349, 0.0523, 0.0698, 0.0872, 0.1045, 0.1219, 0.1392, 0.1564, 0.1736, 0.1908, 0.2079, 0.225, 0.2419, 0.2588, 0.2756, 0.2924, 0.309, 0.3256, 0.342, 0.3584, 0.3746, 0.3907, 0.4067, 0.4226, 0.4384, 0.454, 0.4695, 0.4848, 0.5, 0.515, 0.5299, 0.5446, 0.5592, 0.5736, 0.5878, 0.6018, 0.6157, 0.6293, 0.6428, 0.6561, 0.6691, 0.682, 0.6947, 0.7071, 0.7193, 0.7314, 0.7431, 0.7547, 0.766, 0.7771, 0.788, 0.7986, 0.809, 0.8192, 0.829, 0.8387, 0.848, 0.8572, 0.866, 0.8746, 0.8829, 0.891, 0.8988, 0.9063, 0.9135, 0.9205, 0.9272, 0.9336, 0.9397, 0.9455, 0.9511, 0.9563, 0.9613, 0.9659, 0.9703, 0.9744, 0.9781, 0.9816, 0.9848, 0.9877, 0.9903, 0.9925, 0.9945, 0.9962, 0.9976, 0.9986, 0.9994, 0.9998, 1.0, 0.9998, 0.9994, 0.9986, 0.9976, 0.9962, 0.9945, 0.9925, 0.9903, 0.9877, 0.9848, 0.9816, 0.9781, 0.9744, 0.9703, 0.9659, 0.9613, 0.9563, 0.9511, 0.9455, 0.9397, 0.9336, 0.9272, 0.9205, 0.9135, 0.9063, 0.8988, 0.891, 0.8829, 0.8746, 0.866, 0.8572, 0.848, 0.8387, 0.829, 0.8192, 0.809, 0.7986, 0.788, 0.7771, 0.766, 0.7547, 0.7431, 0.7314, 0.7193, 0.7071, 0.6947, 0.682, 0.6691, 0.6561, 0.6428, 0.6293, 0.6157, 0.6018, 0.5878, 0.5736, 0.5592, 0.5446, 0.5299, 0.515, 0.5, 0.4848, 0.4695, 0.454, 0.4384, 0.4226, 0.4067, 0.3907, 0.3746, 0.3584, 0.342, 0.3256, 0.309, 0.2924, 0.2756, 0.2588, 0.2419, 0.225, 0.2079, 0.1908, 0.1736, 0.1564, 0.1392, 0.1219, 0.1045, 0.0872, 0.0698, 0.0523, 0.0349, 0.0175, 0.0, -0.0175, -0.0349, -0.0523, -0.0698, -0.0872, -0.1045, -0.1219, -0.1392, -0.1564, -0.1736, -0.1908, -0.2079, -0.225, -0.2419, -0.2588, -0.2756, -0.2924, -0.309, -0.3256, -0.342, -0.3584, -0.3746, -0.3907, -0.4067, -0.4226, -0.4384, -0.454, -0.4695, -0.4848, -0.5, -0.515, -0.5299, -0.5446, -0.5592, -0.5736, -0.5878, -0.6018, -0.6157, -0.6293, -0.6428, -0.6561, -0.6691, -0.682, -0.6947, -0.7071, -0.7193, -0.7314, -0.7431, -0.7547, -0.766, -0.7771, -0.788, -0.7986, -0.809, -0.8192, -0.829, -0.8387, -0.848, -0.8572, -0.866, -0.8746, -0.8829, -0.891, -0.8988, -0.9063, -0.9135, -0.9205, -0.9272, -0.9336, -0.9397, -0.9455, -0.9511, -0.9563, -0.9613, -0.9659, -0.9703, -0.9744, -0.9781, -0.9816, -0.9848, -0.9877, -0.9903, -0.9925, -0.9945, -0.9962, -0.9976, -0.9986, -0.9994, -0.9998, -1.0, -0.9998, -0.9994, -0.9986, -0.9976, -0.9962, -0.9945, -0.9925, -0.9903, -0.9877, -0.9848, -0.9816, -0.9781, -0.9744, -0.9703, -0.9659, -0.9613, -0.9563, -0.9511, -0.9455, -0.9397, -0.9336, -0.9272, -0.9205, -0.9135, -0.9063, -0.8988, -0.891, -0.8829, -0.8746, -0.866, -0.8572, -0.848, -0.8387, -0.829, -0.8192, -0.809, -0.7986, -0.788, -0.7771, -0.766, -0.7547, -0.7431, -0.7314, -0.7193, -0.7071, -0.6947, -0.682, -0.6691, -0.6561, -0.6428, -0.6293, -0.6157, -0.6018, -0.5878, -0.5736, -0.5592, -0.5446, -0.5299, -0.515, -0.5, -0.4848, -0.4695, -0.454, -0.4384, -0.4226, -0.4067, -0.3907, -0.3746, -0.3584, -0.342, -0.3256, -0.309, -0.2924, -0.2756, -0.2588, -0.2419, -0.225, -0.2079, -0.1908, -0.1736, -0.1564, -0.1392, -0.1219, -0.1045, -0.0872, -0.0698, -0.0523, -0.0349, -0.0175};
constexpr float cosLUT[360] = {1.0, 0.9998, 0.9994, 0.9986, 0.9976, 0.9962, 0.9945, 0.9925, 0.9903, 0.9877, 0.9848, 0.9816, 0.9781, 0.9744, 0.9703, 0.9659, 0.9613, 0.9563, 0.9511, 0.9455, 0.9397, 0.9336, 0.9272, 0.9205, 0.9135, 0.9063, 0.8988, 0.891, 0.8829, 0.8746, 0.866, 0.8572, 0.848, 0.8387, 0.829, 0.8192, 0.809, 0.7986, 0.788, 0.7771, 0.766, 0.7547, 0.7431, 0.7314, 0.7193, 0.7071, 0.6947, 0.682, 0.6691, 0.6561, 0.6428, 0.6293, 0.6157, 0.6018, 0.5878, 0.5736, 0.5592, 0.5446, 0.5299, 0.515, 0.5, 0.4848, 0.4695, 0.454, 0.4384, 0.4226, 0.4067, 0.3907, 0.3746, 0.3584, 0.342, 0.3256, 0.309, 0.2924, 0.2756, 0.2588, 0.2419, 0.225, 0.2079, 0.1908, 0.1736, 0.1564, 0.1392, 0.1219, 0.1045, 0.0872, 0.0698, 0.0523, 0.0349, 0.0175, 0.0, -0.0175, -0.0349, -0.0523, -0.0698, -0.0872, -0.1045, -0.1219, -0.1392, -0.1564, -0.1736, -0.1908, -0.2079, -0.225, -0.2419, -0.2588, -0.2756, -0.2924, -0.309, -0.3256, -0.342, -0.3584, -0.3746, -0.3907, -0.4067, -0.4226, -0.4384, -0.454, -0.4695, -0.4848, -0.5, -0.515, -0.5299, -0.5446, -0.5592, -0.5736, -0.5878, -0.6018, -0.6157, -0.6293, -0.6428, -0.6561, -0.6691, -0.682, -0.6947, -0.7071, -0.7193, -0.7314, -0.7431, -0.7547, -0.766, -0.7771, -0.788, -0.7986, -0.809, -0.8192, -0.829, -0.8387, -0.848, -0.8572, -0.866, -0.8746, -0.8829, -0.891, -0.8988, -0.9063, -0.9135, -0.9205, -0.9272, -0.9336, -0.9397, -0.9455, -0.9511, -0.9563, -0.9613, -0.9659, -0.9703, -0.9744, -0.9781, -0.9816, -0.9848, -0.9877, -0.9903, -0.9925, -0.9945, -0.9962, -0.9976, -0.9986, -0.9994, -0.9998, -1.0, -0.9998, -0.9994, -0.9986, -0.9976, -0.9962, -0.9945, -0.9925, -0.9903, -0.9877, -0.9848, -0.9816, -0.9781, -0.9744, -0.9703, -0.9659, -0.9613, -0.9563, -0.9511, -0.9455, -0.9397, -0.9336, -0.9272, -0.9205, -0.9135, -0.9063, -0.8988, -0.891, -0.8829, -0.8746, -0.866, -0.8572, -0.848, -0.8387, -0.829, -0.8192, -0.809, -0.7986, -0.788, -0.7771, -0.766, -0.7547, -0.7431, -0.7314, -0.7193, -0.7071, -0.6947, -0.682, -0.6691, -0.6561, -0.6428, -0.6293, -0.6157, -0.6018, -0.5878, -0.5736, -0.5592, -0.5446, -0.5299, -0.515, -0.5, -0.4848, -0.4695, -0.454, -0.4384, -0.4226, -0.4067, -0.3907, -0.3746, -0.3584, -0.342, -0.3256, -0.309, -0.2924, -0.2756, -0.2588, -0.2419, -0.225, -0.2079, -0.1908, -0.1736, -0.1564, -0.1392, -0.1219, -0.1045, -0.0872, -0.0698, -0.0523, -0.0349, -0.0175, -0.0, 0.0175, 0.0349, 0.0523, 0.0698, 0.0872, 0.1045, 0.1219, 0.1392, 0.1564, 0.1736, 0.1908, 0.2079, 0.225, 0.2419, 0.2588, 0.2756, 0.2924, 0.309, 0.3256, 0.342, 0.3584, 0.3746, 0.3907, 0.4067, 0.4226, 0.4384, 0.454, 0.4695, 0.4848, 0.5, 0.515, 0.5299, 0.5446, 0.5592, 0.5736, 0.5878, 0.6018, 0.6157, 0.6293, 0.6428, 0.6561, 0.6691, 0.682, 0.6947, 0.7071, 0.7193, 0.7314, 0.7431, 0.7547, 0.766, 0.7771, 0.788, 0.7986, 0.809, 0.8192, 0.829, 0.8387, 0.848, 0.8572, 0.866, 0.8746, 0.8829, 0.891, 0.8988, 0.9063, 0.9135, 0.9205, 0.9272, 0.9336, 0.9397, 0.9455, 0.9511, 0.9563, 0.9613, 0.9659, 0.9703, 0.9744, 0.9781, 0.9816, 0.9848, 0.9877, 0.9903, 0.9925, 0.9945, 0.9962, 0.9976, 0.9986, 0.9994, 0.9998};


namespace utils {

	//CASIO SDK Colour stuff;
	static inline unsigned short createColour(unsigned char r, unsigned char g, unsigned char b) {
		return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
	}
	static inline vec3 reverseColour(unsigned short colourRGB565) {
		return vec3(
			(colourRGB565 & 0xF800) >> 8, //First 5 bits
			(colourRGB565 & 0x07E0) >> 3, //Next 6 bits
			(colourRGB565 & 0x001F) // (>> 0); //Last 5 bits
		);
	}
	static inline void drawPixel(int xPosition, int yPosition, unsigned short colour, bool forceDraw=false) {
		int newY = yPosition + display::MENU_HEIGHT;
		if (forceDraw || ((xPosition >= 0) && (xPosition < LCD_WIDTH_PX) && (newY >= 0) && (newY < LCD_HEIGHT_PX))) {
			Bdisp_SetPoint_VRAM(xPosition, newY, colour);
		}
	}


	//Maths and Trig
	float sqrtApprox(float value); //Uses newton-raphson for an estimate of the square root.
	static inline float min(float A, float B) {return (A<B) ? A : B;}
	static inline float max(float A, float B) {return (A>B) ? A : B;}
	static inline float clamp(float value, float minV, float maxV) {return (value < maxV) ? ((value > minV) ? value : minV) : maxV;}
	static inline float abs(float value) {return (value<0) ? -value : value;}
	static inline float floor(float value) {return static_cast<float>((value >= 0) ? static_cast<int>(value) : static_cast<int>(value) - (value != static_cast<int>(value)));}
	static inline float ceil(float value) {return static_cast<float>((value <= 0) ? static_cast<int>(value) : static_cast<int>(value) + (value != static_cast<int>(value)));}
	static inline float fract(float value) {return (value>0) ? (value - floor(value)) : (ceil(value) - value);}
	static float sin(float angle) {return sinLUT[static_cast<int>(abs(floor(angle))) % 360];}
	static float cos(float angle) {return cosLUT[static_cast<int>(abs(floor(angle))) % 360];}


	//Vector struct operations
	//vec2;
	static inline vec2 ADD2(vec2 A, vec2 B) {return vec2(A.x+B.x, A.y+B.y);}
	static inline vec2 SUB2(vec2 A, vec2 B) {return vec2(A.x-B.x, A.y-B.y);}
	static inline vec2 MUL2(vec2 A, float scalar) {return vec2(A.x*scalar, A.y*scalar);}
	static inline vec2 DIV2(vec2 A, float scalar) {return vec2(A.x/scalar, A.y/scalar);}
	float LENGTH2(vec2 vector); //Uses sqrtApprox()
	vec2 NORM2(vec2 vector); //Uses LENGTH2()
	static inline vec2 ABS2(vec2 A) {return vec2(abs(A.x), abs(A.y));}
	static inline float DOT2(vec2 A, vec2 B) {return A.x*B.x + A.y*B.y;}
	static inline float DET2(vec2 A, vec2 B) {return A.x*B.y - A.y*B.x;}
	static inline bool EQU2(vec2 A, vec2 B) {return (((A.x-B.x)<EPSILON) && ((A.y-B.y)<EPSILON));}

	//vec3;
	static inline vec3 ADD3(vec3 A, vec3 B) {return vec3(A.x+B.x, A.y+B.y, A.z+B.z);}
	static inline vec3 SUB3(vec3 A, vec3 B) {return vec3(A.x-B.x, A.y-B.y, A.z-B.z);}
	static inline vec3 MUL3(vec3 A, float scalar) {return vec3(A.x*scalar, A.y*scalar, A.z*scalar);}
	static inline vec3 DIV3(vec3 A, float scalar) {return vec3(A.x/scalar, A.y/scalar, A.z/scalar);}
	float LENGTH3(vec3 vector); //Uses sqrtApprox()
	vec3 NORM3(vec3 vector); //Uses LENGTH3()
	static inline vec3 ABS3(vec3 A) {return vec3(abs(A.x), abs(A.y), abs(A.z));}
	static inline float DOT2(vec3 A, vec3 B) {return A.x*B.x + A.y*B.y + A.z*B.z;}
	static inline bool EQU3(vec3 A, vec3 B) {return (((A.x-B.x)<EPSILON) && ((A.y-B.y)<EPSILON) && ((A.z-B.z)<EPSILON));}


	static int printLN = 1;
	static inline void printTXT(const char* text=(const char*)"", int xPosition=5) {
		PrintXY(xPosition, printLN, text, 0, 0);
		printLN = (printLN&0b111) + 1;
	}
	static inline void printINT(int value, int xPosition=5) {
		PrintXY(xPosition, printLN, (const char*)value, 0, 0);
		printLN = (printLN&0b111) + 1;
	}
	static inline void printFLOAT(float value, int xPosition=5) {
		int fracScale = 100; //2DP.
		int whole = static_cast<int>(floor(value));
		int offset = (abs(whole) < 10) ? 1 : ((abs(whole) < 100) ? 2 : ((abs(whole) < 1000) ? 3 : 4));
		if (whole < 0) offset += 1; //Space for negative sign.
		int fractional = static_cast<int>(floor((value - whole) * fracScale));
		PrintXY(xPosition, printLN, (const char*)whole, 0, 0);
		PrintXY(xPosition+offset, printLN, (const char*)".", 0, 0);
		PrintXY(xPosition+offset+1, printLN, (const char*)fractional, 0, 0);
		printLN = (printLN&0b111) + 1;
	}
	static inline void printHEX(unsigned short input, int xPosition=5) {
		PrintXY(xPosition, printLN, (const char*)static_cast<int>(input), 0, 0);
		printLN = (printLN&0b111) + 1;
	}
	static inline void resetPrintLN() {
		printLN = 1;
	}


	int getRNG(); //Uses set RNGtable of 256 unsigned chars.
	void clearRNG();


	//Keypress functions
	void keyUpdate(); //Call every frame.
	bool isKeyPressed(int basicKeycode); //Pass in SDK Enum and returns bool. Non-blocking.



	Snake createSnake(vec2 startPoint);
	Snake movement(Snake snakeInstance, bool (*snakeMap)[display::GRID_SIZE.x * display::GRID_SIZE.y], int direction, vec2 applePos);


	static inline const char* getScoreInt(int digit) {
		switch (digit) {
			case 0:
				return (const char*)"  0";
			case 1:
				return (const char*)"  1";
			case 2:
				return (const char*)"  2";
			case 3:
				return (const char*)"  3";
			case 4:
				return (const char*)"  4";
			case 5:
				return (const char*)"  5";
			case 6:
				return (const char*)"  6";
			case 7:
				return (const char*)"  7";
			case 8:
				return (const char*)"  8";
			case 9:
				return (const char*)"  9";
		}
		return (const char*)"  X";
	};
	static inline void showScore(int score) {
		int units = score % 10;
		int tens = (score % 100) / 10;
		int hundreds = score / 100;


		PrintXY(20, 2, (const char*)"  S", 0, 0);
		const char* charHund = getScoreInt(hundreds);
		PrintXY(20, 3, charHund, 0, 0);
		const char* charTens = getScoreInt(tens);
		PrintXY(20, 4, charTens, 0, 0);
		const char* charUnit = getScoreInt(units);
		PrintXY(20, 5, charUnit, 0, 0);
	};

	int randomInRange(int min, int max);

}

#endif