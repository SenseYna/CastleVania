#include "Whip.h"
#include "Candle.h"
#include "Zombie.h"
#include "BlackLeopard.h"
#include "Bat.h"
#include "FishMan.h"
#include "FireBall.h"
#include "BreakWall.h"

Whip::Whip() : GameObject()
{
	AddAnimation("normalwhip_ani");
	AddAnimation("shortchain_ani");
	AddAnimation("longchain_ani");
	SetState(NORMAL_WHIP);
}

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<BreakWall*>(obj))
		{
			BreakWall * e = dynamic_cast<BreakWall*>(obj);

			float left, top, right, bottom;
			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				e->SetState(BREAK);
				//targetTypeHit = BREAKWALL;
			}
		}

		else if (dynamic_cast<Candle*>(obj)) // va chạm giữa roi và nến
		{
			Candle * e = dynamic_cast<Candle*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom)) 
			{
				e->SetState(CANDLE_DESTROYED);
				sparkCoord.push_back({ left, top });
			}
		}
		else if (dynamic_cast<Zombie*>(obj))
		{
			Zombie * e = dynamic_cast<Zombie*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) // va chạm giữa roi và zombie
			{
				e->SetState(ZOMBIE_DESTROYED);
				sparkCoord.push_back({ left, top });
			}
		}
		else if (dynamic_cast<BlackLeopard*>(obj))
		{
			BlackLeopard * e = dynamic_cast<BlackLeopard*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) // va chạm giữa roi và BlackLeopard
			{
				e->SetState(BLACK_LEOPARD_DESTROYED);
				sparkCoord.push_back({ left, top });
			}
		}
		else if (dynamic_cast<Bat*>(obj))
		{
			Bat * e = dynamic_cast<Bat*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) // va chạm giữa roi và Bat
			{
				e->SetState(BAT_DESTROYED);
				sparkCoord.push_back({ left, top });
			}
		}
		else if (dynamic_cast<FishMan*>(obj))
		{
			FishMan * e = dynamic_cast<FishMan*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) // va chạm giữa roi và FishMan
			{
				e->SetState(FISHMAN_DESTROYED);
				sparkCoord.push_back({ left, top });
			}
		}
		else if (dynamic_cast<FireBall*>(obj))
		{
			FireBall * e = dynamic_cast<FireBall*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) // va chạm giữa roi và FireBall
			{
				e->SetState(FIREBALL_DESTROYED);
				sparkCoord.push_back({ left, top });
			}
		}
	}
}

void Whip::Render(int currentID)
{
	RenderSpark();

	if (currentID >= 0)
		animations[state]->RenderByID(currentID, nx, x, y);

}

void Whip::RenderSpark()
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
			spark->Render(-1, coord[0], coord[1]);
	}
}

void Whip::SetWhipPosition(D3DXVECTOR3 simonPositon, bool isStand)
{
	simonPositon.x -= 90;
	if (isStand == false)
		simonPositon.y += 15;

	SetPosition(simonPositon.x, simonPositon.y);
}

bool Whip::CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left,
		whip_top,
		whip_right,
		whip_bottom;

	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);

	return GameObject::AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
}

void Whip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	top = y + 15;
	bottom = top + WHIP_BBOX_HEIGHT;
	if (nx < 0)
	{
		if (state != LONG_CHAIN)
			left = x + 50;
		else left = x + 20;
	}
	else if (nx > 0)
	{
		if (state != LONG_CHAIN)
			left = (240 - 50) - WHIP_BBOX_WIDTH + x;
		else left = (240 - 20) - LONG_CHAIN_BBOX_WIDTH + x;
	}

	if (state != LONG_CHAIN)
		right = left + WHIP_BBOX_WIDTH;
	else  right = left + LONG_CHAIN_BBOX_WIDTH;
}

void Whip::PowerUp()
{
	if (state == NORMAL_WHIP) SetState(SHORT_CHAIN);
	else if (state == SHORT_CHAIN) SetState(LONG_CHAIN);
}
