/* graphics.c */
#include <fxcg/display.h>

//// SRC headers ////
#include "utils.h"
//// SRC headers ////



int g_getWallIntersect(Line_t* thisRay, Wall_t* thisWall, Vec2_t* intersect_OUT) {
	Vec2_t wallDir = v2_sub(thisWall->end, thisWall->start);
	Vec2_t wallNormal = v2_normalise(v2_normalVec(wallDir));

	Vec2_t startDelta = v2_sub(thisWall->start, thisRay->start);
	Vec2_t endDelta = v2_sub(thisWall->start, thisRay->end);

	float startDot = v2_dot(startDelta, wallNormal);
	float endDot = v2_dot(endDelta, wallNormal);

	if (startDot*endDot >= 0.0f) {
		//Either both have the same sign, or one is 0.
		//Thus, no intersect.
		*intersect_OUT = emptyVec2_t();
		return FALSE;
	}

	//Get t (ratio of the start / (start+end))
	//start/end are projected onto the wall normal. They are proportional to the length of the ray, and start+end = ray len.
	float t = startDot / (startDot + endDot);

	Vec2_t point = v2_add(
		thisRay->start,
		v2_mul(thisRay->dir, t)
	);

	if ((t < 0.0f) || (t > 1.0f)) {
		//Not in the range of the wall.
		return FALSE;
	}

	*intersect_OUT = point;
	return TRUE;
}



void g_drawWall(
	unsigned int x, Line_t* thisRay, Wall_t* thisWall,
	float viewDistance, float xMultiplier
) {
	//Find an intersect and if found, draw a wall here.
	//DOOM/Wolf3D style.

	Vec2_t intersectPoint;
	int hit = g_getWallIntersect(thisRay, thisWall, &intersectPoint);
	if (!hit) {return; /* No hit. */}

	//Was hit, draw.
	float distance = v2_distance(thisRay->start, intersectPoint) / xMultiplier;
	int height = (int)(LCD_HEIGHT_PX / (distance / viewDistance));

	//Draw a vertical line to represent this column.
	Vec2_t screenPos = createVec2_t(
		x, (LCD_HEIGHT_PX-height)/2
	);
	drawVerticalLine(screenPos, height, thisWall->colour);
}



void g_drawFrame(Camera_t* camera, Wall_t* walls) {
	//Loop horizontally through the screen.
	for (unsigned int x=0u; x<LCD_WIDTH_PX; x++) {
		float t = (x + 0.5f) / LCD_WIDTH_PX; //[0,1]
		float angleOffset = (t - 0.5f) * camera->FOV; ////Offset camera ray angle by [-FOV/2, +FOV/2]
		float rayAngle = camera->yaw + angleOffset;
		Vec2_t direction = createVec2_t(
			f_sin(rayAngle), f_cos(rayAngle)
		);
		Line_t thisRay = ray(camera->position, direction, camera->maxDistance);

		float xMultiplier = f_cos(angleOffset); //Used to try combat fisheye effect, artificially changing ray lengths. Visually feels acceptable when applied.

		//Loop through walls.
		for (unsigned int wIndex=0u; wIndex<MAX_WALLS; wIndex++) {
			Wall_t* thisWall = walls+wIndex;
			if (!thisWall->valid) {continue; /* Wall is not valid, skip. */}
			g_drawWall(
				x, &thisRay, thisWall,
				camera->maxDistance,
				xMultiplier
			);
		}
	}
}
