#ifndef RENDER_H
#define RENDER_H


namespace render {

void drawSnake(Snake snakeInstance, int direction);
void drawApple(vec2 applePos);


unsigned short createColour(unsigned char r, unsigned char g, unsigned char b);
void drawPixel(int x, int y, unsigned short colour);

void loseScreen(int score);
void winScreen();

void drawVLine(int x, int y, int length, unsigned short colour);
void drawHLine(int x, int y, int length, unsigned short colour);

}

#endif