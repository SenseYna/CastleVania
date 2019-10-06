#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObjects.h"
#include "Sprites.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = 0.07f;
	vy = 0.07f;
}

void CGameObject::Update(DWORD dt)
{
	
}

void CGameObject::Render()
{
	
}

void CGameObject::AddAnimation(int aniId)
{
	LPANIMATION ani = CAnimations::GetInstance()->Get(aniId);
	animations.push_back(ani);
}



CGameObject::~CGameObject()
{
}

void Test::Update(DWORD dt)
{
	y += vy * dt;
	x += vx * dt;
	if ((vy > 0 && y > 210) || (y < 0 && vy < 0)) vy = -vy;
	if ((vx > 0 && x > 290) || (x < 0 && vx < 0)) vx = -vx;
}
void Test::Render()
{
	LPANIMATION ani;
	ani = animations[0];
	//ani = animations[0];
	ani->Render(x, y);
}
void Mario::Update(DWORD dt)
{
	x += vx * dt;
	if ((vx > 0 && x > 290) || (x < 0 && vx < 0)) vx = -vx;
}
void Mario::Render()
{
	LPANIMATION ani;
	if (vx > 0) ani = animations[0]; else ani = animations[1];
	//ani = animations[0];
	ani->Render(x, y);
}
CGameObjects * CGameObjects::__instance = NULL;

CGameObjects * CGameObjects::GetInstance()
{
	if (__instance == NULL) __instance = new CGameObjects();
	return __instance;
}

void CGameObjects::add(int id, LPGAMEOBJECT obj)
{
	objects[id] = obj;
	ids.push_back(id);
}

void CGameObjects::render()
{
	if (ids.size() == 0) printf("objects is empty");
	else 
		for (int i = 0; i < ids.size(); i++)
			objects[ids[i]]->Render();
}
void CGameObjects::update(DWORD dt)
{
	if (ids.size() == 0) printf("objects is empty");
	else
		for (int i = 0; i < ids.size(); i++)
		{
			int id = ids[i];
			objects[id]->Update(dt);
		}
}

LPGAMEOBJECT CGameObjects::get(int id)
{
	return objects[id];
}




