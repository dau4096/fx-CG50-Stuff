/* graphics.c */
#include <fxcg/display.h>
#include <string.h>

//// SRC headers ////
#include "shared.h" //Constants & Structs
#include "utils.h"
//// SRC headers ////



static unsigned char depthBuffer[LCD_WIDTH_PX];
unsigned char g_mapDepth(fixed_t depthF, fixed_t maxDepth) {
	return (unsigned char)(f_mul(f_div(depthF,  maxDepth), INT_TO_FIX(255)));
}


static int g_getCentreX(const Vec2_t position, Camera_t* camera) {
	Vec2_t direction = (v2_sub(
		position, (Vec2_t){0, 0}
	));
	int theta = f_atan2_int(direction.x, direction.y); //Integer atan2 with LUT.
	int angleDelta = theta - camera->yaw;
	if (angleDelta > 180) {angleDelta -= 360;}
	if (angleDelta < -180) {angleDelta += 360;}
	int centreX = (
		(LCD_WIDTH_PX / 2) * ((angleDelta * 2 / camera->FOV) + 1)
	);
	return centreX;
}


static void g_getLineDefSectorProjections(
	Sector_t* thisSector, fixed_t cameraZ, fixed_t invDistance,
	int* screenYFloor, int* screenYCeiling
) {
	//Projects the height of the floor/ceil at a certain distance into screenspace.
	//Distance is distance to some LineDef.

	//Sector's floor projection
	fixed_t projectedYFloor = f_mul((thisSector->floorHeight - cameraZ), invDistance);
	*screenYFloor = FIX_TO_INT(LCD_HEIGHT_PX * (FLOAT_TO_FIX(0.5f) - projectedYFloor));

	//Sector's ceiling projection
	fixed_t projectedYCeiling = f_mul((thisSector->ceilingHeight - cameraZ), invDistance);
	*screenYCeiling = FIX_TO_INT(LCD_HEIGHT_PX * (FLOAT_TO_FIX(0.5f) - projectedYCeiling));
}


static void g_drawColumn(Sector_t* thisSector, int x, fixed_t depthF, fixed_t cameraZ) {
	//Draw this wall collumn.
	int screenYFloor, screenYCeiling;
	g_getLineDefSectorProjections(
		thisSector, cameraZ, f_div(FIX_ONE, depthF),
		&screenYFloor, &screenYCeiling
	);
	if ((screenYCeiling<0) || (screenYFloor>=LCD_HEIGHT_PX)) {return; /* Completely offscreen vertically. */}

	int yLow = f_max(FIX_ZERO, screenYFloor);
	int yTop = f_min(LCD_HEIGHT_PX, screenYCeiling);
	//Draw.
	/*
	for (int y=yLow; y<yTop; y++) {
		//TBA for texturing.
	}
	*/
	d_drawVerticalLine( //Generic draw.
		(Vec2_t){x, yLow}, yTop-yLow,
		0xF81Fu //(255, 0, 255) in RGB565
	);
}



static void g_drawLineDef_t(Sector_t* thisSector, Vec2_t ldStart, Vec2_t ldEnd, Camera_t* camera) {
	Vec2_t cPosV2 = v2_fromV3(camera->position);

	int startXPosition = g_getCentreX(ldStart, camera);
	int endXPosition = g_getCentreX(ldEnd, camera);
	d_drawVerticalLine((Vec2_t){INT_TO_FIX(startXPosition), FIX_ZERO}, LCD_HEIGHT_PX, 0xF800u);
	d_drawVerticalLine((Vec2_t){INT_TO_FIX(endXPosition), FIX_ZERO}, LCD_HEIGHT_PX, 0x001Fu);
	if (startXPosition == endXPosition) {return; /* Too thin to bother rendering. */}
	return; //Positions seem wrong? Could be fixed_t weirdness. TLB error later on in this func too.

	fixed_t startZ = v2_distance(ldStart, cPosV2);
	fixed_t endZ = v2_distance(ldEnd, cPosV2);

	int leftmost, rightmost;
	fixed_t lZ, rZ;
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
	fixed_t range = (fixed_t)(rightmost - leftmost);

	if ((leftmost>=LCD_WIDTH_PX) || (rightmost<0)) {return; /* LineDef is not onscreen. */}


	//Clamp to screen bounds.
	if (leftmost < 0) {
		fixed_t t = (-leftmost) / range;	//How far into the LineDef is x=0?
		lZ = f_lerp(lZ, rZ, t);
		leftmost = 0;
		range = (fixed_t)(rightmost - leftmost);
	}

	if (rightmost >= LCD_WIDTH_PX) {
		fixed_t t = (rightmost - leftmost) / range;	//How far into the LineDef is the LCD's right edge?
		rZ = f_lerp(lZ, rZ, t);
		rightmost = LCD_WIDTH_PX - 1;
		range = (fixed_t)(rightmost - leftmost);
	}


	//Draw, checking depthmap.
	fixed_t invLZ = 1.0f / lZ; fixed_t invRZ = 1.0f / rZ;
	for (int x=leftmost; x<rightmost; x++) {
		fixed_t t = (x-leftmost) / range;
		fixed_t depthF = 1.0f / f_lerp(invLZ, invRZ, t);
		unsigned char depth8b = g_mapDepth(depthF, camera->maxDistance);

		unsigned char* currentDepthPTR = &(depthBuffer[x]);
		if (depth8b < *currentDepthPTR) {
			//This collumn renders in front, as it's closer.
			*currentDepthPTR = depth8b;
			g_drawColumn(
				thisSector, //Sector ptr
				x, depthF,
				camera->position.z
			);
		}
	}
}



//Lightish blue and grey.
#define SKY_COLOUR 0x661F /* (96, 192, 255) */
#define FLOOR_COLOUR 0x630C /* (96, 96, 96) */
static void g_drawFakeBG() {
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
		for (unsigned int secLD=0u; secLD<thisSector->numLineDefs; secLD++) {
			unsigned int ldIndex = thisSector->lineDefs[secLD];
			LineDef_t* thisLineDef = linedefs+ldIndex;
			g_drawLineDef_t(
				thisSector,
				vertices[thisLineDef->vStart],
				vertices[thisLineDef->vEnd],
				camera
			);
		}
	}

	//f_print(camera->position.x, 2);
	//f_print(camera->position.y, 2);
	//f_print(camera->position.z, 2);
}
