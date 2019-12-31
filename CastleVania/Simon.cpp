
#include "Simon.h"

#include "Ground.h"
#include "NextSceneObject.h"
#include "Door.h"
#include "Zombie.h"
#include <fstream>
#include <string>
#include "BlackLeopard.h"
#include "Bat.h"
#include "FishMan.h"
#include "FireBall.h"
#include "BreakWall.h"
#include "Water.h"
#include "Boss.h"

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
	AddAnimation("simon_stairup_ani");
	AddAnimation("simon_stairdown_ani");
	AddAnimation("simon_hitstairup_ani");
	AddAnimation("simon_hitstairdown_ani");
	AddAnimation("simon_deflect_ani");
}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMovement)
{
	GameObject::Update(dt);
	
	if (!isAutoWalk && !isStandOnStair) {

		//int temp_x = x;
		/*if (GetState() == JUMP && vy > 0.2 && vy< 0.3 && isTouchGround == false)
			SetState(STAND);*/
		//x = temp_x;

		if (vy > -SIMON_SPEED_Y_LOWER_ZONE && vy < SIMON_SPEED_Y_LOWER_ZONE && !isCollisionHead) // trọng lực khi nhảy
			vy += SIMON_GRAVITY_LOWER * dt;
		else
			vy += SIMON_GRAVITY * dt;
	}
	if (!isAutoWalk && !isStandOnStair && (state == STAND || state == WALK)) 
	{
		vy += SIMON_GRAVITY_FASTER * dt;
	} 

	// gia tốc rơi lớn
	if ((vy > 0.9 && !isDelayHightGravitySit && vy < 1) || state == DEFLECT)
		hightGravity = true;
	//DebugOut(L"[ERROR] vy %b \n", hightGravity);

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
				auto ground = dynamic_cast<Ground*>(e->obj);
				if (ground->state == INACTIVE) {
					x += dx;
					y += dy;
					continue;
				}
				if (ground->kind == ACTIVE) {
					continue;
				}
				if (e->ny != 0)				// Trên dưới đụng, nx là trái phải
				{
					if (e->ny == CDIR_BOTTOM) // -1 là đụng dưới
					{
						if (hightGravity && state != JUMP) {
							isDelayHightGravitySit = true;
							SetState(SIT);
							hightGravity = false;
						}
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
				//DebugOut(L"[ERROR] LoadSprites failed!: ID=%d\n", vx);
			}
			else if (dynamic_cast<BreakWall*>(e->obj))
			{
				auto ground = dynamic_cast<BreakWall*>(e->obj);
				if (e->ny != 0)				// Trên dưới đụng, nx là trái phải
				{
					if (e->ny == CDIR_BOTTOM) // -1 là đụng dưới
					{
						if (hightGravity && state != JUMP) {
							isDelayHightGravitySit = true;
							SetState(SIT);
							hightGravity = false;
						}
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
				//DebugOut(L"[ERROR] LoadSprites failed!: ID=%d\n", vx);
			}
			else if (dynamic_cast<Door*>(e->obj))
			{
				auto door = dynamic_cast<Door*>(e->obj);

				if (e->obj->GetState() == DOOR_1)	// đi qua cửa của scene 1
				{
					SetState(WALK);
					vx = SIMON_WALKING_SPEED_LOWER;
					vy = 0;
					AutoWalk(80, STAND, DIR_RIGHT);
				}
				else if (door->GetState() == DOOR_2_IDLE)
				{
					vx = 0;

					if (e->nx == CDIR_LEFT)	 // Simon đã đi qua cửa
						x += 1.0f;		 // +1 để không bị overlap
					else
					{
						door->SetState(DOOR_2_OPEN);
						door->animations[DOOR_2_OPEN]->SetAniStartTime(GetTickCount());
						autoWalkScene2_1Timer->Start();
						isWalkThroughDoor = true;
					}
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
				} 

				this->isNextScene = obj->GetIDNextScene();
				this->isGameState = obj->GetIDGameState();
			}
			else if (dynamic_cast<Zombie*>(e->obj) || dynamic_cast<BlackLeopard*>(e->obj) || dynamic_cast<FireBall*>(e->obj) || dynamic_cast<FishMan*>(e->obj))
			{
				if (e->obj->GetState() == DESTROYED)
					continue;
				if (state != POWER && untouchableTimer->IsTimeUp() == true)
				{
					untouchableTimer->Start();

					if (isStandOnStair == false)  // Simon đứng trên cầu thang sẽ không bị bật ngược lại
					{
						// đặt trạng thái deflect cho simon
						if (e->nx != 0)
						{
							if (e->nx == CDIR_LEFT && this->nx == 1) this->nx = DIR_LEFT;
							else if (e->nx == CDIR_RIGHT && this->nx == -1) this->nx = DIR_RIGHT;
						}
						SetState(DEFLECT);
					}
				}
				else
				{
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
				}
			}
			else if (dynamic_cast<Boss*>(e->obj))
			{
				Boss * boss = dynamic_cast<Boss*>(e->obj);
				
				if (state != POWER && untouchableTimer->IsTimeUp() == true)
				{
					untouchableTimer->Start();

					if (isStandOnStair == false)  // Simon đứng trên cầu thang sẽ không bị bật ngược lại
					{
						// đặt trạng thái deflect cho simon
						if (e->nx != 0)
						{
							if (e->nx == CDIR_LEFT && this->nx == 1) this->nx = DIR_LEFT;
							else if (e->nx == CDIR_RIGHT && this->nx == -1) this->nx = DIR_RIGHT;
						}

						SetState(DEFLECT);
					}
				}
				else
				{
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
				}
			}
			else if (dynamic_cast<Bat*>(e->obj))
			{
				Bat * bat = dynamic_cast<Bat*>(e->obj);
				
				if (bat->GetState() == BAT_INACTIVE) continue;
				bat->SetState(BAT_DESTROYED);

				if (state != POWER && untouchableTimer->IsTimeUp() == true)
				{
					untouchableTimer->Start();

					if (isStandOnStair == false)  // Simon đứng trên cầu thang sẽ không bị bật ngược lại
					{
						// đặt trạng thái deflect cho simon
						if (e->nx != 0)
						{
							if (e->nx == CDIR_LEFT && this->nx == 1) this->nx = DIR_LEFT;
							else if (e->nx == CDIR_RIGHT && this->nx == -1) this->nx = DIR_RIGHT;
						}

						SetState(DEFLECT);
					}
				}
				else
				{
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
				}
			}
			else if (dynamic_cast<Water*>(e->obj))
			{
				if (e->ny == CDIR_BOTTOM)
				{
					Water * water = dynamic_cast<Water*>(e->obj);
					water->AddBubbles(x, y + SIMON_BBOX_HEIGHT);

				}
				SetEnable(false);
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
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

	if (untouchableTimer->IsTimeUp() == false)
		alpha = rand() % 255;
	if (IsEnable()) {
		animations[tempState]->Render(nx, x, y, alpha);
		animations[state]->SetFrame(animations[tempState]->GetCurrentFrame());
	}
	
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
			animations[state]->Reset();
			animations[state]->SetAniStartTime(GetTickCount());
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

		case STAIR_UP:
			if (nx > 0) vx = SIMON_STAIR_SPEED_X;
			else vx = -SIMON_STAIR_SPEED_X;
			vy = -SIMON_STAIR_SPEED_Y;
			animations[state]->Reset();
			animations[state]->SetAniStartTime(GetTickCount());
			break;

		case STAIR_DOWN:
			if (nx > 0) vx = SIMON_STAIR_SPEED_X;
			else vx = -SIMON_STAIR_SPEED_X;
			vy = SIMON_STAIR_SPEED_Y;
			animations[state]->Reset();
			animations[state]->SetAniStartTime(GetTickCount());
			break;

		case HIT_STAIR_UP:
		case HIT_STAIR_DOWN:
			vx = 0;
			vy = 0;
			animations[state]->Reset();
			animations[state]->SetAniStartTime(GetTickCount());
			break;

		case DEFLECT:
			isTouchGround = false;
			vy = -SIMON_DEFLECT_SPEED_Y;
			if (nx > 0) vx = -SIMON_DEFLECT_SPEED_X;
			else vx = SIMON_DEFLECT_SPEED_X;
			animations[state]->Reset();
			animations[state]->SetAniStartTime(GetTickCount());
			break;
		default:
			break;
	}
}

void Simon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	// sprite có kích thước là 60x66, bbox là 30x62
	left = x + 15; //30,60
	top = y + 2;  //62,66
	right = left + SIMON_BBOX_WIDTH;
	bottom = top + SIMON_BBOX_HEIGHT;

}

bool Simon::isWhip()
{
	return state == HIT_SIT || state == HIT_STAND || state == HIT_STAIR_DOWN || state == HIT_STAIR_UP;
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
			case STOPWATCH:
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

bool Simon::CheckRightnessCollisionWithStair(vector<LPGAMEOBJECT>* listStair)
{
	float simon_l, simon_t, simon_r, simon_b;
	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	if (isStandOnStair) 
	{
		needStateMoveUpStair = true;
		needStateMoveDownStair = true;
	}

	for (UINT i = 0; i < listStair->size(); i++)
	{
		float stair_l, stair_t, stair_r, stair_b;	

		listStair->at(i)->GetBoundingBox(stair_l, stair_t, stair_r, stair_b);

		if ((simon_b > stair_b && listStair->at(i)->GetState() == 1) || (simon_b > stair_b && listStair->at(i)->GetState() == 2)) continue; // tương tự dưới
		 // đi xuống thì k xét va chạm stair top - lỗi vì xét va chạm out stair ở cạnh - return
		else if (GameObject::AABB(simon_l, simon_t, simon_r, simon_b+3, stair_l, stair_t, stair_r, stair_b) == true)
		{
			//if (simon_b > stair_b && listStair->at(i)->GetState() == -2) continue;

			if (abs(listStair->at(i)->GetState()) == 1) stairDirection = 1;  // Direction stair left
			else stairDirection = -1;
			if (listStair->at(i)->GetState() < 0 && stairDirection == -1) col_stair_bot = true; // sửa vị trí x cho khớp cầu thang khi đi xuống
			//else col_stair_top = true;

			stairCollided = listStair->at(i);

			// bậc thang ở dưới so với chân Simon->có thể di chuyển xuống.
			if (simon_b + 10 > stair_t) { needStateMoveUpStair = true; needStateMoveDownStair = true; }
			//if (col_stair_bot) needStateMoveDownStair = false;
			
			if (needStateMoveUpStair && simon_b <= stair_t + STAIR_BBOX_HEIGHT/ 2 + 3 && stairCollided->GetState() == 1) // xét đi lên hết cầu thang: đang lên và cao hơn object stair trên
				needStateMoveUpStair = false;
			if (needStateMoveUpStair && simon_b <= stair_t + STAIR_BBOX_HEIGHT / 2 + 10 && stairCollided->GetState() == 2) // xét đi lên hết cầu thang: đang lên và cao hơn object stair trên
				needStateMoveUpStair = false;
			if (needStateMoveDownStair && simon_b >= stair_b - STAIR_BBOX_HEIGHT / 2 -10 && stairCollided->GetState() == -1) // xét đi xuống hết cầu thang: đang xuống và thấp hơn object stair dưới
				needStateMoveDownStair = false;
			else if (needStateMoveDownStair && simon_b >= stair_t - STAIR_BBOX_HEIGHT / 2 - 3 && stairCollided->GetState() == -2) // xét đi xuống hết cầu thang: đang xuống và thấp hơn object stair dưới
				needStateMoveDownStair = false;
			return true; // collision between Simon and stairs
		}

	}

	return false;
}

void Simon::CheckCollisionWithEnemyActiveArea(vector<LPGAMEOBJECT>* listObjects)
{
	float simon_l, simon_t, simon_r, simon_b;

	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	for (UINT i = 0; i < listObjects->size(); i++)
	{
		Enemy * enemy = dynamic_cast<Enemy*>(listObjects->at(i));

		if (enemy == NULL)
			continue;

		// Không cần xét vùng active nữa khi nó đang active / destroyed
		if (enemy->GetState() == ACTIVE || enemy->GetState() == DESTROYED)
			continue;

		// chỉ cần nằm trong grid là enable => sinh zombie
		if (dynamic_cast<Zombie*>(enemy))
		{
			Zombie * zombie = dynamic_cast<Zombie*>(enemy);

			if (zombie->GetState() == ZOMBIE_INACTIVE && zombie->IsAbleToActivate() == true)
				zombie->SetState(ZOMBIE_ACTIVE);
			int x_zombie;

			x_zombie = simon_l + SCREEN_WIDTH;

			zombie->SetPosition(x_zombie, zombie->y); // vị trí chuẩn bị spawn tạm thời
			continue;
		}

		float enemy_l, enemy_t, enemy_r, enemy_b;
		enemy->GetActiveBoundingBox(enemy_l, enemy_t, enemy_r, enemy_b);

		if (GameObject::AABB(simon_l, simon_t, simon_r, simon_b, enemy_l, enemy_t, enemy_r, enemy_b) == true)
		{
			if (dynamic_cast<BlackLeopard*>(enemy))
			{
				BlackLeopard * leopard = dynamic_cast<BlackLeopard*>(enemy);
				if (leopard->GetState() == BLACK_LEOPARD_IDLE)
					leopard->SetState(BLACK_LEOPARD_ACTIVE);
			}
			else if (dynamic_cast<Bat*>(enemy))
			{
				Bat * bat = dynamic_cast<Bat*>(enemy);

				if (bat->GetState() == BAT_INACTIVE && bat->IsAbleToActivate() == true)
					bat->SetState(BAT_ACTIVE);
				int x_bat;

				x_bat = simon_l + SCREEN_WIDTH;

				bat->SetPosition(x_bat, bat->y); // vị trí chuẩn bị spawn tạm thời
			}
			else if (dynamic_cast<FishMan*>(enemy))
			{
				FishMan * fishman = dynamic_cast<FishMan*>(enemy);

				if (fishman->GetState() == FISHMAN_INACTIVE && fishman->IsAbleToActivate() == true)
					fishman->SetState(FISHMAN_ACTIVE);
			}
			else if (dynamic_cast<Boss*>(enemy))
			{
				Boss * boss = dynamic_cast<Boss*>(enemy);
				if (boss->GetState() == BOSS_IDLE)
					boss->SetState(BOSS_ACTIVE);
			}
		}
	}
}

void Simon::AutoWalk(float distance, int new_state, int new_nx)
{
	isAutoWalk = true;
	autoWalkDistance = distance;
	stateAfterAutoWalk = new_state;
	nxAfterAutoWalk = new_nx;
}

void Simon::DoAutoWalk()
{
	if (abs(dx) <= abs(autoWalkDistance))
	{
		x += dx;
		y += dy;
		autoWalkDistance -= abs(dx);
	}
	else 
	{ 
		x += autoWalkDistance;
		state = stateAfterAutoWalk;
		nx = nxAfterAutoWalk;

		SetState(state);
		if (state == STAIR_DOWN && stairDirection == 1) {
			x += 20.0f; y += 1.0f;
		}
		else if (state == STAIR_DOWN && stairDirection == -1) {
			x -= 8.0f; y += 0.5f;
		}

		isAutoWalk = false;
		autoWalkDistance = 0;
		stateAfterAutoWalk = -1;
		nxAfterAutoWalk = 0;
	}

}

void Simon::CheckCollisionWithBoss(LPGAMEOBJECT boss)
{
	float simon_l, simon_t, simon_r, simon_b;
	float boss_l, boss_t, boss_r, boss_b;

	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	boss->GetBoundingBox(boss_l, boss_t, boss_r, boss_b);

	if (GameObject::AABB(simon_l, simon_t, simon_r, simon_b, boss_l, boss_t, boss_r, boss_b))
	{
		if (state != POWER && untouchableTimer->IsTimeUp() == true)
		{
			untouchableTimer->Start();

			if (isStandOnStair == false)  // Simon đứng trên cầu thang sẽ không bị bật ngược lại
			{
				// đặt trạng thái deflect cho simon
				if (boss->nx != 0)
				{
					if (boss->nx == CDIR_LEFT && this->nx == 1) this->nx = DIR_RIGHT;
					else if (boss->nx == CDIR_RIGHT && this->nx == -1) this->nx = DIR_LEFT;
				}

				SetState(DEFLECT);
			}
		}
		else
		{
			if (boss->nx != 0) x += dx;
			//if (boss->ny != 0) y += dy;
		}
	
	}
}





