﻿#include "Input.h"



Input::Input(Game * game, Scenes * scene)
{
	this->game = game;
	this->scene = scene;
	//simon = scene->GetSimon();
}

Input::~Input()
{
}

bool Input::AnimationDelay()
{
	if (simon->GetState() == POWER && simon->animations[POWER]->IsOver(POWER_ANI_TIME_DELAY) == false)
		return true;

	if (simon->GetState() == HIT_STAND && simon->animations[HIT_STAND]->IsOver(HIT_ANI_TIME_DELAY) == false)
		return true;

	if (simon->GetState() == HIT_SIT && simon->animations[HIT_SIT]->IsOver(HIT_ANI_TIME_DELAY) == false)
		return true;
	if (simon->GetState() == STAIR_UP && simon->animations[STAIR_UP]->IsOver(STAIR_WALK_ANI_TIME_DELAY) == false)
		return true;

	if (simon->GetState() == STAIR_DOWN && simon->animations[STAIR_DOWN]->IsOver(STAIR_WALK_ANI_TIME_DELAY) == false)
		return true;
	if (simon->GetState() == HIT_STAIR_UP && simon->animations[HIT_STAIR_UP]->IsOver(HIT_ANI_TIME_DELAY) == false)
		return true;
	if (simon->GetState() == HIT_STAIR_DOWN && simon->animations[HIT_STAIR_DOWN]->IsOver(HIT_ANI_TIME_DELAY) == false)
		return true;
	if (simon->GetState() == DEFLECT && simon->animations[DEFLECT]->IsOver(DEFLECT_ANI_TIME_DELAY) == false)
		return true;
	if (simon->GetState() == SIT && simon->isDelayHightGravitySit == true && simon->animations[SIT]->IsOver(SIT_ANI_TIME_DELAY) == false)
	{
		
		return true;
	}
	simon->isDelayHightGravitySit = false;
	return false;
}

bool Input::CanProcessKeyboard()
{
	if (!(simon->autoWalkScene2_1Timer->IsTimeUp()))
		return false;
	if (AnimationDelay() == true)
		return false;
	if (simon->isAutoWalk == true)
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
		if (simon->isStandOnStair)// && Check_Simon_Collection_Stair() == true)
		{
			Check_Simon_Collection_Stair();
			if (abs(simon->stairCollided->GetState()) == 2) {
				if (simon->stairDirection) // cầu thang trái dưới - phải trên
					Simon_Stair_Down();
				else 
					Simon_Stair_Up(); // cầu thang trái dưới - phải trên

				return;
			}
			if (simon->stairDirection) // cầu thang trái dưới - phải trên
				Simon_Stair_Up();
			else // cầu thang trái dưới - phải trên
				Simon_Stair_Down();
			return;
		}
		Simon_Walk_Right();
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (simon->isStandOnStair)// && Check_Simon_Collection_Stair() == true)
		{
			Check_Simon_Collection_Stair();
			if (abs(simon->stairCollided->GetState()) == 2) {
				if (simon->stairDirection) // cầu thang trái dưới - phải trên
					Simon_Stair_Up();
				else Simon_Stair_Down(); // cầu thang trái dưới - phải trên

				return;
			}
			if (simon->stairDirection) // cầu thang trái dưới - phải trên
				Simon_Stair_Down();
			else Simon_Stair_Up(); // cầu thang trái dưới - phải trên
				
			return;
		}
		Simon_Walk_Left();
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (Check_Simon_Collection_Stair() == true || simon->isStandOnStair)
		{
			if (Simon_Stair_Down()) return;	
		}

		if (simon->isTouchGround == false)
		{
			simon->SetState(STAND);
			return;
		}

		simon->SetState(SIT);
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		if (Check_Simon_Collection_Stair() == true || simon->isStandOnStair)
		{
			Simon_Stair_Up();
			return;
		}

		simon->SetState(STAND);
	}
	else
	{
		simon->isHitWeapons = false;

		if (simon->isStandOnStair)
		{
			if (Simon_Stair_Stand())
				return;
		}
		simon->SetState(STAND);
	}
}

void Input::OnKeyDown(int KeyCode)
{
	if (!(simon->autoWalkScene2_1Timer->IsTimeUp()))
		return;

	switch (KeyCode)
	{
	case DIK_A:
		if (game->IsKeyDown(DIK_UP))
			Simon_Whip_Weapons();
		else
			Simon_Whip();
		break;
	case DIK_S:
		Simon_Whip_Weapons();
		break;
	case DIK_Q:
		scene->Init(SCENE_2, GAMESTATE_1);
		break;
	case DIK_W:
		scene->Init(SCENE_2, GAMESTATE_2);
		break;
	case DIK_E:
		scene->Init(SCENE_2, GAMESTATE_3);
		break;
	case DIK_R:
		scene->Init(SCENE_2, GAMESTATE_4);
		break;
	case DIK_T:
		scene->Init(SCENE_3, GAMESTATE_1);
		break;
	case DIK_Y:
		scene->Init(SCENE_3, GAMESTATE_2);
		break;
	case DIK_SPACE:
		if (simon->isStandOnStair == false) Simon_Jump();
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
	else if (simon->GetState() == STAIR_UP)
	{
		simon->SetState(HIT_STAIR_UP);
	}
	else if (simon->GetState() == STAIR_DOWN)
	{
		simon->SetState(HIT_STAIR_DOWN);
	}
}

void Input::Simon_Whip_Weapons()
{
	if (simon->GetCurrentWeapons() == -1) return; //chưa nhặt weapon thì return

	vector<Weapons*> * weaponlist = scene->GetWeaponList();
	Weapons * weapon;

	if (simon->GetCurrentWeapons() == -1 || simon->GetEnergy() == 0) // không có vũ khí hoặc enery = 0
		return;

	if (simon->GetCurrentWeapons() == STOPWATCH)
	{
		if (scene->stopWatchTimer->IsTimeUp() == false) // đang sử dụng stop watch
			return;

		weaponlist->at(0)->SetEnable(false);
	}

	if (weaponlist->at(0)->IsEnable() == false) // weapon đã bắn chưa , rồi thì đợi
		weapon = weaponlist->at(0);
	else return;

	

	if (simon->GetState() == STAND || simon->GetState() == JUMP ||
		simon->GetState() == SIT || simon->GetState() == STAIR_UP || 
		simon->GetState() == STAIR_DOWN)
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
		if (weapon->GetState() == STOPWATCH)
		{
			scene->stopWatchTimer->Start();
		}
		simon->LoseEnergy(1);
		simon->isHitWeapons = true;
		Simon_Whip();

	}
}

bool Input::Check_Simon_Collection_Stair()
{
	return simon->CheckRightnessCollisionWithStair(scene->GetListStairs());
}

void Input::Simon_Stair_Up()
{
	int stairDirection = simon->stairDirection;

	if (!(simon->needStateMoveUpStair) && simon->isStandOnStair)
	{
			int nx = simon->stairDirection;
			simon->SetOrientation(nx);
			simon->SetState(STAIR_UP);
			simon->isStandOnStair = false;
			simon->col_stair_bot = false;
			simon->needStateMoveDownStair = false;
			simon->AutoWalk(abs(18 *nx), STAND, stairDirection);
			return;
	}

	if (simon->needStateMoveUpStair)
	{
		if (simon->isStandOnStair == false)
		{
			float stair_x, simon_x, temp_y;

			simon->stairCollided->GetPosition(stair_x, temp_y);
			simon->GetPosition(simon_x, temp_y);

			if (stairDirection == 1) stair_x -= 33.0f; // trừ hao để x của simon + width simon là vừa đứng trước cầu thang (xem **)
			//else (stairDirection == -1) stair_x -= 
			if (stair_x < simon_x) simon->SetOrientation(-1);
			else if (stair_x > simon_x)  simon->SetOrientation(1);
			else return;

			simon->SetState(WALK);
			simon->vy = 0;
			simon->AutoWalk(abs(stair_x - simon_x), STAIR_UP, stairDirection); // **
			simon->isStandOnStair = true;
			return;
		} 
		else
		{
			simon->SetOrientation(stairDirection);
			simon->SetState(STAIR_UP);
		}

	}
	
	return;
}

bool Input::Simon_Stair_Down()
{
	int stairDirection = -simon->stairDirection;

	if (!(simon->needStateMoveDownStair) && simon->isStandOnStair)
	{
		int nx = -simon->stairDirection;
		simon->SetOrientation(nx);
		simon->SetState(STAIR_DOWN);
		simon->isStandOnStair = false;
		simon->needStateMoveUpStair = false;
		simon->col_stair_bot = false;
		simon->AutoWalk(abs(30 * nx), STAND, nx);
		return 1;
	}

	if (simon->needStateMoveDownStair)
	{
		if (simon->isStandOnStair == false)
		{
			float stair_x, simon_x, temp_y;

			simon->stairCollided->GetPosition(stair_x, temp_y);
			simon->GetPosition(simon_x, temp_y);

			if (stairDirection == -1) stair_x -= 15.0f;
			else if (stairDirection == 1) stair_x -= 25.0f;

			if (stair_x < simon_x) simon->SetOrientation(-1);
			else if (stair_x > simon_x)  simon->SetOrientation(1);
			else return 1;

			simon->SetState(WALK);
			simon->vy = 0;
			simon->AutoWalk(abs(stair_x - simon_x), STAIR_DOWN, stairDirection); // **
			simon->isStandOnStair = true;
			return 1;
		}
		else
		{
			if (simon->col_stair_bot && stairDirection == -1) {
				simon->x += 8.0f;
				simon->col_stair_bot = false;
			}
			/*else if (simon->col_stair_bot && stairDirection == -1) {
				simon->x += 6.0f;
				simon->col_stair_bot = false;
			}*/
			simon->SetOrientation(stairDirection);
			simon->SetState(STAIR_DOWN);
			return 1;
		}

	}

	return 0;
}

bool Input::Simon_Stair_Stand()
{
	if (simon->GetState() == STAIR_UP || simon->GetState() == STAIR_DOWN ||
		simon->GetState() == HIT_STAIR_UP || simon->GetState() == HIT_STAIR_DOWN)
	{
		
		simon->animations[STAIR_UP]->Reset();
		simon->animations[STAIR_DOWN]->Reset();

		if (simon->GetState() == HIT_STAIR_UP)
		{
			simon->SetState(STAIR_UP);
			simon->animations[STAIR_UP]->SetAniStartTime(0);
			//isNeedToWaitingAnimation = false;
		}
		else if (simon->GetState() == HIT_STAIR_DOWN)
		{
			simon->SetState(STAIR_DOWN);
			simon->animations[STAIR_DOWN]->SetAniStartTime(0);
			//isNeedToWaitingAnimation = false;
		}

		simon->StandOnStair();
		return true;
	}

	return false;
}