#pragma once

#include "GameObject.h"

class Whip : public GameObject
{
	int targetTypeHit = -1; // Loại mục tiêu đánh trúng (dùng để kiểm tra máu của boss...)

	vector<vector<float>> sparkCoord; // vector lưu toạ độ để render spark khi roi đánh trúng mục tiêu
	Animation * spark = Animations::GetInstance()->Get("spark_ani");
	int startTimeRenderSpark = 0;

public:
	Whip();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMovement = false);
	virtual void Render() {}

	void Render(int currentID = -1);
	void RenderSpark();

	void SetWhipPosition(D3DXVECTOR3 simonPositon, bool isStand);
	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void PowerUp();

	
	int GetTargetTypeHit() { return targetTypeHit; }

	
	void SetTargetTypeHit(int x) { targetTypeHit = x; }
};


