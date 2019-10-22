#pragma once

#include <vector>
#include "GameObject.h"

using namespace std;

class Simon : public GameObject
{
public:
	bool isTouchGround = false;	
	Simon();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL, bool stopMovement = false);
	void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	bool isWhip();
	
};


