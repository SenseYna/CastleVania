#pragma once

#include "GameObject.h"

// Trigger object để nhận biết việc chuyển scene khi xét va chạm với Simon.
class NextSceneObject : public GameObject
{
	int IDNextScene;
	int IDGameState;

public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL) {}
	virtual void Render() {}

	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void SetIDNextScene(int x) { this->IDNextScene = x; }
	void SetIDGameState(int x) { this->IDGameState = x; }
	int GetIDNextScene() { return this->IDNextScene; }
	int GetIDGameState() { return this->IDGameState; }
};

typedef NextSceneObject * LPCHANGESCENEOBJ;

