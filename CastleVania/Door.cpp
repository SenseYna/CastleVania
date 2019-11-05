#include "Door.h"



Door::Door() : GameObject()
{
	AddAnimation("door1_ani");
}

void Door::Render()
{
	if (state == DOOR_1)
		animations[state]->Render(-1, x + 28, y-220);
}

void Door::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	switch (state)
	{
	case DOOR_1:
		l = x;
		t = y;
		r = l + DOOR_BBOX_WIDTH;
		b = t + DOOR_BBOX_HEIGHT;
		break;
	default:
		break;
	}
}