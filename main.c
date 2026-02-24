#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <stdio.h>


//// SRC headers ////
#include "src/utils.h"
#include "src/graphics.h"
//// SRC headers ////




//// SECTORS, VERTICES AND LINDEFS ////
Vec2_t vertices[MAX_VERTICES];
LineDef_t linedefs[MAX_LINEDEFS];
Sector_t sectors[MAX_SECTORS];
//// SECTORS, VERTICES AND LINDEFS ////


void createTestGEO() {
	//Vertices
	vertices[0] = createVec2_t(-1.0f, -1.0f);
	vertices[1] = createVec2_t(-1.0f,  1.0f);
	vertices[2] = createVec2_t( 1.0f,  1.0f);
	vertices[3] = createVec2_t( 1.0f, -1.0f);

	//Lines (square Sector)
	linedefs[0] = createLineDef_t(0, 1, 0, -1);
	linedefs[1] = createLineDef_t(1, 2, 0, -1);
	linedefs[2] = createLineDef_t(2, 3, 0, -1);
	linedefs[3] = createLineDef_t(3, 0, 0, -1);

	//Sector
	int lnIndices[4] = {0, 1, 2, 3};
	sectors[0] = createSector_t(-1.0f, 1.0f, lnIndices, 4);
}



void handleInput(Camera_t* camera, int* RUN) {
	int key;
	GetKey(&key);

	Vec3_t f = v3_mul(createVec3_t( //FORWARD
		f_sin(camera->yaw), f_cos(camera->yaw), 0.0f
	), MOVE_SPD);
	Vec3_t l = v3_normalVec(f); //LEFT
	Vec3_t u = createVec3_t(0.0f, 0.0f, MOVE_SPD); //UP

	switch (key) {
		case KEY_CTRL_EXE: {RUN=FALSE; break;} //EXIT PRGM [0x7534]

		case KEY_CTRL_LEFT:  {camera->yaw = (camera->yaw - TURN_SPD) % 360; break;} //TURN LEFT  [0x7544]
		case KEY_CTRL_RIGHT: {camera->yaw = (camera->yaw + TURN_SPD) % 360; break;} //TURN RIGHT [0x7545]

		case KEY_CHAR_8: {camera->position = v3_add(camera->position, f); break;} //MOVE FORWARD  [0x38]
		case KEY_CHAR_2: {camera->position = v3_sub(camera->position, f); break;} //MOVE BACKWARD [0x32]
		case KEY_CHAR_4: {camera->position = v3_add(camera->position, l); break;} //STRAFE LEFT   [0x34]
		case KEY_CHAR_6: {camera->position = v3_sub(camera->position, l); break;} //STRAFE RIGHT  [0x36]
		case KEY_CHAR_7: {camera->position = v3_sub(camera->position, u); break;} //MOVE DOWN     [0x37]
		case KEY_CHAR_9: {camera->position = v3_add(camera->position, u); break;} //MOVE UP       [0x39]
	}
}



int main() {
	Bdisp_EnableColor(1);
	d_update();


	//Create camera instance
	Camera_t camera;
	camera.position = createVec3_t(0.0f, 0.0f, 0.0f);
	camera.yaw = 0.0f; //Start at 0.0 deg yaw/rotation
	camera.FOV = 70.0f; //70 deg
	camera.maxDistance = 16.0f; //Maximum view distance.

	createTestGEO();

	int RUN = TRUE;
	while (RUN) {
		g_drawFrame(&camera, vertices, linedefs, sectors);
		i_print(camera.yaw);

		//d_printBuf((char*)(buf));
		d_update(); //Update screen
		d_resetPrintLN();


		//Input
		handleInput(&camera, &RUN);
	}

	return 0;
}
