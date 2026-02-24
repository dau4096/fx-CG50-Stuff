/* graphics.c */
#include <fxcg/display.h>

//// SRC headers ////
#include "shared.h" //Constants & Structs
#include "utils.h"
//// SRC headers ////



static unsigned char depthBuffer[LCD_WIDTH_PX];
unsigned char g_mapDepth(float depthF, float maxDepth) {
	return (unsigned char)((depthF / maxDepth) * 255.0f);
}


int g_getCentreX(Vec2_t position, Camera_t* camera) {
	Vec2_t direction = v2_normalise(position - v2_fromV3(camera->position));
	int theta = f_atan2_int(direction.x, direction.y); //Integer atan2 with LUT.
	int angleDelta = theta - camera->viewAngle;
	if (angleDelta > 180) {angleDelta -= 360;}
	if (angleDelta < -180) {angleDelta += 360;}
	int centreX = (
		(LCD_WIDTH_PX / 2) * ((angleDelta * 2 / camera->fov) + 1)
	);
	return centreX;
}


/*
//Raycasting logic, Not relevant anymore.
//Keeping for near-future reference.
int g_getWallIntersect(Line_t* thisRay, Wall_t* thisWall, Vec2_t* intersectPoint) {
	Vec2_t start = v2_fromV3(thisWall->start);
	Vec2_t end   = v2_fromV3(thisWall->end);

	Vec2_t wallDir = v2_sub(end, start);
	Vec2_t wallNormal = v2_normalVec(wallDir);

	float projStart = v2_dot(
		v2_sub(thisRay->start, start),
		wallNormal
	);
	float projEnd = v2_dot(
		v2_sub(thisRay->end, start),
		wallNormal
	);
	if (projStart * projEnd > 0.0f) {
		//Ray never crosses infinite line.
		return FALSE;
	}

	float t = projStart / (projEnd - projStart);
	*intersectPoint = v2_sub(
		thisRay->start,
		v2_mul(thisRay->dir, thisRay->length * t)
	);
	Vec2_t minLine = v2_min(start, end);
	Vec2_t maxLine = v2_max(start, end);
	if (
		intersectPoint->x + EPSILON < minLine.x || intersectPoint->x - EPSILON > maxLine.x ||
		intersectPoint->y + EPSILON < minLine.y || intersectPoint->y - EPSILON > maxLine.y
	) {
		//Outside of valid line segment.
		return FALSE;
	}

	return TRUE;
}



void g_getWallProjections(
	Wall_t* thisWall, float cameraZ, float invDistance,
	int* screenYLow, int* screenYTop
) {
	//Seems to render weirdly. z=1.0f camera is inline with z=0.0f wall somehow?

	//Wall's start projection
	float projectedYStart = (cameraZ - thisWall->start.z) * invDistance;
	int yStart = (int)(LCD_HEIGHT_PX * (0.5f - projectedYStart));

	//Wall's end projection
	float projectedYEnd = (cameraZ - thisWall->end.z) * invDistance;
	int yEnd = (int)(LCD_HEIGHT_PX * (0.5f - projectedYEnd));

	if (yStart < yEnd) {
		*screenYLow = yStart;
		*screenYTop = yEnd;
	} else {
		*screenYLow = yEnd;
		*screenYTop = yStart;
	}
}



void g_drawWall(
	unsigned int x, Line_t* thisRay,
	Wall_t* thisWall, float xMult,
	float cameraZ
) {
	//Find an intersect and if found, draw a wall here.
	//DOOM/Wolf3D style.

	Vec2_t intersectPoint;
	int hit = g_getWallIntersect(thisRay, thisWall, &intersectPoint);
	if (!hit) {return; /* No hit. /}

	//Was hit, draw.
	float distance = f_max(v2_distance(thisRay->start, intersectPoint) * xMult, 0.1f); //Stop the height getting too absurd.
	int screenYLow, screenYTop; //Lower/upper screen position
	g_getWallProjections(
		thisWall, cameraZ, 1.0f/distance,
		&screenYLow, &screenYTop
	);

	//Draw a vertical line to represent this column.
	Vec2_t screenPos = createVec2_t(
		x, screenYTop
	);
	int height = screenYTop - screenYLow;

	float cMult = f_clamp(
		1.125f - (distance*2.0f / thisRay->length),
		0.125f, 1.0f
	);
	color_t colour = v3_toRGB565(v3_mul(thisWall->colour, cMult));
	d_drawVerticalLine(screenPos, height, colour);
}
*/



void g_getWallProjections(
	LineDef_t* thisLineDef, float cameraZ, float invDistance,
	int* screenYLow, int* screenYTop
) {
	//Seems to render weirdly. z=1.0f camera is inline with z=0.0f wall somehow?

	//Wall's start projection
	float projectedYStart = (thisLineDef->start.z - cameraZ) * invDistance;
	int yStart = (int)(LCD_HEIGHT_PX * (0.5f - projectedYStart));

	//Wall's end projection
	float projectedYEnd = (thisLineDef->end.z - cameraZ) * invDistance;
	int yEnd = (int)(LCD_HEIGHT_PX * (0.5f - projectedYEnd));

	if (yStart < yEnd) {
		*screenYLow = yStart;
		*screenYTop = yEnd;
	} else {
		*screenYLow = yEnd;
		*screenYTop = yStart;
	}
}


void g_drawColumn(LineDef_t* thisLineDef, int x, float depthF, float cameraZ) {
	//Draw this wall collumn.
	int screenYLow, screenYTop;
	g_getWallProjections(
		thisLineDef, cameraZ, 1.0f/depthF,
		&screenYLow, &screenYTop
	);
	if ((screenYTop<0) || (screenYLow>=LCD_HEIGHT_PX)) {return; /* Completely offscreen vertically. */}

	int yLow = f_max(0, screenYLow);
	int yTop = f_min(LCD_HEIGHT_PX, screenYTop);
	//Draw.
	/*
	for (int y=yLow; y<yTop; y++) {
		//TBA for texturing.
	}
	*/
	d_drawVerticalLine( //Generic draw.
		emptyVec2_t(x, y), yTop-yLow,
		v3_toRGB565(thisLineDef->colour)
	);
}



void g_drawLineDef_t(LineDef_t* thisLineDef, Camera_t* camera) {
	if (!thisLineDef->valid) {return; /* Wall is not valid, exit. */}
	Vec2_t cPosV2 = v2_fromV3(camera->position);
	Vec2_t wallSV2 = v2_fromV3(thisLineDef->start);
	Vec2_t wallEV2 = v2_fromV3(thisLineDef->end);

	int startXPosition = g_getCentreX(wallSV2, camera);
	int endXPosition = g_getCentreX(wallEV2, camera);
	if (startXPosition == endXPosition) {return; /* Too thin to bother rendering. */}

	float startZ = v2_distance(wallSV2, cPosV2);
	float endZ = v2_distance(wallEV2, cPosV2);

	int leftmost, rightmost;
	float lZ, rZ;
	if (startXPosition < endXPosition) {
		leftmost = startXPosition;
		rightmost = endXPosition;
		lZ = startZ;
		rZ = endZ;
	} else {
		leftmost = endXPosition;
		rightmost = startXPosition;
		lZ = endZ;
		rZ = startZ;
	}
	float range = (float)(rightmost - leftmost);

	if ((leftmost>=LCD_WIDTH_PX) || (rightmost<0)) {return; /* Wall is not onscreen. */}


	//Clamp to screen bounds.
	if (leftmost < 0) {
		float t = (-leftmost) / range;	//How far into the wall is x=0?
		lZ = f_lerp(lZ, rZ, t);
		leftmost = 0;
		range = (float)(rightmost - leftmost);
	}

	if (rightmost >= LCD_WIDTH_PX) {
		float t = (rightmost - leftmost) / range;	//How far into the wall is the LCD's right edge?
		rZ = f_lerp(lZ, rZ, t);
		rightmost = LCD_WIDTH_PX - 1;
		range = (float)(rightmost - leftmost);
	}


	//Draw, checking depthmap.
	for (int x=leftmost; x<rightmost; x++) {
		float t = (x-leftmost) / range;
		float depthF = f_lerp(lZ, rZ, t);
		unsigned char depth8b = g_mapDepth(depthF, camera->maxDistance);

		unsigned char* currentDepthPTR = &(depthBuffer[x]);
		if (depth8b < *currentDepthPTR) {
			//This collumn renders in front, as it's closer.
			*currentDepthPTR = depth8b;
			g_drawColumn(thisLineDef, x, depthF, camera->position.z);
		}
	}
}



//Lightish blue and grey.
#define SKY_COLOUR 0x661F /* (96, 192, 255) */
#define FLOOR_COLOUR 0x630C /* (96, 96, 96) */
void g_drawFakeBG() {
	d_fill(SKY_COLOUR); //Draw "sky"

	/* //Removed lower half colour for now.
	Vec2_t p;
	p.x = 0.0f;
	for (unsigned int y=LCD_HEIGHT_PX/2; y<LCD_HEIGHT_PX; y++) {
		//Draw "floor"
		p.y=y;
		d_drawHorizontalLine(p, LCD_WIDTH_PX, FLOOR_COLOUR);
	}
	*/	
}



void g_drawFrame(
	Camera_t* camera,
	Vec2_t* vertices,
	LineDef_t* linedefs,
	Sector_t* sectors
) {
	//Loop horizontally through the screen.
	g_drawFakeBG();

	//Clear "depth buffer" to max value (255s)
	memset(depthBuffer, 0xFF, sizeof(depthBuffer));

	//Loop through sectors.
	for (unsigned int sIndex=0u; sIndex<MAX_SECTORS; sIndex++) {
		Sector_t* thisSector = sectors+sIndex;
		//Loop through sector's linedefs.
		for (unsigned int ldIndex=0u; ldIndex<thisSector->numLineDefs; ldIndex++) {
			LineDef_t* thisLineDef = linedefs+ldIndex;
			g_drawLineDef_t(thisLineDef, camera);
		}
	}

	f_print(camera->position.x, 2);
	f_print(camera->position.y, 2);
	f_print(camera->position.z, 2);
}
