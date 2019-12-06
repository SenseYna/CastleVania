#include "Bubbles.h"



Bubble::Bubble()
{
	AddAnimation("bubbles_ani");
	SetState(0);
}

void Bubble::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
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

	/*for (int i = 0; i < 3; i++)
	{
		bubble = new Bubble();
		bubble->SetPosition(x, y);

		float vx =  (float)(rand() % 100 * 2 ) / 1000;
		if (i == 2) vx = -vx;
		float vy =  - (float)(rand() % 100 * 2 ) / 1000;
		bubble->SetSpeed(vx, vy);

		bubbles.push_back(bubble);
	}*/
}

Bubbles::~Bubbles()
{
	for (UINT i = 0; i < 3; i++)
		delete(bubbles[i]);

	bubbles.clear();
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


