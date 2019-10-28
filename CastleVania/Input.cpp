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
	if (simon->GetState() == HIT_STAND && simon->animations[HIT_STAND]->IsOver(HIT_ANI_TIME_DELAY) == false)
		return true;

	if (simon->GetState() == HIT_SIT && simon->animations[HIT_SIT]->IsOver(HIT_ANI_TIME_DELAY) == false)
		return true;
	
	return false;
}

bool Input::CanProcessKeyboard()
{
	if (AnimationDelay() == true)
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
		simon->isHitWeapons = false;
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

	case DIK_S:
		Simon_Whip_Weapons();
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

void Input::Simon_Whip_Weapons()
{
	vector<Weapons*> * weaponlist = scene->GetWeaponList();
	Weapons * weapon;

	if (simon->GetCurrentWeapons() == -1 || simon->GetEnergy() == 0) // không có vũ khí hoặc enery = 0
		return;
	if (weaponlist->at(0)->IsEnable() == false)
		weapon = weaponlist->at(0);
	else if (weaponlist->at(1)->IsEnable() == false)// && (scene->doubleShotTimer->IsTimeUp() == false || scene->tripleShotTimer->IsTimeUp() == false))
		weapon = weaponlist->at(1);
	else if (weaponlist->at(2)->IsEnable() == false)//  && scene->tripleShotTimer->IsTimeUp() == false)
		weapon = weaponlist->at(2);
	else return;

	if (simon->GetState() == STAND || simon->GetState() == JUMP ||
		simon->GetState() == SIT)
	{
		float sx, sy;

		// position
		simon->GetPosition(sx, sy);

		if (simon->GetState() == SIT) sy += 25.0f; // khớp vị trí tay
		else sy += 10.0f;
		if (simon->GetOrientation() < 0) sx += 30.0f;

		weapon->SetPosition(sx, sy);

		// orientation
		weapon->SetOrientation(simon->GetOrientation()); // HƯỚNG
	
		// state enable
		weapon->SetEnable(true);
		weapon->SetState(simon->GetCurrentWeapons());

		simon->LoseEnergy(1);
		simon->isHitWeapons = true;
		Simon_Whip();

	}
}




