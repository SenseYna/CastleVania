#include "Simon.h"

#include "Ground.h"

#include <fstream>
#include <string>

Simon::Simon() : GameObject()
{
	SetState(STAND);

	AddAnimation("simon_stand_ani");
	AddAnimation("simon_walk_ani");
	AddAnimation("simon_sit_ani");
	AddAnimation("simon_jump_ani");

	AddAnimation("simon_stand_invisible_ani");
	AddAnimation("simon_walk_invisible_ani");
	AddAnimation("simon_sit_invisible_ani");
	AddAnimation("simon_jump_invisible_ani");



}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMovement)
{
	GameObject::Update(dt);

	if (vy > -SIMON_SPEED_Y_LOWER_ZONE && vy < SIMON_SPEED_Y_LOWER_ZONE)
		vy += SIMON_GRAVITY_LOWER*dt;
	else
		vy += SIMON_GRAVITY*dt;

	// Reset untouchable timer if untouchable time has passed
	if (untouchableTimer->IsTimeUp() == true)
		untouchableTimer->Stop();

	// Reset invisibility timer if invisibility time has passed
	if (invisibilityTimer->IsTimeUp() == true)
		invisibilityTimer->Stop();

	if (coObjects == NULL)
	{
		x += dx;
		y += dy;
		return;
	}

	// Check collision between Simon and other objects
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
	//int tempState = state;	// Sử dụng biến tạm để không thay đổi trạng thái gốc của Simon
	
	int alpha = 255;
	float ratio = 0;

	
		
	
	

	animations[state]->Render(1, nx, x, y, alpha);
	animations[state]->SetFrame(animations[state]->GetCurrentFrame());
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






