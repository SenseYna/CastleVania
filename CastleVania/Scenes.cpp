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

void Scenes::Init(int idScene)
{
	
	IDScene = idScene;

	switch (idScene)
	{
	case SCENE_1:
		grid = new Grid(1536, 480); // 48 15
		LoadObjectsFromFile(FILEPATH_OBJECTS_SCENE_1);
		SetGameState(GAMESTATE_1);
		break;
	case SCENE_2:
		grid = new Grid(5632, 480); //5632
		LoadObjectsFromFile(FILEPATH_OBJECTS_SCENE_2);
		SetGameState(GAMESTATE_2);
		break;
	default:
		break;
	}
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
				candle->SetEnable(isEnable);
				candle->SetIDItem(idItem);
				unit = new Unit(grid, candle, pos_x, pos_y);
				break;
			}
			case NEXT_SCENE_OBJECT:
			{
				NextSceneObject * nextScene = new NextSceneObject();
				nextScene->SetPosition(pos_x, pos_y);
				nextScene->SetIDNextScene(state);
				nextScene->SetEnable(true);
				unit = new Unit(grid, nextScene, pos_x, pos_y);
				break;
			}
			case DOOR:
			{
				Door * door = new Door();
				door->SetPosition(pos_x, pos_y);
				door->SetState(state);
				door->SetEnable(true);
				unit = new Unit(grid, door, pos_x, pos_y);
				break;
			}
			case STAIR: 
			{
				Stair * stair = new Stair();
				stair->SetPosition(pos_x, pos_y);
				stair->SetState(state);  // -1: stair left bottom , -2: stair left top, 1: stair right bottom, 2 stair right top
				stair->SetEnable(true);
				unit = new Unit(grid, stair, pos_x, pos_y);
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
	listDoors.clear();
	listStairs.clear();

	grid->Get(game->GetCameraPositon(), listUnits);

	for (UINT i = 0; i < listUnits.size(); i++)
	{

		LPGAMEOBJECT obj = listUnits[i]->GetObj();
		listObjects.push_back(obj);

		if (dynamic_cast<Door*>(obj))
			listDoors.push_back(obj);
		if (dynamic_cast<Stair*>(obj))
			listStairs.push_back(obj);
	}
}

void Scenes::ChangeScene()
{

	if (IDScene == SCENE_1 && simon->isNextScene == SCENE_2)
		Init(SCENE_2);
}

void Scenes::Update(DWORD dt)
{
	// Khi Simon va chạm với ChangScene objects, tiến hành thay đổi, cập nhật trạng thái
	if (simon->isNextScene)
	{
		ChangeScene();
		simon->isNextScene = false;
	}

	// Lấy danh sách object từ grid 
	GetObjectFromGrid();

	// Drop item
	SetDropItems();

	Simon_Update(dt);
	Whip_Update(dt);

	Weapon_Update(dt, 0);

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
	tilemaps->Get(IDScene)->Draw(game->GetCameraPositon());

	for (auto obj : listObjects)
	{
		obj->Render();
		//obj->RenderBoundingBox();
	}

	simon->Render();
	//simon->RenderBoundingBox();

	weaponlist[0]->Render();

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

	for (auto obj : listDoors)
	{
		obj->Render();
		//obj->RenderBoundingBox();
	}
	for (auto obj : listStairs)
	{
		//obj->Render();
		obj->RenderBoundingBox();
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
	/*for (int i = 0; i < 3; i++)
	{*/
		if (weaponlist[0]->IsEnable() == true)
		{
			if (IsInViewport(weaponlist[0]) == false)
				weaponlist[0]->SetEnable(false);
		}
	/*}*/
}

void Scenes::UpdateCameraPosition()
{

	if (simon->x + SIMON_BBOX_WIDTH > SCREEN_WIDTH / 2 &&
		simon->x + SIMON_BBOX_WIDTH  + SCREEN_WIDTH / 2 < tilemaps->Get(IDScene)->GetMapWidth())
	{
		{
			game->SetCameraPosition(simon->x + SIMON_BBOX_WIDTH - SCREEN_WIDTH / 2, 0);
		}
	}
}

void Scenes::SetGameState(int state)
{
	switch (state)
	{
	case GAMESTATE_1:
		simon->SetState(STAND);
		simon->SetPosition(0, 302);
		game->SetCameraPosition(0, 0);
		break;
	case GAMESTATE_2:
		simon->SetState(STAND);
		simon->SetPosition(0, 200); 
		game->SetCameraPosition(0, 0);
		break;
	default:
		break;
	}
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
			if (dynamic_cast<NextSceneObject*>(obj) || dynamic_cast<Ground*>(obj) || dynamic_cast<Door*>(obj))
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
			if (dynamic_cast<Candle*>(obj) || dynamic_cast<Ground*>(obj))
				coObjects.push_back(obj);
		}
	}

}


