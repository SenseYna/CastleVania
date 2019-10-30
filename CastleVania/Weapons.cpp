#include "Weapons.h"
#include "Candle.h"
#include "Simon.h"
#include "Ground.h"

Weapons::Weapons()
{
	AddAnimation("weapon_dagger_ani");
	state = -1; // no Weapons
}

void Weapons::UpdateCollisionState()
{
	this->isEnable = false;
}

void Weapons::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	GameObject::Update(dt);

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
				GetCoordinateObject(e->obj);
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

	if (this->isEnable == true) {
		animations[state]->Render(1, nx, x, y);
	//	simon->SetState(STAND);
	}
}

void Weapons::RenderSpark()
{
	if (sparkCoord.size() > 0)
	{
		if (startTimeRenderSpark == 0)
			startTimeRenderSpark = GetTickCount();
		else if (GetTickCount() - startTimeRenderSpark > SPARK_ANI_TIME_DELAY)
		{
			startTimeRenderSpark = 0;
			sparkCoord.clear();
		}

		for (auto coord : sparkCoord)
			spark->Render(1, -1, coord[0], coord[1]);
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
	default:
		right = left;
		bottom = top;
		break;
	}
}

void Weapons::GetCoordinateObject(LPGAMEOBJECT obj)
{
	float l, t, r, b;
	obj->GetBoundingBox(l, t, r, b);
	sparkCoord.push_back({ l, t });
}


