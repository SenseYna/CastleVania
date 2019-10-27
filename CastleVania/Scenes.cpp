#include "Scenes.h"


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

//480
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
			unit = new Unit(grid, candle, pos_x, pos_y);
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

	// update camera
	UpdateCameraPosition();

	// update grid
	UpdateGrid();
}

void Scenes::Whip_Update(DWORD dt)
{
	if (simon->isHitWeapons) 
		return;

	// lấy vị trí và phương của simon cho whip
	float simon_x, simon_y;
	simon->GetPosition(simon_x, simon_y);
	bool isSimonStand = true;

	if (simon->GetState() == SIT || simon->GetState() == HIT_SIT)
		isSimonStand = false;

	whip->SetOrientation(simon->GetOrientation());
	whip->SetWhipPosition(D3DXVECTOR3(simon_x, simon_y, 0), isSimonStand);

	// chỉ xét va chạm khi render tới sprite cuối cùng của simon (vung tay tới)
	if (simon->isWhip() &&
		simon->animations[simon->GetState()]->IsRenderingLastFrame() == true)
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
		weaponlist[index]->SetTargetTypeHit(-1);
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

	for (int i = 0; i < 3; i++)
	{
		weaponlist[i]->Render();
		//weaponlist[i]->RenderBoundingBox();
	}

	if (simon->isHitWeapons == false)
	{
		if (simon->isWhip() == true)
			whip->Render(simon->animations[simon->GetState()]->GetCurrentFrame());
		else
			whip->Render(-1);

		//whip->RenderBoundingBox();
	}

	simon->RenderBoundingBox();

	if (simon->isWhip() == true)
		whip->Render(simon->animations[simon->GetState()]->GetCurrentFrame());

}

void Scenes::UpdateCameraPosition()
{

	if (simon->x > SCREEN_WIDTH / 2 &&
		simon->x + SCREEN_WIDTH / 2 < tilemaps->Get(SCENE_1)->GetMapWidth())
	{

		/*TileMap * map = tilemaps->Get(SCENE_1);
		int min_col = map->min_max_col_to_draw[map->index][0];
		int max_col = map->min_max_col_to_draw[map->index][1];*/

		/*if (simon->x >= min_col * 32 + (SCREEN_WIDTH / 2 - 16) &&
			simon->x <= max_col * 32 - (SCREEN_WIDTH / 2 - 16))*/
		{
			game->SetCameraPosition(simon->x - SCREEN_WIDTH / 2, 0);
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

//bool Scenes::IsInViewport(LPGAMEOBJECT object)
//{
//	/*D3DXVECTOR2 camPosition = game->GetCameraPositon();
//
//	float obj_x, obj_y;
//	object->GetPosition(obj_x, obj_y);
//
//	return obj_x >= camPosition.x && obj_x < camPosition.x + SCREEN_WIDTH
//		&& obj_y >= camPosition.y && obj_y < camPosition.y + SCREEN_HEIGHT; */
//	return 1;
//}

void Scenes::SetGameState()
{
	simon->SetState(STAND);
	simon->SetPosition(0, 302);
	game->SetCameraPosition(0, 0);
	tilemaps->Get(SCENE_1)->index = 0;
}

void Scenes::Simon_Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	GetColliableObjects(simon, coObjects);

	simon->Update(dt, &coObjects);
}

void Scenes::GetColliableObjects(LPGAMEOBJECT Obj, vector<LPGAMEOBJECT>& coObjects)
{

	if (dynamic_cast<Simon*>(Obj))
	{
		for (LPGAMEOBJECT obj : listObjects)
		{
			if (dynamic_cast<Ground*>(obj))
				coObjects.push_back(obj);

		}
	}
	if (dynamic_cast<Whip*>(Obj))
	{
		for (LPGAMEOBJECT obj : listObjects)
		{
			if (dynamic_cast<Candle*>(obj))
				coObjects.push_back(obj);

		}
	}

}


