#pragma once

#include "Game.h"
#include "Scenes.h"


class Input : public KeyEventHandler
{
	Game * game;
	Scenes * scene;
	Simon * simon;

public:
	Input(Game * game, Scenes * scene);
	~Input();
	
	//function
	bool AnimationDelay();
	bool CanProcessKeyboard();

	//Key State
	virtual void KeyState(BYTE *state);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);

	// Simon Action 
	void Simon_Walk_Left();
	void Simon_Walk_Right();
	void Simon_Jump();
	void Simon_Whip();
	void Simon_Whip_Weapons();
	
};

