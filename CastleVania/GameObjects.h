#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"


using namespace std;

class CGameObject
{
protected:
	float x;
	float y;

	float vx;
	float vy;

	int currentState;

	vector<LPANIMATION> animations;

public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetState(int state) { this->currentState = state; }
	void AddAnimation(int aniId);

	CGameObject();

	virtual void Update(DWORD dt);
	virtual void Render();
	~CGameObject();
};

typedef CGameObject * LPGAMEOBJECT;

class CGameObjects {
	static CGameObjects * __instance;
	unordered_map<int, LPGAMEOBJECT> objects;
	vector<int> ids;

public: 
	void add(int id, LPGAMEOBJECT obj);
	void render();
	void update(DWORD dt);
	LPGAMEOBJECT get(int id);
	static CGameObjects * GetInstance();
};

class Test : public CGameObject {
public: 
	Test() :CGameObject() {};
		void Update(DWORD dt);
		void Render();
};

class Mario : public CGameObject {
public: 
	Mario() :CGameObject() {};
		void Update(DWORD dt);
		void Render();
};