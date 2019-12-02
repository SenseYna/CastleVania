#include "Bat.h"



Bat::Bat() : Enemy()
{
	AddAnimation("bat_fly_ani");
	AddAnimation("effect1_ani");

	velocityVariation = 0.004f;
	respawnWaitingTime = 5000;
}

void Bat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (state == BAT_DESTROYED && animations[state]->IsOver(EFFECT_ANI_TIME_DELAY) == true)
	{
		SetState(BAT_INACTIVE);
		return;
	}

	if (state == BAT_INACTIVE)
		return;

	Enemy::Update(dt);

	vy += velocityVariation;

	if (vy >= BAT_FLYING_SPEED_Y || vy <= -BAT_FLYING_SPEED_Y)
		velocityVariation *= -1;

	x += dx;
	y += dy;
}

void Bat::Render()
{
	if (state != BAT_INACTIVE)
		animations[state]->Render(nx, x, y);
}

void Bat::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case BAT_ACTIVE:
		if (nx > 0) vx = BAT_FLYING_SPEED_X;
		else vx = -BAT_FLYING_SPEED_X;
		vy = 0;
		isDroppedItem = false;
		respawnTime_Start = 0;
		isRespawnWaiting = false;
		break;
	case BAT_DESTROYED:
		vx = 0;
		vy = 0;
		animations[state]->SetAniStartTime(GetTickCount());
		break;
	case BAT_INACTIVE:
		x = entryPosition.x;
		y = entryPosition.y;
		vx = 0;
		vy = 0;
		isSettedPosition = false;
		StartRespawnTimeCounter();
		break;
	default:
		break;
	}
}

void Bat::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x + 11; // 10,32
	top = y + 11;
	right = left + BAT_BBOX_WIDTH;
	bottom = top + BAT_BBOX_HEIGHT;
}

void Bat::GetActiveBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = entryPosition.x - BAT_ACTIVE_BBOX_WIDTH;
	top = entryPosition.y - BAT_ACTIVE_BBOX_HEIGHT;
	right = entryPosition.x + BAT_ACTIVE_BBOX_WIDTH;
	bottom = entryPosition.y + BAT_ACTIVE_BBOX_HEIGHT;
}
