﻿#include "Weapons.h"
#include "Candle.h"
#include "Simon.h"
#include "Ground.h"
#include "Zombie.h"
#include "BlackLeopard.h"
#include "Bat.h"
#include "FishMan.h"
#include "FireBall.h"
#include "Boss.h"

Weapons::Weapons()
{
	AddAnimation("weapon_dagger_ani");
	AddAnimation("weapon_holywater_ani");
	AddAnimation("weapon_holywatershattered_ani");
	AddAnimation("weapon_stopwatch_ani");
	AddAnimation("weapon_stopwatch_ani");
	AddAnimation("weapon_stopwatch_ani");
	AddAnimation("weapon_axe_ani");
	AddAnimation("weapon_axe_ani");
	AddAnimation("weapon_axe_ani");
	AddAnimation("weapon_axe_ani");
	AddAnimation("weapon_axe_ani");

	state = -1; // no Weapons
}

void Weapons::UpdateCollisionState()
{
	this->isEnable = false;
}

void Weapons::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	if (isHolyWaterShattered == true &&
		GetTickCount() - holyWaterShatteredCounter > WEAPONS_HOLY_WATER_TIME_EFFECT)
	{
		isHolyWaterShattered = false;
		holyWaterShatteredCounter = 0;
		this->isEnable = false;
		return;
	}

	GameObject::Update(dt);

	switch (state)
	{
	case WEAPONS_HOLY_WATER:
		vy += WEAPONS_HOLY_WATER_GRAVITY * dt;
		break;
	case WEAPONS_AXE:
		vy += WEAPONS_AXE_GRAVITY * dt;
		break;
	default:
		break;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObject, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		// kiểm tra va chạm với object
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Candle*>(e->obj))
			{
				Candle * candle = dynamic_cast<Candle*>(e->obj);
				candle->SetState(CANDLE_DESTROYED);
				SetCoordinateObject(e->obj);
				UpdateCollisionState();
			}
			else if (dynamic_cast<Ground*>(e->obj))
			{
				if (state == WEAPONS_HOLY_WATER && e->ny == -1)
					SetState(WEAPONS_HOLY_WATER_SHATTERED);
				x += dx;
				y += dy;
			}
			else if (dynamic_cast<Zombie*>(e->obj))
			{
				Zombie * zombie = dynamic_cast<Zombie*>(e->obj);
				zombie->SetState(ZOMBIE_DESTROYED);
				SetCoordinateObject(e->obj);

				UpdateCollisionState();
			}
			else if (dynamic_cast<BlackLeopard*>(e->obj))
			{
				BlackLeopard * zombie = dynamic_cast<BlackLeopard*>(e->obj);
				zombie->SetState(BLACK_LEOPARD_DESTROYED);
				SetCoordinateObject(e->obj);

				UpdateCollisionState();
			}
			else if (dynamic_cast<Bat*>(e->obj))
			{
				Bat * zombie = dynamic_cast<Bat*>(e->obj);
				zombie->SetState(BAT_DESTROYED);
				SetCoordinateObject(e->obj);

				UpdateCollisionState();
			}
			else if (dynamic_cast<FishMan*>(e->obj))
			{
				FishMan * fishman = dynamic_cast<FishMan*> (e->obj);
				fishman->SetState(FISHMAN_DESTROYED);
				SetCoordinateObject(e->obj);

				UpdateCollisionState();
			}
			else if (dynamic_cast<FireBall*>(e->obj))
			{
				FireBall * fireball = dynamic_cast<FireBall*> (e->obj);
				fireball->SetState(FIREBALL_DESTROYED);
				SetCoordinateObject(e->obj);

				UpdateCollisionState();
				
			}
			else if (dynamic_cast<Boss*>(e->obj))
			{
				Boss * boss = dynamic_cast<Boss*>(e->obj);
				boss->LoseHP(2);
				SetCoordinateObject(e->obj);

				UpdateCollisionState();
			}

		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Weapons::Render()
{
	RenderSpark();

	if (this->isEnable == true && state != WEAPONS_STOP_WATCH) {
		animations[state]->Render(nx, x, y);
	//	simon->SetState(STAND);
	}
}

void Weapons::RenderSpark()
{
	if (sparkEffectPosition.size() > 0)
	{
		if (startTimeRenderSpark == 0)
			startTimeRenderSpark = GetTickCount();
		else if (GetTickCount() - startTimeRenderSpark > SPARK_ANI_TIME_DELAY)
		{
			startTimeRenderSpark = 0;
			sparkEffectPosition.clear();
		}

		for (auto coord : sparkEffectPosition)
			spark->Render(-1, coord[0], coord[1]);
	}
}

void Weapons::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case WEAPONS_DAGGER:
		if (nx > 0) vx = WEAPONS_DAGGER_SPEED;
		else vx = -WEAPONS_DAGGER_SPEED;
		vy = 0;
		break;
	case WEAPONS_HOLY_WATER:
		vx = nx * WEAPONS_HOLY_WATER_SPEED_X;
		vy = -WEAPONS_HOLY_WATER_SPEED_Y;
		break;
	case WEAPONS_HOLY_WATER_SHATTERED:
		vx = 0;
		vy = 0;
		RenderHolyWaterEffect();
		break;
	case WEAPONS_AXE:
		if (nx > 0) vx = 0.30f;//WEAPONS_AXE_SPEED_X;
		else vx = -0.30f;
		vy = -0.35f;// WEAPONS_AXE_SPEED_Y;
		break;
	default:
		break;
	}
}

void Weapons::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;

	switch (state)
	{
	case WEAPONS_DAGGER:
		right = left + WEAPONS_DAGGER_BBOX_WIDTH;
		bottom = top + WEAPONS_DAGGER_BBOX_HEIGHT;
		break;
	case WEAPONS_HOLY_WATER:
		right = left + WEAPONS_HOLY_WATER_BBOX_WIDTH;
		bottom = top + WEAPONS_HOLY_WATER_BBOX_HEIGHT;
		break;

	case WEAPONS_AXE:
		right = left + WEAPONS_AXE_BBOX_WIDTH;
		bottom = top + WEAPONS_AXE_BBOX_HEIGHT;
		break;
	default:
		right = left;
		bottom = top;
		break;
	}
}

void Weapons::SetCoordinateObject(LPGAMEOBJECT obj)
{
	float l, t, r, b;
	obj->GetBoundingBox(l, t, r, b);
	sparkEffectPosition.push_back({ l, t });
}


