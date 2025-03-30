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

	constexpr unsigned short RGB565_BLACK = 0x0000;
	constexpr unsigned short RGB565_RED = 0xF800;
	constexpr unsigned short RGB565_GREEN = 0x07E0; 
	constexpr unsigned short RGB565_BLUE = 0x001F;
	constexpr unsigned short RGB565_YELLOW = 0xFFE0;
	constexpr unsigned short RGB565_MAGENTA = 0xF81F; 
	constexpr unsigned short RGB565_CYAN = 0x07FF;
	constexpr unsigned short RGB565_WHTE = 0xFFFF;

}


#endif