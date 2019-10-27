#pragma once

#include <vector>
#include "GameObject.h"

using namespace std;

class Simon : public GameObject
{
	int currentWeapon; 
	int energy;
public:
	bool isTouchGround = false;	
	bool isHitWeapons = false;

	Simon();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL, bool stopMovement = false);
	void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	bool isWhip();
	int GetCurrentWeapons() { return currentWeapon; }
	void SetCurrentWeapons(int CurrentWeapons) { this->currentWeapon = CurrentWeapons;}
	int GetEnergy() { return energy; }
	
};


