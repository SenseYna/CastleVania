#pragma once

#include "GameObject.h"

class Whip : public GameObject
{
	vector<vector<float>> sparkCoord; // vector lưu toạ độ để render spark khi roi đánh trúng mục tiêu
	Animation * spark = Animations::GetInstance()->Get("spark_ani");
	int startTimeRenderSpark = 0;

public:
	Whip();

	// Update
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);

	// Render
	virtual void Render() {}
	void Render(int currentID = -1);
	void RenderSpark();

	// Set
	void SetWhipPosition(D3DXVECTOR3 simonPositon, bool isStand);

	// Get
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	// Function
	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);
	void PowerUp();
};
