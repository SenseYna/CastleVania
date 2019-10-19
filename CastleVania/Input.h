#pragma once

#include "Game.h"
#include "Simon.h"
#include "Scenes.h"

class Input : public KeyEventHandler
{
	Game * game;
	Scenes * scene;
	Simon * simon;
	bool isNeedToWaitingAnimation = true;

public:
	Input(Game * game, Scenes * scene);
	~Input();

	
	bool CanProcessKeyboard();

	virtual void KeyState(BYTE *state);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);

	///////

	void Simon_Walk_Left();
	void Simon_Walk_Right();
	void Simon_Jump();
	
};

