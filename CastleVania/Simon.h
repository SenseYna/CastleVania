#pragma once

#include <vector>
#include "GameObject.h"
#include "Timer.h"

using namespace std;

class Simon : public GameObject
{
	int currentWeapon; 
	int energy;
	int item;
	

	// auto-walk
	float autoWalkDistance = 0;		// Khoảng cách 

public:
	Simon();

	// Bool
	bool isWhip();
	bool isTouchGround = false;	
	bool isHitWeapons = false;
	int IDWeapon = 1;
	bool isGotChainItem = false;	// xác định xem có nhặt được Chain item hay không, dùng để update whip
	int isNextScene = -1;
	bool isAutoWalk = false;
	bool isCollisionHead = false;

	//Update
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	
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

	//Auto Walk
	void AutoWalk(float distance);
	void DoAutoWalk();

	//Render
	void Render();

};


