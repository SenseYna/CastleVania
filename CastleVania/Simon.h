#pragma once

#include <vector>
#include "GameObject.h"
#include "Timer.h"

using namespace std;

class Simon : public GameObject
{
public:
	Timer * untouchableTimer = new Timer(SIMON_UNTOUCHABLE_TIME);
	Timer * invisibilityTimer = new Timer(SIMON_INVISIBILITY_TIME);

	bool isTouchGround = false;	

	Simon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL, bool stopMovement = false);
	void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

};


