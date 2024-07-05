#pragma once
#include "Game.h"

enum class ScreenState {
	NONE,
	RUNNING,
	EXIT_APPLICATION,
	CHANGE_NEXT,
	CHANGE_PREVIEW
};

class Game;
class IGameScreen
{
public: // lo siento, pero no funciona con protected, no se por que. Pero de entregar, entrego TA3 y TA4
	int screenIndex = 1;
	ScreenState currentState = ScreenState::NONE;
	Game* game = nullptr;

	friend class ScreenList; // para usar sus metodos y att privados en ScreenList

	// funciones abstractas obligar a mis hijos sobreescribir 

	virtual void checkInput() = 0;
	virtual void initSystem() = 0;
	virtual void build() = 0;
	virtual void destroy() = 0;
	virtual void onExit() = 0;
	virtual void onEntry() = 0;
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual int getNextScreen() const = 0;
	virtual int getPreviousScreen() const = 0;

	ScreenState getState() const {
		return currentState;
	}

	int getIndex() const {
		return screenIndex;
	}

	void setRunning() {
		currentState = ScreenState::RUNNING;
	}

	void setParent(Game* game) {
		this->game = game;
	}

};

