#ifndef UTILS_H
#define UTILS_H


enum Side {
	NONE = 0b000,
	TOP = 0b001,
	LEFT = 0b010,
	RIGHT = 0b100,
	ALL = 0b111
};



struct vec2 {
	float x, y;

	constexpr vec2() : x(0.0f), y(0.0f) {}
	constexpr vec2(float x, float y)
		: x(x), y(y) {}
};

struct vec3 {
	float x, y, z;

	constexpr vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	constexpr vec3(float x, float y, float z)
		: x(x), y(y), z(z) {}
};

//Invalids and constants.
constexpr vec2 INVALID = vec2(0xFFFFFFFF, 0xFFFFFFFF);
namespace display {
	//Max Values
	constexpr int MENU_HEIGHT = 24;

	constexpr int TILE_GRID_WIDTH = 12;
	constexpr int TILE_GRID_HEIGHT = TILE_GRID_WIDTH * (LCD_HEIGHT_PX - MENU_HEIGHT) / LCD_WIDTH_PX;
	constexpr float TILE_GRID_SCALE = LCD_WIDTH_PX / TILE_GRID_WIDTH;
	constexpr vec2 TILE_GRID_OFFSET = vec2(TILE_GRID_SCALE/2, 0);

	constexpr int MAX_TILES = TILE_GRID_WIDTH * TILE_GRID_HEIGHT;
	constexpr int MAX_MATERIALS = 16;


	//Colour stuff.
	constexpr Side SHADOW_SIDE = RIGHT;
	constexpr int SHADOW_SHADING = 64;
	constexpr int SHADOW_SHADING_ALT = 32;


	constexpr unsigned short RGB565_BLACK = 0x0000;
	constexpr unsigned short RGB565_RED = 0xF800;
	constexpr unsigned short RGB565_GREEN = 0x07E0; 
	constexpr unsigned short RGB565_BLUE = 0x001F;
	constexpr unsigned short RGB565_YELLOW = 0xFFE0;
	constexpr unsigned short RGB565_MAGENTA = 0xF81F; 
	constexpr unsigned short RGB565_CYAN = 0x07FF;
	constexpr unsigned short RGB565_WHTE = 0xFFFF;

}
constexpr unsigned short INVALID_COLOUR = display::RGB565_MAGENTA;



constexpr float restrictV(float value, float minV, float maxV) {
	return (value <= maxV) ? ((value >= minV) ? value : minV) : maxV;
}
constexpr unsigned short cColour(int r, int g, int b) {
	int rF = restrictV(r, 0x00, 0xFF), gF = restrictV(g, 0x00, 0xFF), bF = restrictV(b, 0x00, 0xFF);
	return static_cast<unsigned short>((rF >> 3) << 11) | ((gF >> 2) << 5) | (bF >> 3);
}
struct Material {
	unsigned short colourUnshaded, colourShadeR, colourShadeL;
	bool hasShadow, valid;

	constexpr Material() : colourUnshaded(0x0000), colourShadeR(0x0000), colourShadeL(0x0000), hasShadow(false), valid(false) {}
	constexpr Material(vec3 colour, bool hasShadow)
		: colourUnshaded(cColour(colour.x, colour.y, colour.z)),
		colourShadeR(cColour(colour.x - display::SHADOW_SHADING, colour.y - display::SHADOW_SHADING, colour.z - display::SHADOW_SHADING)),
		colourShadeL(cColour(colour.x - display::SHADOW_SHADING_ALT, colour.y - display::SHADOW_SHADING_ALT, colour.z - display::SHADOW_SHADING_ALT)),
		hasShadow(hasShadow), valid(true) {}
};

struct Tile {
	vec2 position;
	const Material* material;
	bool valid;

	constexpr Tile() : position(0, 0), material(nullptr), valid(false) {}
	constexpr Tile(int index, const Material* material)
		: position(vec2(index % display::TILE_GRID_WIDTH, index / display::TILE_GRID_WIDTH)), material(material), valid(true) {}
};




//Materials
namespace material {
	constexpr Material YELLOW = Material(vec3(255, 255, 0), true);
	constexpr Material CYAN = Material(vec3(0, 255, 255), true);
	constexpr Material MAGENTA = Material(vec3(255, 0, 255), true);
}



//Utility functions
namespace utils {

	//CASIO SDK Specific;
	unsigned short createColour(unsigned char r, unsigned char g, unsigned char b);
	vec3 reverseColour(unsigned short colourRGB565);


	//Vector operations
	float dot(vec2 A, vec2 B);
	float determinant(vec2 A, vec2 B);
	float length(vec2 vector);
	vec2 normalise(vec2 vector);
	vec2 ADD(vec2 A, vec2 B);
	vec2 SUB(vec2 A, vec2 B);
	vec2 MUL(vec2 A, float scalar);
	vec2 DIV(vec2 A, float scalar);


	//Maths and Trig
	float min(float A, float B);
	float max(float A, float B);
	float clamp(float value, float min, float max);
	float abs(float value);

	void printTXT(const char* text=(const char*)"");
	void resetPrintLN();


	int getRNG();
	void clearRNG();
}

#endif