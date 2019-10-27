#include "Weapons.h"
#include "Candle.h"
#include "Simon.h"
#include "Ground.h"

Weapons::Weapons()
{
	AddAnimation("weapon_dagger_ani");
	AddAnimation("weapon_boomerang_ani");

	state = -1; // no Weapons
}

void Weapons::UpdateCollisionState()
{
	if (state == WEAPONS_DAGGER || state == WEAPONS_BOOMERANG)
		this->isEnable = false;
	else
	{
		x += dx;
		y += dy;
	}
}

void Weapons::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	GameObject::Update(dt);

	switch (state)
	{
	case WEAPONS_BOOMERANG:
		if (nx > 0) vx -= WEAPONS_BOOMERANG_TURNBACK_SPEED;
		else vx += WEAPONS_BOOMERANG_TURNBACK_SPEED;
		break;
	default:
		break;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObject, coEvents);

	static int  c = 0;

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

				targetTypeHit = CANDLE;
				GetCoordinateObject(e->obj);

				UpdateCollisionState();
			}
			else if (dynamic_cast<Simon*>(e->obj))
			{
				if (state == WEAPONS_BOOMERANG)
					this->isEnable = false;
			}
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Weapons::Render()
{
	RenderSpark();

	if (this->isEnable == true)
		animations[state]->Render(1, nx, x, y);
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

	scoreReceived = 0;
	targetTypeHit = -1;

	switch (state)
	{
	case WEAPONS_DAGGER:
		if (nx > 0) vx = WEAPONS_DAGGER_SPEED;
		else vx = -WEAPONS_DAGGER_SPEED;
		vy = 0;
		break;
	case WEAPONS_BOOMERANG:
		vx = nx * WEAPONS_BOOMERANG_SPEED;
		vy = 0;
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
	case WEAPONS_BOOMERANG:
		right = left + WEAPONS_BOOMERANG_BBOX_WIDTH;
		bottom = top + WEAPONS_BOOMERANG_BBOX_HEIGHT;
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


