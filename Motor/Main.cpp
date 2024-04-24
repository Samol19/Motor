#include <iostream>
#include <SDL/SDL.h>
#include "MainGame.h"


int main(int argc, char** argv) {
	srand(time(NULL));
	MainGame mainGame;
	mainGame.run();
	system("pause");
	return 0;
}
