#include <iostream>
#include <SDL/SDL.h>
#include "MainGame.h"

//INSTALAR GLEW32 E INTRODUCIRLO EN EL DEBUG DE LA CARPETA DEL DEBUG


int main(int argc, char** argv) {
	srand(time(NULL));
	MainGame mainGame;
	mainGame.run();
	system("pause");
	return 0;
}
