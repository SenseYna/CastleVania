#include "Weapons.h"
#include "Candle.h"
#include "Simon.h"
#include "Ground.h"

Weapons::Weapons()
{
	AddAnimation("weapon_dagger_ani");
	AddAnimation("weapon_holywater_ani");
	AddAnimation("weapon_holywatershattered_ani");

	state = -1; // no Weapons
}

void Weapons::UpdateCollisionState()
{
	this->isEnable = false;
}

void Weapons::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
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
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Weapons::Render()
{
	RenderSpark();

	if (this->isEnable == true) {
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


