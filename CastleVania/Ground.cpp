#include "Ground.h"

Ground::Ground() : GameObject()
{
	AddAnimation("ground1_ani");
	AddAnimation("ground2_ani");
}

void Ground::Render()
{
	if (kind != GROUND_HIDE && state != INACTIVE && kind != GROUND_HIDE_STATE && kind != GROUND_WALL)    // trick không vẽ ground chỗ cầu thang xuống hầm
		animations[state]->Render(-1, x, y);
}

void Ground::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	if (l == GROUND_BBOX_HALF_X && t == GROUND_BBOX_HALF_Y)
	{
		r = l + GROUND_BBOX_WIDTH;
		b = t + GROUND_BBOX_HEIGHT_HALF_TOP; // cục gạch 3904 176 nó đụng đầu simon nên thu nhỏ lại
	}
	else if (kind == GROUND_HIDE) {
		r = l + GROUND_BBOX_WIDTH_HALF_TOP;
		b = t + GROUND_BBOX_HEIGHT; // dãy gạch đứng 4096 176 chỉ có bbox là 1 nửa trái
	}
	else 
	{
		r = l + GROUND_BBOX_WIDTH;
		b = t + GROUND_BBOX_HEIGHT;
	}
	
} 