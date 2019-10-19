#include "Input.h"



Input::Input(Game * game, Scenes * scene)
{
	this->game = game;
	this->scene = scene;
}

Input::~Input()
{
}

bool Input::CanProcessKeyboard()
{
	if (scene->IsMovingCamera() == true)
		return false;

	

	return true;
}

void Input::KeyState(BYTE *state)
{
	simon = scene->GetSimon();

	if (CanProcessKeyboard() == false)
		return;

	// nếu simon đang nhảy và chưa chạm đất
	if ((simon->GetState() == JUMP || simon->GetState() == STAND)
		&& simon->isTouchGround == false)
		return;

	// Xét trạng thái phím
	if (game->IsKeyDown(DIK_RIGHT))
	{
		Simon_Walk_Right();
		
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		Simon_Walk_Left();
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (simon->isTouchGround == false)
		{
			simon->SetState(STAND);
			return;
		}

		simon->SetState(SIT);
	}
	else
	{
		simon->SetState(STAND);
	}
}

void Input::OnKeyDown(int KeyCode)
{
	

	switch (KeyCode)
	{
	
	case DIK_SPACE:
		Simon_Jump();
		break;

	default:
		break;
	}
}

void Input::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void Input::Simon_Walk_Left()
{
	simon->SetOrientation(-1);
	simon->SetState(WALK);
}

void Input::Simon_Walk_Right()
{
	simon->SetOrientation(1);
	simon->SetState(WALK);
}

void Input::Simon_Jump()
{
	if (simon->isTouchGround == false)
		return;

	simon->SetState(JUMP);
}





