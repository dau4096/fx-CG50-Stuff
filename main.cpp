extern "C" {
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/system.h"
}

#include <stdlib.h>
#include <stdint.h>
#include "src/structs.h"
#include "src/constants.h"
#include "src/utils.h"
#include "src/render.h"
using namespace std;
using namespace utils;


int direction, prevDirection;
bool snakeMap[display::GRID_SIZE.x * display::GRID_SIZE.y];


int main() {
	//SDK Initialisation
	Bdisp_EnableColor(1); //Enable 16b colour;
	Bdisp_AllClr_VRAM();
	Bdisp_PutDisp_DD();


	Snake snake = utils::createSnake(vec2(3,3));//snakeCFG::SNAKE_START_POSITION); //Start point at left centre
	direction = snakeCFG::SNAKE_START_DIRECTION; //Move right to start with
	prevDirection = direction;
	snakeMap[(snakeCFG::SNAKE_START_POSITION.y * display::GRID_SIZE.x) + snakeCFG::SNAKE_START_POSITION.x] = true;
	vec2 applePos = vec2(15, 4);
	int t=0;
	bool done = false;
	while (1) { //Constant loop until exit.
		t++;
		keyUpdate();
		if (isKeyPressed(key::EXIT) || isKeyPressed(key::MENU)) {break;} // Exit on EXIT/MENU key.
		//Change direction with the D-Pad type input. Does not allow 180* movements due to colliding with self.
		//Otherwise keep moving the same direction.
		if ((isKeyPressed(key::UP)) && (prevDirection != D_DOWN)) {direction = D_UP;}
		if ((isKeyPressed(key::DOWN)) && (prevDirection != D_UP)) {direction = D_DOWN;}
		if ((isKeyPressed(key::LEFT)) && (prevDirection != D_RIGHT)) {direction = D_LEFT;}
		if ((isKeyPressed(key::RIGHT)) && (prevDirection != D_LEFT)) {direction = D_RIGHT;}

		if (!done) {
			render::drawApple(applePos);

			if (t >= snakeCFG::MOVE_INTERVAL) {
				prevDirection = direction;

				snake = utils::movement(snake, &snakeMap, direction, applePos);
				t = 0;
			}
		}

		switch (snake.state) {
			case S_EATING:
				for (int i=0; i<snakeCFG::MAX_APPLE_RETRIES; i++) {
					applePos = vec2(
						utils::randomInRange(0, display::GRID_SIZE.x - 1),
						utils::randomInRange(0, display::GRID_SIZE.y - 1)
					);
					int packedPos = (applePos.y * display::GRID_SIZE.y) + applePos.x;
					if (!snakeMap[packedPos]) {break;}
				}
				break;
			case S_LOSE:
				render::loseScreen(snake.length);
				done = true;
				break;
			case S_WIN:
				render::winScreen(snake.length);
				done = true;
				break;
			default: //Nothing happens otherwise.
				break;
		}

		if (!done) {
			render::drawSnake(snake, direction);
			

			//Grid.
			if (display::DRAW_GRID) {
				for (int x = 0; x < 21; x++) {
					render::drawVLine(x*17, 0, 17*11, 0xCE59);
				}
				for (int y = 0; y < 12; y++) {
					render::drawHLine(0, y*17, 17*20, 0xCE59);
				}
			}

			if (display::DRAW_BOUNDARY) {
				render::drawHLine(0, 0, 17*20, 0xCE59);
				render::drawHLine(0, 11*17, 17*20, 0xCE59);

				render::drawVLine(0, 0, 17*11, 0xCE59);
				render::drawVLine(20*17, 0, 17*11, 0xCE59);
			}

			if (display::DRAW_SCORE) {
				showScore(snake.length);
			}
		}


		Bdisp_PutDisp_DD();
		Bdisp_AllClr_VRAM();
		resetPrintLN();
	}
	return 0;

}