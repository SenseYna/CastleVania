﻿#pragma once

#include <vector>
#include "GameObject.h"
#include "Timer.h"

using namespace std;

class Simon : public GameObject
{
	int currentWeapon; 
	int energy;
	int item;

	//auto-walk
	float autoWalkDistance = 0;		// Khoảng cách 
	int stateAfterAutoWalk = -1;	// Trạng thái sau khi auto-walk
	int nxAfterAutoWalk = 0;		// Hướng Simon sau khi auto-walk

public:
	Simon();
	Timer * untouchableTimer = new Timer(SIMON_UNTOUCHABLE_TIME);
	Timer * invisibilityTimer = new Timer(SIMON_INVISIBILITY_TIME);
	Timer * hightGravitySitTimer = new Timer(SIMON_INVISIBILITY_TIME);
	Timer * autoWalkScene2_1Timer = new Timer(GOTO_SCENE_2_1_TIME_DELAY);

	// Bool
	bool isWhip();
	bool isTouchGround = false;	
	bool hadSecrect = false;
	bool isHitWeapons = false;
	int IDWeapon = 1;
	bool isGotChainItem = false;	// xác định xem có nhặt được Chain item hay không, dùng để update whip
	bool isGotCrossItem = false;
	int isNextScene = -1;
	int isGameState= -1;
	bool isAutoWalk = false;
	bool isCollisionHead = false;
	bool col_stair_top = false; // va chạm bbox stair trên
	bool col_stair_bot = false; // va chạm bbox stair dưới
	bool isWalkThroughDoor = false;	// đi qua cửa
	bool hightGravity = false; // trọng lực lớn 
	bool isDelayHightGravitySit = false; // delay input

	//Stair
	bool isStandOnStair = false;	// trạng thái đang đứng trên cầu thang 
	bool needStateMoveUpStair = false;	// có thể di chuyển lên cầu thang
	bool needStateMoveDownStair = false;	// có thể di chuyển xuống cầu thang
	int stairDirection; 
	LPGAMEOBJECT stairCollided = nullptr; // lưu bậc thang va chạm với simon -> để xét vị trí đi tới cầu thang để lên - xuống

	//Update
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL, bool stopMovement = false);
	
	// Get
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	int GetCurrentWeapons() { return currentWeapon; }
	int GetEnergy() { return energy; }
	
	// Set
	void SetState(int state);
	void LoseEnergy(int x) { energy -= x; }
	void SetCurrentWeapons(int CurrentWeapons) { this->currentWeapon = CurrentWeapons; }
	
	// Kiểm tra va chạm với danh sách item
	bool CheckCollisionWithItem(vector<LPGAMEOBJECT> * listItem);

	void CheckCollisionWithBoss(LPGAMEOBJECT boss);

	// Kiểm tra tính đúng đắn va chạm với danh sách stair
	bool CheckRightnessCollisionWithStair(vector<LPGAMEOBJECT> * listStair);

	// Kiểm tra va chạm với vùng hoạt động của enemy
	void CheckCollisionWithEnemyActiveArea(vector<LPGAMEOBJECT> * listObjects);

	// Giữ cho Simon đứng yên trên bậc thang
	void StandOnStair() { vx = vy = 0; }

	//Auto Walk
	void AutoWalk(float distance, int new_state, int new_nx);
	void DoAutoWalk();

	//Render
	void Render();

};


