extern "C" {
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
}
#include "src/structs.h"
#include "src/constants.h"
#include "src/utils.h"
#include "src/graphics.h"
using namespace std;
using namespace utils;


Camera camera = Camera(
	Vec3(0.0f, -2.0f, 0.0f),    //Position
	Vec2(0.0f, 0.0f),           //Angle
	70.0f, 			            //FOV
	0.01f, 100.0f				//Near/Far
);


void TMPFillTris() {
	data::vertices[0] = Vec3(-1.0f, 0.0f, 0.0f);
	data::vertices[1] = Vec3( 1.0f, 0.0f, 0.0f);
	data::vertices[2] = Vec3( 0.0f, 0.0f, 1.0f);

	data::indices[0] = Vec4(0, 1, 2, 0);

	data::numTris = 1u;
}


void handleMovement(int key) {
	//Turning
	if (key == KEY_PRGM_UP) {
		camera.angle.y = min(camera.angle.y + TURN_SPEED, 90.0f - TURN_SPEED);
		return;
	} else if (key == KEY_PRGM_DOWN) {
		camera.angle.y = max(camera.angle.y - TURN_SPEED, TURN_SPEED - 90.0f);
		return;
	} else if (key == KEY_PRGM_LEFT) {
		camera.angle.x = camera.angle.x + TURN_SPEED;
		return;
	} else if (key == KEY_PRGM_RIGHT) {
		camera.angle.x = camera.angle.x - TURN_SPEED;
		return;
	}


	Vec3 forward = Vec3(
		sin(camera.angle.x),
		cos(camera.angle.x),
		0.0f
	).normalise();
	Vec3 right = forward.cross(Vec3(0.0f, 0.0f, 1.0f)).normalise();

	//Movement
	if (key == KEY_CHAR_8) {
		camera.position = camera.position + forward * MOVE_SPEED;
		return;
	} else if (key == KEY_CHAR_2) {
		camera.position = camera.position - forward * MOVE_SPEED;
		return;
	} else if (key == KEY_CHAR_6) {
		camera.position = camera.position + right * MOVE_SPEED;
		return;
	} else if (key == KEY_CHAR_4) {
		camera.position = camera.position - right * MOVE_SPEED;
		return;
	}
}


int main() {
	//SDK Initialisation
	Bdisp_EnableColor(1); //Enable 16b colour;
	Bdisp_AllClr_VRAM();
	Bdisp_PutDisp_DD();

	TMPFillTris();
	int key;

	while (1) { //Constant loop until exit.
		//Handle input
		if (key == KEY_CTRL_EXIT) {
			key = 0;
			break; //Exit program immediately.
		}

		handleMovement(key);


		graphics::drawScene(camera);


		Bdisp_PutDisp_DD();
		resetPrintLN();
		key = 0;
		GetKey(&key); //For next frame.
	}
	return 0;

}