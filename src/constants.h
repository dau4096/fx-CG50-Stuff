#ifndef CONSTANTS_H
#define CONSTANTS_H
extern "C" {
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
}

#include "structs.h"



constexpr Vec2 INVALIDv2 = Vec2(INF, INF);
constexpr Vec3 INVALIDv3 = Vec3(INF, INF, INF);
constexpr float MOVE_SPEED = 0.125f;
constexpr float TURN_SPEED = 5.0f;


//Mathematical Constants
#define PI 3.141592f
#define EXP 2.718281f

#define TO_RAD 0.017453f;
#define TO_DEG 57.295779f;

namespace display {
	//Max Values
	constexpr int MENU_HEIGHT = 24;
	constexpr Vec2 SCREEN_RESOLUTION = Vec2(LCD_WIDTH_PX, LCD_HEIGHT_PX);
	constexpr float ASPECT_RATIO = LCD_WIDTH_PX / LCD_HEIGHT_PX;

	constexpr unsigned int MAX_TRIANGLES = 1u;
	constexpr unsigned int MAX_VERTICES = MAX_TRIANGLES * 3u;
	constexpr unsigned int MAX_EDGES = (MAX_VERTICES >= 1u) ? MAX_VERTICES - 1u : 0u;

}

namespace dev {
	//Rendering options.
	constexpr bool DRAW_EDGES = false; //Draws the Edge class instances over the triangles
	constexpr bool DRAW_WIREFRAME = false; //Only draws edges. (Horizontal included)
	constexpr bool SHOW_CORNERS = false; //Draws markers on each corner for visual coordinate reference

	constexpr bool REQUIRES_EDGES = DRAW_EDGES || DRAW_WIREFRAME;

	constexpr bool DRAW_BACKFACES = true; //If a triangle has inverted winding order (the back) then it corrects that. Can cause issues as method is not ideal.	
}

namespace RGB565 {
	constexpr unsigned short BLACK = 0x0000u;
	constexpr unsigned short RED = 0xF800u;
	constexpr unsigned short GREEN = 0x07E0u;
	constexpr unsigned short BLUE = 0x001Fu;
	constexpr unsigned short YELLOW = 0xFFE0u;
	constexpr unsigned short MAGENTA = 0xF81Fu;
	constexpr unsigned short CYAN = 0x07FFu;
	constexpr unsigned short WHTE = 0xFFFFu;

	constexpr unsigned short EDGE_COLOUR_L = 0xFFEFu;
	constexpr unsigned short EDGE_COLOUR_R = 0x7FFFu;
}



//All keycodes from the FX-CG50 keyboard, for use in utils::isKeyPressed();
#define FUNC_1 0x070Au
#define FUNC_2 0x060Au
#define FUNC_3 0x050Au
#define FUNC_4 0x040Au
#define FUNC_5 0x030Au
#define FUNC_6 0x020Au

#define SHIFT 0x0709u
#define OPTN 0x0609u
#define VARS 0x0509u
#define MENU 0x0409u

#define UP 0x0209u
#define DOWN 0x0308u
#define LEFT 0x0309u
#define RIGHT 0x0208u

#define ALPHA 0x0708u
#define SQUARE 0x0608u
#define EXPONENT 0x0508u
#define EXIT 0x0408u

#define X_THETA_T 0x0707u
#define LOGARITHM_10 0x0607u
#define LOGARITHM_E 0x0507u
#define SINE 0x0407u
#define COSINE 0x0307u
#define TANGENT 0x0207u

#define MIXED_FRACTION 0x0706u
#define STANDARD_TO_DECIMAL 0x0606u
#define OPEN_BRACKET 0x0506u
#define CLOSE_BRACKET 0x0406u
#define COMMA 0x0306u
#define ARROW_RIGHT 0x0206u

#define NUM_7 0x0705u
#define NUM_8 0x0605u
#define NUM_9 0x0505u
#define DELETE 0x0405u
#define AC_ON 0x0101u

#define NUM_4 0x0704u
#define NUM_5 0x0604u
#define NUM_6 0x0504u
#define MULTIPLY 0x0404u
#define DIVIDE 0x0304u

#define NUM_1 0x0703u
#define NUM_2 0x0603u
#define NUM_3 0x0503u
#define ADD 0x0403u
#define SUBTRACT 0x0303u

#define NUM_0 0x0702u
#define DECIMAL_MARKER 0x0602u
#define BASE_10_POWER_X 0x0502u
#define DASH 0x0402u
#define EXECUTE 0x0302u


#endif