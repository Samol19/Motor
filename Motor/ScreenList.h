#pragma once
#include<vector>
#include "IGameScreen.h"
#include "Game.h"

class Game;
class IGameScreen;
class ScreenList
{
public:
	ScreenList(Game* game);
	~ScreenList();
	IGameScreen* moveNext();
	IGameScreen* movePrevious();
	void setScreen(int nextScreen);
	void addScreen(IGameScreen* newScreen);
	void destroy();
	IGameScreen* getCurrent();
protected:
	std::vector<IGameScreen*> screens;
	Game* game;
	int currentIndex = -1;

};

