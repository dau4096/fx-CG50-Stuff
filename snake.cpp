extern "C" {
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/system.h"
}

#include <stdlib.h>
#include "src/constants.h"
#include "src/utils.h"
#include "src/render.h"


int key, direction;


int keyPressed(int basic_keycode){
    const unsigned short* keyboard_register = (unsigned short*)0xA44B0000;
    int row, col, word, bit;
    row = basic_keycode%10;
    col = basic_keycode/10-1;
    word = row>>1;
    bit = col + ((row&1)<<3);
    return (0 != (keyboard_register[word] & 1<<bit));
}

void wait(int ms) {
	int start_value = RTC_GetTicks();
	while (RTC_Elapsed_ms(start_value, ms) < ms) {
	}
}

int main() {
	//SDK Initialisation
	Bdisp_EnableColor(1); //Enable 16b colour;
	Bdisp_AllClr_VRAM();
	Bdisp_PutDisp_DD();

	srand((unsigned int)RTC_GetTicks());

	Snake snake = utils::createSnake(vec2(0, 3)); //Start point at left centre
	direction = RIGHT; //Move right to start with


	Snake testSnake = utils::createSnake(vec2(15, 3)); //Head
	testSnake.segments[1] = vec2(14, 3);
	testSnake.segments[2] = vec2(13, 3);
	testSnake.segments[3] = vec2(12, 3);
	testSnake.segments[4] = vec2(11, 3);
	testSnake.segments[5] = vec2(11, 4);
	testSnake.segments[6] = vec2(11, 5);
	testSnake.segments[7] = vec2(10, 5);
	testSnake.segments[8] = vec2(10, 4);
	testSnake.length = 9;


	vec2 applePos(
		utils::randomInRange(0, gridSize.x),
		utils::randomInRange(0, gridSize.y)
	);

	int frame = 0;
	while (1) { //Constant loop until exit.
		//GetKey(&key); //Requires keypress to see next frame; fix somehow.
		//Handle input
		if (key == KEY_CTRL_EXIT || frame > 100) {
			key = 0;
			break; //Exit program immediately.
		}
		frame++;


		//Change direction with the D-Pad type input.
		if (key == KEY_CTRL_UP) direction = UP;
		if (key == KEY_CTRL_DOWN) direction = DOWN;
		if (key == KEY_CTRL_LEFT) direction = LEFT;
		if (key == KEY_CTRL_RIGHT) direction = RIGHT;
		//Otherwise keep moving the same direction.


		render::drawApple(applePos);

		//snake = utils::movement(snake, direction, applePos);

		switch (snake.state) {
			case EATING:
				applePos = vec2(
					utils::randomInRange(0, gridSize.x),
					utils::randomInRange(0, gridSize.y)
				);
				break;
			case LOSE:
				render::loseScreen(snake.length);
				return 0;
			case WIN:
				render::winScreen();
				return 0;
			default: //Nothing happens otherwise.
				break;
		}

		render::drawSnake(snake, direction);
		
		/*
		//!Replace with valid function!
		wait(100); //10Hz-ish.
		*/

		//Grid.
		for (int x = 0; x < 23; x++) {
			render::drawVLine(x*17, 0, 17*11, 0xCE59);
		}
		for (int y = 0; y < 12; y++) {
			render::drawHLine(0, y*17, 17*22, 0xCE59);
		}


		//render::drawSnake(testSnake, RIGHT);
		//render::drawApple(vec2(17, 3));


		Bdisp_PutDisp_DD();
		Bdisp_AllClr_VRAM();
	}
	return 0;

}