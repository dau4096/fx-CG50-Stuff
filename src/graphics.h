#ifndef GRAPHICS_H
#define GRAPHICS_H

extern "C" {
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
}
#include "utils.h"
#include <string.h>
using namespace std;



namespace data {
	inline unsigned int numTris;
	inline Vec4 indices[display::MAX_TRIANGLES]; //3 indices and a triangle index.
	inline Vec3 vertices[display::MAX_VERTICES];
	inline Vec4 projectedVertices[display::MAX_VERTICES];
	inline Edge edges[display::MAX_EDGES];
}

namespace graphics {

void drawScene(Camera& camera);

}

#endif