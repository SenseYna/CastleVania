#include "Bubbles.h"



Bubble::Bubble()
{
	AddAnimation("bubbles_ani");
	SetState(0);
}

void Bubble::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	GameObject::Update(dt);
	vy += BUBBLES_GRAVITY * dt;

	x += dx;
	y += dy;
}

void Bubble::Render()
{
	animations[state]->Render(nx, x, y);
}

Bubbles::Bubbles(float x, float y)
{
	// Khởi tạo 3 bubble

	Bubble * bubble;
	float vx, vy;
	bubble = new Bubble();
	bubble->SetPosition(x, y);

	vx = -0.03;
	vy = -0.12;
	bubble->SetSpeed(vx, vy);
	bubbles.push_back(bubble);

	bubble = new Bubble();
	bubble->SetPosition(x, y);
	vx = -0.01;
	vy = -0.18;
	bubble->SetSpeed(vx, vy);
	bubbles.push_back(bubble);

	bubble = new Bubble();
	bubble->SetPosition(x, y);
	vx = 0.06;
	vy = -0.1;
	bubble->SetSpeed(vx, vy);
	bubbles.push_back(bubble);

}

Bubbles::~Bubbles()
{

}

void Bubbles::Update(DWORD dt)
{
	for (auto bubble : bubbles)
	{
		bubble->Update(dt);
	}
}

void Bubbles::Render()
{
	for (auto bubble : bubbles)
	{
		bubble->Render();
	}
}


