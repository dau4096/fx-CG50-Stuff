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
constexpr float cosLUT[91] = {1.0, 0.9998, 0.9994, 0.9986, 0.9976, 0.9962, 0.9945, 0.9925, 0.9903, 0.9877, 0.9848, 0.9816, 0.9781, 0.9744, 0.9703, 0.9659, 0.9613, 0.9563, 0.9511, 0.9455, 0.9397, 0.9336, 0.9272, 0.9205, 0.9135, 0.9063, 0.8988, 0.891, 0.8829, 0.8746, 0.866, 0.8572, 0.848, 0.8387, 0.829, 0.8192, 0.809, 0.7986, 0.788, 0.7771, 0.766, 0.7547, 0.7431, 0.7314, 0.7193, 0.7071, 0.6947, 0.682, 0.6691, 0.6561, 0.6428, 0.6293, 0.6157, 0.6018, 0.5878, 0.5736, 0.5592, 0.5446, 0.5299, 0.515, 0.5, 0.4848, 0.4695, 0.454, 0.4384, 0.4226, 0.4067, 0.3907, 0.3746, 0.3584, 0.342, 0.3256, 0.309, 0.2924, 0.2756, 0.2588, 0.2419, 0.225, 0.2079, 0.1908, 0.1736, 0.1564, 0.1392, 0.1219, 0.1045, 0.0872, 0.0698, 0.0523, 0.0349, 0.0175, 0.0};



//Utility functions
namespace utils {

	
	//CASIO SDK Colour stuff;
	static inline unsigned short createColour(unsigned char r, unsigned char g, unsigned char b) {
		return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
	}
	static inline unsigned short createColour(Vec3 colour) {
		Vec3 c = min(max(colour, Vec3()), Vec3(0xFF, 0xFF, 0xFF));
		return ((int(c.x) >> 3) << 11) | ((int(c.y) >> 2) << 5) | (int(c.z) >> 3);
	}
	static inline Vec3 reverseColour(unsigned short colourRGB565) {
		return Vec3(
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



	//Trig.
	static inline float cos(int angle) {
		int modAngle = angle % 360;

		int LUTangle, sgn;
		if (modAngle < -90 || modAngle > 90) {
			LUTangle = 180 - abs(modAngle);
			sgn = -1;
		} else {
			LUTangle = abs(modAngle);
			sgn = 1;
		}

		return cosLUT[LUTangle] * sgn;
	}
	static inline float cos(float angle) {
		return cos(static_cast<int>(floor(angle + 0.5f)));
	}
	static inline float sin(int angle) {
		return cos(angle - 90);
	}
	static inline float sin(float angle) {
		return cos(static_cast<int>(floor(angle + 0.5f)) - 90);
	}
	static inline float tan(int angle) {
		return cos(angle - 90) / cos(angle);
	} //Tan is Sin/Cos
	static inline float tan(float angle) {
		return tan(static_cast<int>(floor(angle + 0.5f)));
	}





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

	
	static inline float fmod(float numerator, float divisor) {
		if (divisor == 0.0f) return 0.0f;
		int divis = static_cast<int>(numerator / divisor);
		return numerator - (divisor * divis);
	}
}

#endif