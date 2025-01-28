#include "game.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;
const char* TITLE = "Pong game from scratch";
const float BG_COLOR[] = {94, 70, 152};

int main(){
	gameLoop(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, BG_COLOR);
	return 0;
}