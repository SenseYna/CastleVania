#include "FireBall.h"



FireBall::FireBall()
{
	AddAnimation("fireball_ani");
	AddAnimation("effect1_ani");

	SetState(FIREBALL_ACTIVE);
}

void FireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (state == FIREBALL_DESTROYED && animations[state]->IsOver(EFFECT_ANI_TIME_DELAY) == true)
	{
		this->isEnable = false;
		return;
	}


	GameObject::Update(dt);
	x += dx;
}

void FireBall::Render()
{
	animations[state]->Render(nx, x, y);
}

void FireBall::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case FIREBALL_ACTIVE:
		if (nx == 1) vx = FIREBALL_SPEED;
		else vx = -FIREBALL_SPEED;
		vy = 0;
		break;
	case FIREBALL_DESTROYED:
		vx = vy = 0;
		animations[state]->SetAniStartTime(GetTickCount());
		break;
	default:
		break;
	}
}

void FireBall::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + FIREBALL_BBOX_WIDTH;
	bottom = top + FIREBALL_BBOX_HEIGHT;
}
