﻿#include "WallPieces.h"

WallPiece::WallPiece()
{
	AddAnimation("wall_piece_ani");
	SetState(0);
}

void WallPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	GameObject::Update(dt);
	vy += WALLPIECES_GRAVITY * dt;

	x += dx;
	y += dy;
}

void WallPiece::Render()
{ 
	animations[state]->Render(nx, x, y);
}

WallPieces::WallPieces(float x, float y)
{
	WallPiece * piece;

	for (int i = 1; i <= 4; i++)
	{
		piece = new WallPiece();
		piece->SetPosition(x, y);

		float vx = (float)(-100 + rand() % 200) / 1000;
		float vy = (float)(-100 + rand() % 200) / 1000;
		piece->SetSpeed(vx, vy);

		pieces.push_back(piece);
	}
}

WallPieces::~WallPieces()
{
	
}

void WallPieces::Update(DWORD dt)
{
	for (auto piece : pieces)
		piece->Update(dt);
}

void WallPieces::Render()
{
	for (auto piece : pieces)
		piece->Render();
}
