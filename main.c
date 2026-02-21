#include <fxcg/display.h>
#include <fxcg/keyboard.h>


//// SRC headers ////
#include "src/utils.h"
#include "src/graphics.h"
//// SRC headers ////



Wall_t walls[MAX_WALLS];


int main() {
	Bdisp_AllClr_VRAM();

	PrintXY(1, 1, "  Hello, world!", 0, TEXT_COLOR_BLACK);

	Bdisp_PutDisp_DD();


	//Create camera instance
	Camera_t camera;
	camera.position = createVec2_t(0.0f, 0.0f);
	camera.yaw = 0.0f; //Start at 0.0 deg yaw/rotation
	camera.FOV = 70.0f; //70 deg
	camera.maxDistance = 16.0f; //Maximum view distance.


	//Add a testing wall;
	walls[0].valid = TRUE;
	walls[0].start = createVec2_t(-1.0f, 2.0f);
	walls[0].end = createVec2_t(1.0f, 2.0f);
	walls[0].colour = toRGB565(255u, 0, 255u);


	int key;
	while (TRUE) {
		//Keypresses
		if (GetKey(&key)) {
			if (key == KEY_CTRL_EXE) {break;}
		}

		g_drawFrame(&camera, walls);
		Bdisp_PutDisp_DD(); //Push VRAM to screen
		Bdisp_AllClr_VRAM(); //Clear VRAM.
	}

	return 0;
}
