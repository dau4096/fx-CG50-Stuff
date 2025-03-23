#ifndef RENDER_H
#define RENDER_H

#include "utils.h"

namespace render {

	//CASIO SDK Specific;
	void drawPixel(int x, int y, unsigned short colour);
	void drawLine(int x, int length, unsigned short colour);

	void showTiles(const Tile (*tileData)[display::MAX_TILES]);

}

#endif