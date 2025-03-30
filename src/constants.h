#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "structs.h"


/*
//Enums go here. Example;
Enum Valid {
	NONE = 0,
	NO = 1,
	YES = 2
}
*/


constexpr float EPSILON = 1e-4f;
namespace display {
	//Max Values
	constexpr int MENU_HEIGHT = 24;
}

namespace RGB565 {
	constexpr unsigned short BLACK = 0x0000;
	constexpr unsigned short RED = 0xF800;
	constexpr unsigned short GREEN = 0x07E0; 
	constexpr unsigned short BLUE = 0x001F;
	constexpr unsigned short YELLOW = 0xFFE0;
	constexpr unsigned short MAGENTA = 0xF81F; 
	constexpr unsigned short CYAN = 0x07FF;
	constexpr unsigned short WHTE = 0xFFFF;	
}

namespace key {
	//All keycodes from the FX-CG50 keyboard, for use in utils::isKeyPressed();
	constexpr int FUNC_1 = 0x070A;
	constexpr int FUNC_2 = 0x060A;
	constexpr int FUNC_3 = 0x050A;
	constexpr int FUNC_4 = 0x040A;
	constexpr int FUNC_5 = 0x030A;
	constexpr int FUNC_6 = 0x020A;

	constexpr int SHIFT = 0x0709;
	constexpr int OPTN = 0x0609;
	constexpr int VARS = 0x0509;
	constexpr int MENU = 0x0409;

	constexpr int UP = 0x0209;
	constexpr int DOWN = 0x0308;
	constexpr int LEFT = 0x0309;
	constexpr int RIGHT = 0x0208;

	constexpr int ALPHA = 0x0708;
	constexpr int SQUARE = 0x0608;
	constexpr int EXPONENT = 0x0508;
	constexpr int EXIT = 0x0408;

	constexpr int X_THETA_T = 0x0707;
	constexpr int LOGARITHM_10 = 0x0607;
	constexpr int LOGARITHM_E = 0x0507;
	constexpr int SINE = 0x0407;
	constexpr int COSINE = 0x0307;
	constexpr int TANGENT = 0x0207;

	constexpr int MIXED_FRACTION = 0x0706;
	constexpr int STANDARD_TO_DECIMAL = 0x0606;
	constexpr int OPEN_BRACKET = 0x0506;
	constexpr int CLOSE_BRACKET = 0x0406;
	constexpr int COMMA = 0x0306;
	constexpr int ARROW_RIGHT = 0x0206;

	constexpr int NUM_7 = 0x0705;
	constexpr int NUM_8 = 0x0605;
	constexpr int NUM_9 = 0x0505;
	constexpr int DELETE = 0x0405;
	constexpr int AC_ON = 0x0101;

	constexpr int NUM_4 = 0x0704;
	constexpr int NUM_5 = 0x0604;
	constexpr int NUM_6 = 0x0504;
	constexpr int MULTIPLY = 0x0404;
	constexpr int DIVIDE = 0x0304;

	constexpr int NUM_1 = 0x0703;
	constexpr int NUM_2 = 0x0603;
	constexpr int NUM_3 = 0x0503;
	constexpr int ADD = 0x0403;
	constexpr int SUBTRACT = 0x0303;

	constexpr int NUM_0 = 0x0702;
	constexpr int DECIMAL_MARKER = 0x0602;
	constexpr int BASE_10_POWER_X = 0x0502;
	constexpr int DASH = 0x0402;
	constexpr int EXECUTE = 0x0302;
}


#endif