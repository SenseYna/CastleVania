
#include "Simon.h"

#include "Ground.h"
#include "NextSceneObject.h"
#include "Door.h"
#include <fstream>
#include <string>

Simon::Simon() : GameObject()
{
	SetState(STAND);
	currentWeapon = -1;
	energy = 100;

	AddAnimation("simon_stand_ani");
	AddAnimation("simon_walk_ani");
	AddAnimation("simon_sit_ani");
	AddAnimation("simon_jump_ani");
	AddAnimation("simon_hitstand_ani");
	AddAnimation("simon_hitsit_ani");
	AddAnimation("simon_powerup_ani");
}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);
	if (!isAutoWalk) {
		if (vy > -SIMON_SPEED_Y_LOWER_ZONE && vy < SIMON_SPEED_Y_LOWER_ZONE && !isCollisionHead) // trọng lực khi nhảy
			vy += SIMON_GRAVITY_LOWER * dt;
		else
			vy += SIMON_GRAVITY * dt;
	}
	// Auto - walk
	if (isAutoWalk)
		DoAutoWalk();

	if (coObjects == NULL)
	{
		if (!isAutoWalk) 
		{
			x += dx;
			y += dy;
		}
		return;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	
	if (coEvents.size() == 0 && isAutoWalk == false)
	{
		x += dx;
		y += dy;

	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		if (isAutoWalk == false)
		{
			x += min_tx * dx + nx * 0.1f;
			y += min_ty * dy + ny * 0.1f;
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->ny != 0)				// Trên dưới đụng, nx là trái phải
				{
					if (e->ny == CDIR_BOTTOM) // -1 là đụng dưới
					{
						vy = 0;
						isTouchGround = true;
						isCollisionHead = false;
					}
					else
					{
						if (vy <= 0) vy = SIMON_GRAVITY;
						isCollisionHead = true;
					}
				}	
			}
			else if (dynamic_cast<Door*>(e->obj))
			{
				auto door = dynamic_cast<Door*>(e->obj);

				if (e->obj->GetState() == DOOR_1)	// đi qua cửa của scene 1
				{
					SetState(WALK);
					vx = SIMON_WALKING_SPEED_LOWER;
					vy = 0;
					AutoWalk(80);
				}
			}
			else if (dynamic_cast<NextSceneObject*>(e->obj))
			{
				x += dx;
				y += dy;

				NextSceneObject * obj = dynamic_cast<NextSceneObject*>(e->obj);

				if (obj->GetIDNextScene() == SCENE_2)
				{
					isAutoWalk = false;
					this->isNextScene = obj->GetIDNextScene();
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void Simon::Render()
{
	int tempState = state;
	int alpha = 255;
	float ratio = 0;

	animations[tempState]->Render(nx, x, y, alpha);
	animations[state]->SetFrame(animations[tempState]->GetCurrentFrame());
	
}

void Simon::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
		case STAND:
			vx = 0;
			break;

		case WALK:
			if (nx > 0) vx = SIMON_WALKING_SPEED;
			else vx = -SIMON_WALKING_SPEED;
			break;
	
		case JUMP:
			isTouchGround = false;
			vy = -SIMON_JUMP_SPEED_Y;
			animations[state]->SetAniStartTime(GetTickCount());
			break;

		case SIT:
			vx = 0;
			vy = 0;
			break;

		case HIT_STAND:
			animations[state]->Reset();
			animations[state]->SetAniStartTime(GetTickCount());
			break;

		case HIT_SIT:
			animations[state]->Reset();
			animations[state]->SetAniStartTime(GetTickCount());
			break;

		case POWER:
			animations[state]->Reset();
			animations[state]->SetAniStartTime(GetTickCount());
			break;
		default:
			break;
	}
}

void Simon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	// sprite có kích thước là 60x66, bbox là 40x62
	left = x + 15; //30,60
	top = y + 2;  //62,66
	right = left + SIMON_BBOX_WIDTH;
	bottom = top + SIMON_BBOX_HEIGHT;

}

bool Simon::isWhip()
{
	return state == HIT_SIT || state == HIT_STAND;
}

bool Simon::CheckCollisionWithItem(vector<LPGAMEOBJECT>* listItem)
{
	float simon_l, simon_t, simon_r, simon_b;
	float item_l, item_t, item_r, item_b;

	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	for (UINT i = 0; i < listItem->size(); i++)
	{
		if (listItem->at(i)->IsEnable() == false)
			continue;

		listItem->at(i)->GetBoundingBox(item_l, item_t, item_r, item_b);

		if (GameObject::AABB(simon_l, simon_t, simon_r, simon_b, item_l, item_t, item_r, item_b))
		{
			listItem->at(i)->isEnable = false;

			int idItem = listItem->at(i)->GetState();

			switch (idItem)
			{
			case DAGGER:
			case HOLY_WATER: 
				SetCurrentWeapons(idItem);
				break;
			case SMALL_HEART:
				energy += 1;
				break;
			case LARGE_HEART:
				energy += 5;
				break;
			case CHAIN:
				SetState(POWER); // đổi trạng thái power - biến hình nhấp nháy các kiểu đà điểu
				vx = 0;
				isGotChainItem = true;
				break;
			default:
				break;
			}

			return true;
		}
	}
}

void Simon::AutoWalk(float distance)
{
	isAutoWalk = true;
	autoWalkDistance = distance;

}

void Simon::DoAutoWalk()
{
	if (abs(dx) <= abs(autoWalkDistance))
	{
		x += dx;
		y += dy;
		autoWalkDistance -= dx;
	}
}







