#pragma once

#include "GameObject.h"
#include "Simon.h"

class Weapons : public GameObject
{
	vector<vector<float>> sparkCoord; // vector lưu toạ độ để render spark khi Weapons trúng mục tiêu
	Animation * spark = Animations::GetInstance()->Get("spark_ani");
	int startTimeRenderSpark = 0;

public:
	Weapons();
	~Weapons();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);

	void SetState(int state);
	void UpdateCollisionState();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void GetCoordinateObject(LPGAMEOBJECT obj);			// Lấy toạ độ của object để lưu vào sparkCoord

	virtual void Render();
	void RenderSpark();
};

