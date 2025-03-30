#ifndef UTILS_H
#define UTILS_H
extern "C" {
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
}
#include "structs.h"
#include "constants.h"


//Invalids and constants.
constexpr vec2 INVALID = vec2(0xFFFFFFFF, 0xFFFFFFFF);



//Utility functions
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
	static inline void drawPixel(int xPosition, int yPosition, unsigned short colour) {
		int newY = yPosition + display::MENU_HEIGHT;
		if (xPosition >= 0 && xPosition < LCD_WIDTH_PX && newY >= 0 && newY < LCD_HEIGHT_PX) {
			Bdisp_SetPoint_VRAM(xPosition, newY, colour);
		}
	}




	//Maths and Trig
	float sqrtApprox(float value); //Uses newton-raphson for an estimate of the square root.
	static inline float min(float A, float B) {return (A<B) ? A : B;}
	static inline float max(float A, float B) {return (A>B) ? A : B;}
	static inline float clamp(float value, float minV, float maxV) {return (value < maxV) ? ((value > minV) ? value : minV) : maxV;}
	static inline float abs(float value) {return (value<0) ? -value : value;}


	//Vector struct operations
	//vec2;
	static inline vec2 ADD2(vec2 A, vec2 B) {return vec2(A.x+B.x, A.y+B.y);}
	static inline vec2 SUB2(vec2 A, vec2 B) {return vec2(A.x-B.x, A.y-B.y);}
	static inline vec2 MUL2(vec2 A, float scalar) {return vec2(A.x*scalar, A.y*scalar);}
	static inline vec2 DIV2(vec2 A, float scalar) {return vec2(A.x/scalar, A.y/scalar);}
	float LENGTH2(vec2 vector); //Uses sqrtApprox()
	vec2 NORM2(vec2 vector); //Uses LENGTH2()
	static inline float DOT2(vec2 A, vec2 B) {return A.x*B.x + A.y*B.y;}
	static inline float DET2(vec2 A, vec2 B) {return A.x*B.y - A.y*B.x;}

	//vec3;
	static inline vec3 ADD3(vec3 A, vec3 B) {return vec3(A.x+B.x, A.y+B.y, A.z+B.z);}
	static inline vec3 SUB3(vec3 A, vec3 B) {return vec3(A.x-B.x, A.y-B.y, A.z-B.z);}
	static inline vec3 MUL3(vec3 A, float scalar) {return vec3(A.x*scalar, A.y*scalar, A.z*scalar);}
	static inline vec3 DIV3(vec3 A, float scalar) {return vec3(A.x/scalar, A.y/scalar, A.z/scalar);}
	float LENGTH3(vec3 vector); //Uses sqrtApprox()
	vec3 NORM3(vec3 vector); //Uses LENGTH3()
	static inline float DOT2(vec3 A, vec3 B) {return A.x*B.x + A.y*B.y + A.z*B.z;}


	static int printLN = 1;
	static inline void printTXT(const char* text=(const char*)"", int xPosition=5) {
		PrintXY(xPosition, printLN, text, 0, 0);
		printLN = (printLN % 8) + 1;		
	}
	static inline void resetPrintLN() {
		printLN = 1;
	}


	int getRNG(); //Uses set RNGtable of 256 unsigned chars.
	void clearRNG();


	//Keypress functions
	void keyUpdateAlt(); //Call every frame.
	bool isKeyPressed(int basicKeycode); //Pass in SDK Enum and returns bool. Non-blocking.
}

#endif