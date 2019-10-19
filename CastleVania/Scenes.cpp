#include "Scenes.h"


Scenes::Scenes(Game * game)
{
	this->game = game;

	// INIT
	simon = new Simon();

	

	
}

Scenes::~Scenes()
{
}

//480
void Scenes::Init()
{
	grid = new Grid(1536, 480); 
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


	// update camera
	UpdateCameraPosition();

	// update grid
	UpdateGrid();
}

void Scenes::Render()
{
		tilemaps->Get(SCENE_1)->Draw(game->GetCameraPositon());

		simon->Render();
		//simon->RenderBoundingBox();
	
	
}

void Scenes::UpdateCameraPosition()
{

	if (simon->x > SCREEN_WIDTH / 2 &&
		simon->x + SCREEN_WIDTH / 2 < tilemaps->Get(SCENE_1)->GetMapWidth())
	{

		TileMap * map = tilemaps->Get(SCENE_1);
		int min_col = map->min_max_col_to_draw[map->index][0];
		int max_col = map->min_max_col_to_draw[map->index][1];

		if (simon->x >= min_col * 32 + (SCREEN_WIDTH / 2 - 16) &&
			simon->x <= max_col * 32 - (SCREEN_WIDTH / 2 - 16))
		{
			game->SetCameraPosition(simon->x - SCREEN_WIDTH / 2, 0);
		}
	}
}

void Scenes::UpdateGrid()
{
	for (int i = 0; i < listUnits.size(); i++)
	{
		LPGAMEOBJECT obj = listUnits[i]->GetObj();

		if (obj->IsEnable() == false)
			continue;

		float newPos_x, newPos_y;
		obj->GetPosition(newPos_x, newPos_y);
		listUnits[i]->Move(newPos_x, newPos_y);
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

void Scenes::GetColliableObjects(LPGAMEOBJECT curObj, vector<LPGAMEOBJECT>& coObjects)
{
	
		 if (dynamic_cast<Simon*>(curObj))
		{
			for (auto obj : listObjects)
			{
				if (dynamic_cast<Ground*>(obj))
					coObjects.push_back(obj);

			}
		}
}


