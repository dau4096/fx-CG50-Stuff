/* graphics.c */
#include <fxcg/display.h>

//// SRC headers ////
#include "shared.h" //Constants & Structs
#include "utils.h"
//// SRC headers ////


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
	if (!hit) {return; /* No hit. */}

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



//Lightish blue and grey.
#define SKY_COLOUR 0x661F /* (96, 192, 255) */
#define FLOOR_COLOUR 0x630C /* (96, 96, 96) */
void g_drawFakeBG() {
	d_fill(SKY_COLOUR); //Draw "sky"

	Vec2_t p;
	p.x = 0.0f;
	for (unsigned int y=LCD_HEIGHT_PX/2; y<LCD_HEIGHT_PX; y++) {
		//Draw "floor"
		p.y=y;
		d_drawHorizontalLine(p, LCD_WIDTH_PX, FLOOR_COLOUR);
	}	
}



void g_drawFrame(Camera_t* camera, Wall_t* walls) {
	//Loop horizontally through the screen.
	g_drawFakeBG();
	for (unsigned int x=0u; x<LCD_WIDTH_PX; x++) {
		float t = (x + 0.5f) / LCD_WIDTH_PX; //[0,1]
		float angleOffset = (t - 0.5f) * camera->FOV; ////Offset camera ray angle by [-FOV/2, +FOV/2]

		float rayAngle = (float)(camera->yaw) + angleOffset;
		Vec2_t direction = createVec2_t(
			f_sin(rayAngle), f_cos(rayAngle)
		);

		Line_t thisRay = l_ray(
			v2_fromV3(camera->position),
			direction, camera->maxDistance
		);

		float xMult = f_cos(angleOffset); //Used to try combat fisheye effect, artificially changing ray lengths. Visually feels acceptable when applied.

		//Loop through walls.
		for (unsigned int wIndex=0u; wIndex<MAX_WALLS; wIndex++) {
			Wall_t* thisWall = walls+wIndex;
			if (!thisWall->valid) {continue; /* Wall is not valid, skip. */}
			g_drawWall(
				x, &thisRay,
				thisWall, xMult,
				camera->position.z
			);
		}
	}
	f_print(camera->position.x, 2);
	f_print(camera->position.y, 2);
	f_print(camera->position.z, 2);
}
