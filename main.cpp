extern "C" {
    #include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
    #include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
    #include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
}
#include "src/structs.h"
#include "src/constants.h"
#include "src/utils.h"
using namespace std;
using namespace utils;

int key;

int main() {
    //SDK Initialisation
    Bdisp_EnableColor(1); //Enable 16b colour;
    Bdisp_AllClr_VRAM();
    Bdisp_PutDisp_DD();

    int t=0;
    while (1) { //Constant loop until exit.
        utils::keyUpdateAlt();
        t++;
        //Handle input
        if (utils::isKeyPressed(0x0408) || t > 500) {
            key = 0;
            break; //Exit program immediately.
        }


        //Display some text;
        printTXT((const char*)"  Running template.g3a", 1);


        Bdisp_PutDisp_DD();
        resetPrintLN();
    }
    return 0;

}