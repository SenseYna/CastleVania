#include "Stair.h"



Stair::Stair()
{
}

void Stair::Render()
{
}

void Stair::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (this->GetState() == -1 || this->GetState() == 2) l = x - STAIR_BBOX_WIDTH_BONUS;
	else l = x;
	t = y;
	r = l + STAIR_BBOX_WIDTH + STAIR_BBOX_WIDTH_BONUS;
	b = t + STAIR_BBOX_HEIGHT;
}
