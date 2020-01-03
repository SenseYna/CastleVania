#include "Items.h"
#include "Water.h"

Items::Items() : GameObject()
{
	AddAnimation("item_dagger_ani");
	AddAnimation("item_holywater_ani");
	AddAnimation("item_smallheart_ani");
	AddAnimation("item_largeheart_ani");
	AddAnimation("item_chain_ani");
	AddAnimation("item_stopwatch_ani");
	AddAnimation("item_porkchop_ani");
	AddAnimation("item_cross_ani");
	AddAnimation("item_invisibilitypotion_ani");
	AddAnimation("item_bluemoneybag_ani");
	AddAnimation("item_axe_ani");
	AddAnimation("item_bluemoneybag_ani_secrect");

	timeAppear = -1;
}

void Items::Render()
{
	int alpha = 255;

	if (GetTickCount() - timeAppear > ITEM_TIME_DESTROYED / 2)
		alpha -= 100 * (rand() % 2);

	animations[state]->Render(-1, x, y, alpha);
}

void Items::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	if (timeAppear == -1)
		timeAppear = GetTickCount();
	else
	{
		DWORD now = GetTickCount();

		if (now - timeAppear > ITEM_TIME_DESTROYED)
		{
			isEnable = false;
			return;
		}
	}

	if (state == MONEY_BAG_BLUE_SECRECT) {
		vy += 0.0013;
	}

	if (state == SMALL_HEART && vy != 0)
	{
		vx -= velocityVariation_x;
		if (vx >= ITEM_SMALLHEART_FALLING_SPEED_X_MAX || vx <= -ITEM_SMALLHEART_FALLING_SPEED_X_MAX)
			velocityVariation_x *= -1; // đổi chiều
	}
	GameObject::Update(dt);

	// Check collision between item and ground (falling on ground)
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
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground*>(e->obj) || dynamic_cast<BreakWall*>(e->obj))
			{
				if (e->ny == CDIR_BOTTOM)				// Hướng va chạm là bên dưới
				{
					vx = 0;
					vy = 0;
		
				}
			}
			else if (dynamic_cast<Water*>(e->obj))
			{
				this->isEnable = false;
				Water * water = dynamic_cast<Water*>(e->obj);
				water->AddBubbles(x, y);
			}
		}
		
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Items::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;

	switch (state)
	{
	case DAGGER:
		right = left + 32;
		bottom = top + 18;
		break;
	case SMALL_HEART:
		right = left + 16;
		bottom = top + 16;
		break;
	case LARGE_HEART:
		right = left + 24;
		bottom = top + 20;
		break;
	case CHAIN:
		right = left + 32;
		bottom = top + 32;
		break;
	case HOLY_WATER:
		right = left + 32;
		bottom = top + 28;
		break;
	case STOPWATCH:
		right = left + 32;
		bottom = top + 28;
		break;
	case PORK_CHOP:
		right = left + 32;
		bottom = top + 26;
		break;
	case CROSS:
		right = left + 32;
		bottom = top + 32;
		break;
	case INVISIBILITY_POTION:
		right = left + 29;
		bottom = top + 36;
		break;
	case MONEY_BAG_BLUE:
		right = left + 30;
		bottom = top + 30;
		break;
	case AXE:
		right = left + 30;
		bottom = top + 28;
		break;
	case MONEY_BAG_BLUE_SECRECT:
		right = left + 30;
		bottom = top + 30;
		break;
	default:
		right = left;
		bottom = top;
		break;
	}
}

void Items::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case SMALL_HEART:
		velocityVariation_x = ITEM_FALLING_SPEED_X_VARIATION;
		vx = ITEM_FALLING_SPEED_X;
		vy = ITEM_SMALLHEART_FALLING_SPEED_Y;
		break;
	case MONEY_BAG_BLUE_SECRECT:
		vx = ITEM_FALLING_SPEED_X;
		vy = -ITEM_SECRECT_SPEED_Y;
		break;
	default:
		vx = ITEM_FALLING_SPEED_X;
		vy = ITEM_FALLING_SPEED_Y;
		break;
	}
}
