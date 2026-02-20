#include <fxcg/display.h>
#include <fxcg/keyboard.h>


int main() {
	Bdisp_AllClr_VRAM();

	PrintXY(1, 1, "  Hello, world!", 0, TEXT_COLOR_BLACK);

	Bdisp_PutDisp_DD();

	int key;
	while (1) {
		if (GetKey(&key)) {
			if (key == KEY_CTRL_EXE) break; // EXE
		}
	}

	return 0;
}
