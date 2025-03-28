extern "C" {
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
}

#include "src\utils.h"
#include "src\render.h"
using namespace utils;


int key;

/*

int keyPressed(int basic_keycode){
	const unsigned short* keyboard_register = (unsigned short*)0xA44B0000;
	int row, col, word, bit;
	row = basic_keycode%10;
	col = basic_keycode/10-1;
	word = row>>1;
	bit = col + ((row&1)<<3);
	return (0 != (keyboard_register[word] & 1<<bit));
}

*/




int main() {
	Tile tileGrid[display::MAX_TILES] = {
		Tile(0, &(material::CYAN)),
		Tile(12, &(material::MAGENTA)),
	};
	tileGrid[0].valid = true;

	//SDK Initialisation
	Bdisp_EnableColor(1); //Enable 16b
	Bdisp_AllClr_VRAM();
	Bdisp_PutDisp_DD();




	while (1) { //Constant loop until exit.
		//Handle input
		if (key == KEY_CTRL_EXIT) {
			key = 0;
			break; //Exit program immediately.
		}

		render::showTiles(&tileGrid);


		Bdisp_PutDisp_DD();
		GetKey(&key);
		resetPrintLN();
	}
	return 0;

}

