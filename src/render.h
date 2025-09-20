#ifndef RENDER_H
#define RENDER_H

extern "C" {
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
}
#include "utils.h"
#include <string.h>
using namespace std;



namespace data {
	inline size_t numTris;
	inline Vec3 vertices[600];
	inline Vec3 indices[200];
}

namespace render {

void drawScene(Camera& camera);

}

#endif