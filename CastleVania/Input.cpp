#include "Input.h"



Input::Input(Game * game, Scenes * scene)
{
	this->game = game;
	this->scene = scene;
}

Input::~Input()
{
}

bool Input::AnimationDelay()
{
	if (isNeedToWaitingAnimation == true)
	{
		if (simon->GetState() == HIT_STAND && simon->animations[HIT_STAND]->IsOver(HIT_ANI_TIME_DELAY) == false)
			return true;

		if (simon->GetState() == HIT_SIT && simon->animations[HIT_SIT]->IsOver(HIT_ANI_TIME_DELAY) == false)
			return true;



	}
	else
	{
		// Đặt lại biến chờ render animation
		isNeedToWaitingAnimation = true;

	}

	return false;
}

void Input::KeyState(BYTE *state)
{
	simon = scene->GetSimon();

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
	case DIK_A:
		Simon_Whip();
		break;
	
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

void Input::Simon_Whip()
{
	if (simon->isWhip() == true)
		return;

	if (simon->GetState() == STAND || simon->GetState() == JUMP)
	{
		simon->SetState(HIT_STAND);
	}
	else if (simon->GetState() == SIT)
	{
		simon->SetState(HIT_SIT);
	}
}





