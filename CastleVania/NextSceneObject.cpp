#include "NextSceneObject.h"
#include "Define.h"

void NextSceneObject::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = l + NEXTSCENEOBJECT_BBOX_WIDTH;
	b = t + NEXTSCENEOBJECT_BBOX_HEIGHT;
}

