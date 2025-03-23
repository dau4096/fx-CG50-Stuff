extern "C" {
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
}

#include "utils.h"
#include <stdio.h>
using namespace std;
using namespace utils;

namespace render {


unsigned short shade(unsigned short colour, Side sideToShade) {
	int newColour = colour;
	if ((sideToShade & TOP) != 0) {
		newColour = utils::min(colour + display::SHADOW_SHADING_TOP, 0xFFFF);
	} else if (((sideToShade & LEFT) | (sideToShade & RIGHT)) != 0) {
		newColour = utils::max(colour - display::SHADOW_SHADING_SIDE, 0x0000);
	}
	unsigned short reformattedColour = static_cast<unsigned short>(newColour & 0xFFFF);
	return reformattedColour;
}


void drawPixel(int xPosition, int yPosition, unsigned short colour) {
	const int newY = yPosition + display::MENU_HEIGHT;
	if (xPosition >= 0 && xPosition < LCD_WIDTH_PX && newY >= 0 && newY < LCD_HEIGHT_PX) {
		Bdisp_SetPoint_VRAM(xPosition, newY, colour);
	}
}


void drawLine(int xPosition, int startY, int endY, unsigned short colour) {
	const int minY = utils::min(startY, endY);
	const int yRange = utils::abs(endY - startY);
	if (xPosition >= 0 && xPosition < LCD_WIDTH_PX) {
		for (int yOffset = 0; yOffset < yRange; yOffset++) {
			drawPixel(xPosition, minY + yOffset, colour);
		}
	}
}


void drawCursor(vec2 cursorPosition, const Tile (*tileData)[display::MAX_TILES]) {
	int sidesToDraw = TOP; //Draw top by default.
	if ((cursorPosition.x > 0) && (cursorPosition.y < display::TILE_GRID_HEIGHT - 1)) {
		int packedPosition = (cursorPosition.y + 1) * display::TILE_GRID_WIDTH + (cursorPosition.x - 1);
		const Tile& neighbourTile = *tileData[packedPosition];
		if (!neighbourTile.valid) {sidesToDraw |= LEFT; /* Draw left side */}
	}
	if ((cursorPosition.x < display::TILE_GRID_WIDTH - 1) && (cursorPosition.y < display::TILE_GRID_HEIGHT - 1)) {
		int packedPosition = (cursorPosition.y + 1) * display::TILE_GRID_WIDTH + (cursorPosition.x + 1);
		const Tile& neighbourTile = *tileData[packedPosition];
		if (!neighbourTile.valid) {sidesToDraw |= RIGHT; /* Draw right side */}
	}

	//Draw it now, given the sides to draw.
}


void drawTile(const Tile* tile, Side sidesToDraw=ALL) {
	const Material& thisMaterial = tile->material ? *(tile->material) : material::MAGENTA; //.material is a pointer to a Material instance
	const xOffset = (tile->position.x * display::TILE_GRID_SCALE) + display::TILE_GRID_OFFSET.x;
	const yOffset = (tile->position.y * display::TILE_GRID_SCALE) + display::TILE_GRID_OFFSET.y;
	if ((int(tile->position.y) & 1) == 0) {xOffset += display::TILE_GRID_SCALE/2; /* Even Y values offset right by half a tile. */}

	if ((sidesToDraw & TOP) != 0) {
		printTXT((const char*)"  Top was drawn");
		unsigned short topColour = thisMaterial.colour;
		if (thisMaterial.hasShadow) {topColour = shade(topColour, TOP);}

		for (int x=0; x<display::TILE_GRID_SCALE/2; x++) {
			const int height = display::TILE_GRID_SCALE/4 - x/2;
			const int onscreenY = yOffset + x/2;

			//Left half of the diamond.
			const int xLeft = xOffset - x;
			drawLine(xLeft, onscreenY, onscreenY + height, topColour); 

			//Right half of the diamond.
			const int xRight = xOffset + x;
			drawLine(xRight, onscreenY, onscreenY + height, topColour);
		}
	}

	if ((sidesToDraw & LEFT) != 0) {
		printTXT((const char*)"  Left was drawn");
		unsigned short leftColour = thisMaterial.colour;
		const int height = display::TILE_GRID_SCALE / 2;

		if ((thisMaterial.hasShadow) && (display::SHADOW_SIDE == LEFT)) {leftColour = shade(leftColour, LEFT);}

		for (int x=0; x<height; x++) {
			const int onscreenX = xOffset + x;	
			const int onscreenY = yOffset + x/2;
			drawLine(onscreenX, onscreenY, onscreenY + height, leftColour);
		}
	}

	if ((sidesToDraw & RIGHT) != 0) {
		printTXT((const char*)"  Right was drawn");
		unsigned short rightColour = thisMaterial.colour;
		const int height = display::TILE_GRID_SCALE / 2;

		if ((thisMaterial.hasShadow) && (display::SHADOW_SIDE == RIGHT)) {rightColour = shade(rightColour, RIGHT);}

		for (int x=display::TILE_GRID_SCALE; x>=height; x--) {
			const int onscreenX = xOffset + x;	
			const int onscreenY = yOffset - x/2;
			drawLine(onscreenX, onscreenY + height, onscreenY + display::TILE_GRID_SCALE, rightColour);
		}
	}
}


void showTiles(const Tile (*tileData)[display::MAX_TILES]) {
	for (int idx=0; idx<display::MAX_TILES; idx++) {
		const Tile& thisTile = (*tileData)[idx];
		if (thisTile.valid == false) {continue;} //valid is always false.
		printTXT((const char*)"  Drawing Cube.");

		vec2 gridPosition = vec2(idx % display::TILE_GRID_WIDTH, idx / display::TILE_GRID_WIDTH);
		int sidesToDraw = TOP; //Draw top by default.
		if ((gridPosition.x > 0) && (gridPosition.y < display::TILE_GRID_HEIGHT - 1)) {
			int packedPosition = (gridPosition.y + 1) * display::TILE_GRID_WIDTH + (gridPosition.x - 1);
			const Tile& neighbourTile = *tileData[packedPosition];
			if (neighbourTile.valid == false) {sidesToDraw |= LEFT; /* Draw left side */}
		}
		if ((gridPosition.x < display::TILE_GRID_WIDTH - 1) && (gridPosition.y < display::TILE_GRID_HEIGHT - 1)) {
			int packedPosition = (gridPosition.y + 1) * display::TILE_GRID_WIDTH + (gridPosition.x + 1);
			const Tile& neighbourTile = *tileData[packedPosition];
			if (neighbourTile.valid == false) {sidesToDraw |= RIGHT; /* Draw right side */}
		}

		drawTile(&thisTile);//, static_cast<Side>(sidesToDraw)); //Ignore sides for now.

	}
}

}