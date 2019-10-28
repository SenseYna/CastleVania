#include "Simon.h"

#include "Ground.h"

#include <fstream>
#include <string>

Simon::Simon() : GameObject()
{
	SetState(STAND);
	currentWeapon = 0;
	energy = 100;

	AddAnimation("simon_stand_ani");
	AddAnimation("simon_walk_ani");
	AddAnimation("simon_sit_ani");
	AddAnimation("simon_jump_ani");
	AddAnimation("simon_hitstand_ani");
	AddAnimation("simon_hitsit_ani");
	



}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMovement)
{
	GameObject::Update(dt);

	if (vy > -SIMON_SPEED_Y_LOWER_ZONE && vy < SIMON_SPEED_Y_LOWER_ZONE) // trọng lực khi nhảy
		vy += SIMON_GRAVITY_LOWER*dt;
	else
		vy += SIMON_GRAVITY*dt;

	
	if (coObjects == NULL)
	{
		x += dx;
		y += dy;
		return;
	}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		/*if (vy > SIMON_SPEED_Y_LOWER_ZONE)
			isFalling = true;*/
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	/*	if (isAutoWalk == false)
		{*/
			x += min_tx*dx + nx*0.1f;
			y += min_ty*dy + ny*0.1f;
		//}


		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->ny != 0)
				{

					if (e->ny == CDIR_BOTTOM)
					{
						vy = 0;
						isTouchGround = true;
					
					}
					else
						y += dy;
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
	if (state == 4) 
			alpha = 255;
	animations[tempState]->Render(1, nx, x, y, alpha);
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

//Simon * Simon::GetInstance()
//{
//	if (_instance == NULL) _instance = new Simon();
//	return _instance;
//}






