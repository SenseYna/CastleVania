﻿#include "Scenes.h"


Scenes::Scenes(Game * game)
{
	this->game = game;

	// INIT
	simon = new Simon();
	whip = new Whip();

	for (int i = 1; i <= 3; i++)
	{
		weapons = new Weapons();
		weapons->SetEnable(false);
		weaponlist.push_back(weapons);
	}
}

Scenes::~Scenes()
{
}

void Scenes::Init()
{
	grid = new Grid(1536, 480); // 48 15
	LoadObjectsFromFile(FILEPATH_OBJECTS_SCENE_1);
	SetGameState();
}

void Scenes::LoadObjectsFromFile(LPCWSTR FilePath)
{
	fstream fs;
	fs.open(FilePath, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Scene %d load data failed: file path = %s\n", IDScene, FilePath);
		fs.close();
		return;
	}

	int ID_Obj;
	float pos_x, pos_y;
	int state;
	bool isEnable;
	int idItem;

	while (!fs.eof())
	{
		fs >> ID_Obj >> pos_x >> pos_y >> state >> isEnable >> idItem;

		switch (ID_Obj)
		{
			case GROUND:
			{
				Ground * ground = new Ground();
				ground->SetPosition(pos_x, pos_y);
				ground->SetState(state);
				ground->SetEnable(true);
				unit = new Unit(grid, ground, pos_x, pos_y);
				break;
			}
			case CANDLE:
			{
				Candle * candle = new Candle();
				candle->SetPosition(pos_x, pos_y);
				candle->SetState(state);
				candle->SetEnable(true);
				candle->SetIDItem(idItem);
				unit = new Unit(grid, candle, pos_x, pos_y);
				break;
			}
			default:
				break;
		}
	}

	fs.close();
}

void Scenes::GetObjectFromGrid()
{
	listUnits.clear();
	listObjects.clear();

	grid->Get(game->GetCameraPositon(), listUnits);

	//DebugOut(L"%d \n", listUnits.size());

	for (UINT i = 0; i < listUnits.size(); i++)
	{
		LPGAMEOBJECT obj = listUnits[i]->GetObj();
		listObjects.push_back(obj);

	}
}

void Scenes::Update(DWORD dt)
{
	// Lấy danh sách object từ grid 
	GetObjectFromGrid();

	// Drop item
	SetDropItems();

	Simon_Update(dt);
	Whip_Update(dt);

	for (int i = 0; i < 3; i++)
		Weapon_Update(dt, i);


	for (UINT i = 0; i < listObjects.size(); i++)
	{
		LPGAMEOBJECT object = listObjects[i];
		vector<LPGAMEOBJECT> coObjects;

		GetColliableObjects(object, coObjects);
		object->Update(dt, &coObjects);
	}

	// Xoá các object đi ra khỏi vùng viewport
	SetInactivationByPosition();

	// update camera
	UpdateCameraPosition();

	// update grid
	UpdateGrid();
}

void Scenes::Whip_Update(DWORD dt)
{	
	if (simon->isHitWeapons) 
		return;

	if (simon->isGotChainItem == true) // update trạng thái của whip
	{
		simon->isGotChainItem = false;
		whip->PowerUp();
	}

	// lấy vị trí và phương của simon cho whip
	float simon_x, simon_y;
	simon->GetPosition(simon_x, simon_y);
	bool isSimonStand = true;

	if (simon->GetState() == SIT || simon->GetState() == HIT_SIT)
		isSimonStand = false;

	whip->SetOrientation(simon->GetOrientation()); // Lấy hướng
	whip->SetWhipPosition(D3DXVECTOR3(simon_x, simon_y, 0), isSimonStand);

	// chỉ xét va chạm khi render tới sprite cuối cùng của simon (vung tay tới)
	if (simon->isWhip() && simon->animations[simon->GetState()]->IsRenderingLastFrame() == true)
	{
		vector<LPGAMEOBJECT> coObjects;

		GetColliableObjects(whip, coObjects);

		whip->Update(dt, &coObjects);
	}

}

void Scenes::Weapon_Update(DWORD dt, int index)
{
	if (weaponlist[index]->IsEnable() == false)
	{
		return;
	}

	vector<LPGAMEOBJECT> coObjects;
	GetColliableObjects(weaponlist[index], coObjects);

	weaponlist[index]->Update(dt, &coObjects);
}

void Scenes::Render()
{
	tilemaps->Get(SCENE_1)->Draw(game->GetCameraPositon());

	for (auto obj : listObjects)
	{
		obj->Render();
		//obj->RenderBoundingBox();
	}

	simon->Render();
	//simon->RenderBoundingBox();

	for (int i = 0; i < 3; i++)
	{
		weaponlist[i]->Render();
		//weaponlist[i]->RenderBoundingBox();
	}

	if (simon->isHitWeapons == false)
	{
		if (simon->isWhip() == true) {
			whip->Render(simon->animations[simon->GetState()]->GetCurrentFrame());
			//whip->RenderBoundingBox();
		}
		else
			whip->Render(-1);
			//whip->RenderBoundingBox();
	}
}

void Scenes::SetDropItems()
{
	for (UINT i = 0; i < listObjects.size(); i++)
	{
		LPGAMEOBJECT object = listObjects[i];
		int idItem = -1;
		float x, y;

		if (object->IsDroppedItem() == true)
			continue;

		if (dynamic_cast<Candle*>(object) && object->GetState() == CANDLE_DESTROYED)
		{
			idItem = object->idItem;
			object->GetPosition(x, y);
			object->SetIsDroppedItem(true);
		}
		
		if (idItem != -1)
		{
			// Tạo một item theo id
			auto item = new Items();
			item->SetEnable(true);
			item->SetPosition(x, y);
			item->SetState(idItem);

			listItems.push_back(item);
			unit = new Unit(grid, item, x, y);
		}
	}
}

bool Scenes::IsInViewport(LPGAMEOBJECT object)
{
		D3DXVECTOR2 camPosition = game->GetCameraPositon();

		float obj_x, obj_y;
		object->GetPosition(obj_x, obj_y);

		return obj_x >= camPosition.x && obj_x < camPosition.x + SCREEN_WIDTH
			&& obj_y >= camPosition.y && obj_y < camPosition.y + SCREEN_HEIGHT;
}

void Scenes::SetInactivationByPosition()   // Xoá các object đi ra khỏi vùng viewport
{
	for (int i = 0; i < 3; i++)
	{
		if (weaponlist[i]->IsEnable() == true)
		{
			if (IsInViewport(weaponlist[i]) == false)
				weaponlist[i]->SetEnable(false);
		}
	}
}

void Scenes::UpdateCameraPosition()
{

	if (simon->x + SIMON_BBOX_WIDTH > SCREEN_WIDTH / 2 &&
		simon->x + SIMON_BBOX_WIDTH  + SCREEN_WIDTH / 2 < tilemaps->Get(SCENE_1)->GetMapWidth())
	{
		{
			game->SetCameraPosition(simon->x + SIMON_BBOX_WIDTH - SCREEN_WIDTH / 2, 0);
		}
	}
}

void Scenes::UpdateGrid()
{
	//for (int i = 0; i < listUnits.size(); i++)
	//{
	//	LPGAMEOBJECT obj = listUnits[i]->GetObj();

	//	/*if (obj->IsEnable() == false)
	//		continue;

	//	float newPos_x, newPos_y;
	//	obj->GetPosition(newPos_x, newPos_y);
	//	listUnits[i]->Move(newPos_x, newPos_y);*/
	//}
}

void Scenes::SetGameState()
{
	simon->SetState(STAND);
	simon->SetPosition(0, 302);
	game->SetCameraPosition(0, 0);
}

void Scenes::Simon_Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	GetColliableObjects(simon, coObjects);

	simon->Update(dt, &coObjects);
	simon->CheckCollisionWithItem(&listItems);
}

void Scenes::GetColliableObjects(LPGAMEOBJECT Obj, vector<LPGAMEOBJECT>& coObjects)
{
	if (dynamic_cast<Items*>(Obj))
	{
		for (auto obj : listObjects)
		{
			if (dynamic_cast<Ground*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Simon*>(Obj))
	{
		for (LPGAMEOBJECT obj : listObjects)
		{
			if (dynamic_cast<Ground*>(obj))
				coObjects.push_back(obj);

		}
	}
	else if (dynamic_cast<Whip*>(Obj))
	{
		for (LPGAMEOBJECT obj : listObjects)
		{
			if (dynamic_cast<Candle*>(obj))
				coObjects.push_back(obj);

		}
	}
	else if (dynamic_cast<Weapons*>(Obj))
	{
		for (LPGAMEOBJECT obj : listObjects)
		{
			if (dynamic_cast<Candle*>(obj))
				coObjects.push_back(obj);

		}
	}

}


