#pragma once

#include "GameObject.h"

class Enemy : public GameObject
{
protected:
	DWORD respawnTime_Start; 
	bool isRespawnWaiting; // đang đợi hồi sinh
	int respawnWaitingTime; // Khoảng thời gian đợi hồi sinh

	D3DXVECTOR2 entryPosition; // đưa enemy về vị trí entry sau thời gian respawn

public:
	bool isSettedPosition;

	Enemy();
	~Enemy();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void GetActiveBoundingBox(float &left, float &top, float &right, float &bottom) = 0;

	void RenderActiveBoundingBox();

	void StartRespawnTimeCounter();
	bool IsAbleToActivate();

	void SetEntryPosition(float x, float y) { entryPosition.x = x; entryPosition.y = y; }
	D3DXVECTOR2 GetEntryPosition() { return entryPosition; }
};

#pragma once
