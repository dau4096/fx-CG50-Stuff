extern "C" {
    #include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
    #include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
    #include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
}

int key;

int main() {
    //SDK Initialisation
    Bdisp_EnableColor(1); //Enable 16b colour;
    Bdisp_AllClr_VRAM();
    Bdisp_PutDisp_DD();


    while (1) { //Constant loop until exit.
        GetKey(&key);
        //Handle input
        if (key == KEY_CTRL_EXIT) {
            key = 0;
            break; //Exit program immediately.
        }
        


        Bdisp_PutDisp_DD();
    }
    return 0;

}