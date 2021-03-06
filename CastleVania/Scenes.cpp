﻿#include "Scenes.h"

// Read data of sprites and animations from file.
void Scenes::LoadSprites(int id, LPCWSTR tex, LPCWSTR sprite_data, LPCWSTR animation_data)
{
	textures->Add(id, tex);
	LPDIRECT3DTEXTURE9 texture = textures->Get(id);

	ifstream spriteReader, animationReader;

	spriteReader.open(sprite_data);
	animationReader.open(animation_data);

	if (spriteReader.fail())
	{
		DebugOut(L"[ERROR] LoadSprites failed!: ID=%d", id);
		spriteReader.close();
		return;
	}

	if (animationReader.fail())
	{
		DebugOut(L"[ERROR] LoadAnimation failed!: ID=%d", id);
		animationReader.close();
		return;
	}

	// Load sprite data

	string spriteid;
	int left, top, right, bottom;

	while (spriteReader >> spriteid >> left >> top >> right >> bottom)
		sprites->Add(spriteid, left, top, right, bottom, texture);

	spriteReader.close();

	// Load animation data

	string animationId;
	string line;
	string spriteId;
	int frameTime;

	while (getline(animationReader, line))
	{
		LPANIMATION ani = new Animation();

		istringstream iss(line, istringstream::in);
		iss >> animationId;

		while (iss >> spriteId >> frameTime)
			ani->Add(spriteId, frameTime);

		animations->Add(animationId, ani);
	}

	animationReader.close();
}

wchar_t* Scenes::ConvertToWideChar(char* p) // Covert string sang wchar_t*
{
	wchar_t *r;
	r = new wchar_t[strlen(p) + 1];

	char *tempsour = p;
	wchar_t *tempdest = r;
	while (*tempdest++ = *tempsour++);

	return r;
}

void Scenes::LoadSpritesFromFile(LPCWSTR FilePath)
{
	fstream fss;
	fss.open(FilePath, ios::in);
	if (fss.fail())
	{
		DebugOut(L"[ERROR] Scene %d load resource failed: file path = %s\n", FilePath);
		fss.close();
		return;
	}

	int ID_Tex;
	string path_texture;
	string path_sprites;
	string path_animations;

	while (!fss.eof())
	{
		fss >> ID_Tex >> path_texture >> path_sprites >> path_animations;
		LoadSprites(ID_Tex, ConvertToWideChar((char*)path_texture.c_str()), ConvertToWideChar((char*)path_sprites.c_str()), ConvertToWideChar((char*)path_animations.c_str()));
	}

	fss.close();
}

void Scenes::LoadTileMapFromFile(LPCWSTR FilePath)
{
	fstream fss;
	fss.open(FilePath, ios::in);
	if (fss.fail())
	{
		DebugOut(L"[ERROR] Scene %d load resource failed: file path = %s\n", FilePath);
		fss.close();
		return;
	}

	int ID_Tex;
	string path_texture;
	string path_map;
	int map_width;
	int map_height;

	while (!fss.eof())
	{
		fss >> ID_Tex >> path_texture >> path_map >> map_width >> map_height;
		tilemaps->Add(ID_Tex, ConvertToWideChar((char*)path_texture.c_str()), ConvertToWideChar((char*)path_map.c_str()), map_width, map_height);
	}

	fss.close();
}

// Load all sprite, animations, texture, tilemap data from file
void Scenes::LoadResources()
{
	// for render bounding box
	textures->Add(ID_TEX_BBOX, FILEPATH_BBOX_RESOURCE);

	//id tex, texture, sprite, animation
	LoadSpritesFromFile(FILEPATH_TEXTURE_RESOURCE);

	//tile map
	LoadTileMapFromFile(FILEPATH_TILE_MAP_RESOURCE);
}

Scenes::Scenes(Game * game)
{
	this->game = game;

	LoadResources();

	// INIT
	simon = new Simon();
	whip = new Whip();
	boss = new Boss();

	weapons = new Weapons();
	weapons->SetEnable(false);
	weaponlist.push_back(weapons);
}

Scenes::~Scenes()
{
}

void Scenes::Init(int idScene, int idGameState)
{
	
	IDScene = idScene;
	IDGameState = idGameState;
	
	switch (idScene)
	{
	case SCENE_1:
		LoadObjectsFromFileToGrid(FILEPATH_OBJECTS_SCENE_1);
		switch (idGameState)
		{
		case GAMESTATE_1:
			simon->SetState(STAND);
			simon->SetPosition(0, 302);
			game->SetCameraPosition(0, 0);
			isBossFighting = false;
			break;
		default:
			break;
		}
		break;
	case SCENE_2:
		LoadObjectsFromFileToGrid(FILEPATH_OBJECTS_SCENE_2);
		switch (idGameState)
		{
		case GAMESTATE_1:
			simon->SetState(STAND);
			simon->SetPosition(0, 272);
			game->SetCameraPosition(0, 0);
			isBossFighting = false;
			break;

		case GAMESTATE_2:
			simon->SetState(STAIR_UP);
			simon->SetPosition(3140, 338);
			simon->SetOrientation(-1);
			simon->isStandOnStair = true;
			game->SetCameraPosition(3056, 0);
			isBossFighting = false;
			break;

		case GAMESTATE_3:
			simon->SetState(STAIR_UP);
			simon->SetPosition(3780, 336); //-2 y
			simon->SetOrientation(-1);
			simon->isStandOnStair = true;
			game->SetCameraPosition(3056, 0);
			isBossFighting = false;
			break;

		case GAMESTATE_4:
			simon->SetState(STAND);
			simon->isStandOnStair = false;
			simon->SetPosition(5100, 48); //-2 y
			simon->SetOrientation(1);
			game->SetCameraPosition(4684, 0);
			isBossFighting = false;
			break;

		default:
			break;
		}
		break;

	case SCENE_3:
		LoadObjectsFromFileToGrid(FILEPATH_OBJECTS_SCENE_3);
		switch (idGameState)
		{
		case GAMESTATE_1:
			simon->SetState(STAIR_DOWN);
			simon->SetPosition(100, 16);
			simon->needStateMoveDownStair = true;
			simon->SetOrientation(1);
			simon->isStandOnStair = true;
			game->SetCameraPosition(0, 0);
			isBossFighting = false;
			break;

		case GAMESTATE_2:
			simon->SetState(STAIR_DOWN);
			simon->SetPosition(732, 16);
			simon->SetOrientation(1);
			game->SetCameraPosition(0, 0);
			isBossFighting = false;
			break;

		default:
			break;
		}
		break;
	default:
		break;
	}
}

void Scenes::LoadObjectsFromFileToGrid(LPCWSTR FilePath)
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
	int grid_width;
	int grid_height;
	int numberOfColumns;
	int numberOfRows;
	int cell_x;
	int cell_y;

	//256 // 240
	//grid = new Grid(grid_width, grid_height, numberOfRows, numberOfColumns);

	if (!fs.eof()) fs >> grid_width >> grid_height >> numberOfRows >> numberOfColumns;
	else {
		DebugOut(L"[ERROR] File is empty: file path = %s\n", FilePath);
		fs.close();
		return;
	}

	grid = new Grid(grid_width, grid_height, numberOfRows, numberOfColumns);

	while (!fs.eof())
	{
		fs >> ID_Obj >> pos_x >> pos_y >> state >> isEnable >> idItem >> cell_x >> cell_y;

		switch (ID_Obj)
		{
			case GROUND:
			{
				Ground * ground = new Ground();
				ground->SetPosition(pos_x, pos_y);
				ground->SetState(state);
				ground->kind = idItem;
				ground->SetEnable(true);
				ground->SetIDItem(idItem);
				unit = new Unit(grid, ground, pos_x, pos_y, cell_x, cell_y);
				break;
			}
			case CANDLE:
			{
				Candle * candle = new Candle();
				candle->SetPosition(pos_x, pos_y);
				candle->SetState(state);
				candle->SetEnable(isEnable);
				candle->SetIDItem(idItem);
				if (idItem == 5)
					idItem = 5;
				unit = new Unit(grid, candle, pos_x, pos_y, cell_x, cell_y);
				break;
			}
			case NEXT_SCENE_OBJECT:
			{
				NextSceneObject * nextScene = new NextSceneObject();
				nextScene->SetPosition(pos_x, pos_y);
				nextScene->SetIDNextScene(state);
				nextScene->SetIDGameState(idItem);
				nextScene->SetEnable(true);
				unit = new Unit(grid, nextScene, pos_x, pos_y, cell_x, cell_y);
				break;
			}
			case DOOR:
			{
				Door * door = new Door();
				door->SetPosition(pos_x, pos_y);
				door->SetState(state);
				door->SetEnable(true);
				unit = new Unit(grid, door, pos_x, pos_y, cell_x, cell_y);
				break;
			}
			case STAIR: 
			{
				Stair * stair = new Stair();
				stair->SetPosition(pos_x, pos_y);
				stair->SetState(state);  // -1: stair left bottom , -2: stair left top, 1: stair right bottom, 2 stair right top
				stair->SetEnable(true);
				unit = new Unit(grid, stair, pos_x, pos_y, cell_x, cell_y);
				break;
			}
			case ZOMBIE:
			{
				Zombie * zombie = new Zombie();
				zombie->SetEntryPosition(pos_x, pos_y);
				zombie->SetState(INACTIVE);
				zombie->SetEnable(true);
				zombie->SetIDItem(idItem);
				unit = new Unit(grid, zombie, pos_x, pos_y, cell_x, cell_y);
				break;
			}
			case BLACK_LEOPARD:
			{
				BlackLeopard * leopard = new BlackLeopard();
				leopard->SetEntryPosition(pos_x, pos_y);
				leopard->SetState(INACTIVE);
				leopard->SetEnable(true);
				leopard->SetIDItem(idItem);
				unit = new Unit(grid, leopard, pos_x, pos_y, cell_x, cell_y);
				break;
			}
			case BAT:
			{
				Bat * bat = new Bat();
				bat->SetEntryPosition(pos_x, pos_y);
				bat->SetState(INACTIVE);
				bat->SetEnable(true);
				unit = new Unit(grid, bat, pos_x, pos_y, cell_x, cell_y);
				break;
			}
			case FISHMAN:
			{
				FishMan * fishman = new FishMan();
				fishman->SetEntryPosition(pos_x, pos_y);
				fishman->SetState(INACTIVE);
				fishman->SetEnable(true);
				unit = new Unit(grid, fishman, pos_x, pos_y, cell_x, cell_y);
				break;
			}
			case WATER:
			{
				water = new Water();
				water->SetPosition(pos_x, pos_y);
				water->SetEnable(true);
				unit = new Unit(grid, water, pos_x, pos_y, cell_x, cell_y);
				break;
			}
			case BREAKWALL:
			{
				BreakWall * breakwall = new BreakWall();
 				breakwall->SetPosition(pos_x, pos_y);
				breakwall->SetEnable(true);
				breakwall->SetState(NORMAL);
				breakwall->SetIDItem(idItem);
				unit = new Unit(grid, breakwall, pos_x, pos_y, cell_x, cell_y);
				break;
			}
			case BOSS:
			{
				//boss = new Boss();
				boss->SetEntryPosition(pos_x, pos_y);
				boss->SetState(BOSS_IDLE);
				boss->SetEnable(true);
				unit = new Unit(grid, boss, pos_x, pos_y, cell_x, cell_y);
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
	listZombie.clear();

	grid->Get(game->GetCameraPositon(), listUnits);

	for (UINT i = 0; i < listUnits.size(); i++)
	{

		LPGAMEOBJECT obj = listUnits[i]->GetObj();
		listObjects.push_back(obj);

		if (dynamic_cast<Door*>(obj))
			listDoors.push_back(obj);
		if (dynamic_cast<Stair*>(obj))
			listStairs.push_back(obj);
		if (dynamic_cast<Zombie*>(obj))
			listZombie.push_back(obj);
		
	}
}

void Scenes::ChangeScene()
{
	Init(simon->isNextScene, simon->isGameState);
}

void Scenes::Update(DWORD dt)
{
	// Nếu Simon đi qua cửa thì không cần cập nhật hay xét va chạm
	if (SimonWalkThroughDoor() == true) {
		for (int i = 0; i < listZombie.size(); i++)
		{
			LPGAMEOBJECT obj = listZombie[i];
			obj->SetEnable(false);
		}
		return;
	}

	// Lấy danh sách object từ grid 
	GetObjectFromGrid();

	// Cross effect
	CrossEffect();

	// Drop item
	SetDropItems();

	// Set position for enemies's spawning
	SetEnemiesSpawnPositon();

	if (boss->GetState() == BOSS_ACTIVE)
		ShowGround();

	Simon_Update(dt);

	// Khi Simon va chạm với ChangScene objects, tiến hành thay đổi, cập nhật trạng thái
	if (simon->isNextScene != -1)
	{
		ChangeScene();
		simon->isNextScene = -1;
		simon->hadSecrect = false;
		hadItemSecrect = false;
		return;
	}

	Whip_Update(dt);
	Weapon_Update(dt, 0);

	for (UINT i = 0; i < listObjects.size(); i++)
	{
		LPGAMEOBJECT object = listObjects[i];
		vector<LPGAMEOBJECT> coObjects;

		GetColliableObjects(object, coObjects);
		object->Update(dt, &coObjects, !stopWatchTimer->IsTimeUp());
		if (dynamic_cast<Ground*>(object) ){
			Ground * ground = dynamic_cast<Ground*>(object);
			if (ground->state == 30) {
				ground->state = 30;
			}
		}

		if (dynamic_cast<FishMan*>(object))
		{
			FishMan * fishman = dynamic_cast<FishMan*>(object);

			if (fishman->CanHit() == true)
			{
				fishman->SetState(FISHMAN_HIT);

				// Đặt hướng quay mặt của Fishman sau khi bắn (quay về phía simon)
				int new_nx;
				if (fishman->x < simon->x) new_nx = DIR_RIGHT;
				else new_nx = DIR_LEFT;

				fishman->Hit(grid, new_nx);
			}
		}
		else if (dynamic_cast<Boss*>(object))
		{
			float sx, sy;
			simon->GetPosition(sx, sy);
			boss->SetSimonPosition(sx, sy);
			simon->CheckCollisionWithBoss(boss);
		}
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
	if (delayChangeScene->IsTimeUp() == false)
		return;

	tilemaps->Get(IDScene)->Draw(game->GetCameraPositon(), !crossEffectTimer->IsTimeUp());

	for (auto obj : listObjects)
	{
		obj->Render();
		obj->RenderBoundingBox(); 
	}

	simon->Render();
	simon->RenderBoundingBox();

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

		if (((dynamic_cast<Candle*>(object) && object->GetState() == CANDLE_DESTROYED) || (dynamic_cast<Zombie*>(object) && object->GetState() == ZOMBIE_DESTROYED) 
			|| (dynamic_cast<Bat*>(object) && object->GetState() == BAT_DESTROYED) || (dynamic_cast<BlackLeopard*>(object) && object->GetState() == BLACK_LEOPARD_DESTROYED)
			|| (dynamic_cast<FishMan*>(object) && object->GetState() == FISHMAN_DESTROYED) || (dynamic_cast<BreakWall*>(object) && object->GetState() == BREAK)) && object->idItem == -1)
			
		{
			idItem = GetRandomItem(); //object->idItem;
			object->GetPosition(x, y);
			object->SetIsDroppedItem(true);
		}
		else if ((dynamic_cast<Candle*>(object) && object->GetState() == CANDLE_DESTROYED)||((dynamic_cast<BreakWall*>(object) && object->GetState() == BREAK))){
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

bool Scenes::IsInViewport(LPGAMEOBJECT object) // xét 4 cell
{
		D3DXVECTOR2 camPosition = game->GetCameraPositon();

		float obj_x, obj_y;
		object->GetPosition(obj_x, obj_y);

		return obj_x >= camPosition.x - CELL_WIDTH_DEFAULT && obj_x < camPosition.x + SCREEN_WIDTH + CELL_WIDTH_DEFAULT
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

		for (auto object : listObjects)
		{
			if (IsInViewport(object) == false)
			{
				if (dynamic_cast<Zombie*>(object) && object->GetState() == ZOMBIE_ACTIVE)
				{
					auto zombie = dynamic_cast<Zombie*>(object);
					zombie->SetState(ZOMBIE_INACTIVE);
				}
				else if (dynamic_cast<BlackLeopard*>(object) && object->GetState() == BLACK_LEOPARD_ACTIVE)
				{
					auto leopard = dynamic_cast<BlackLeopard*>(object);
					leopard->SetState(BLACK_LEOPARD_INACTIVE);
				}
				else if (dynamic_cast<Bat*>(object) && object->GetState() == BAT_ACTIVE)
				{
					auto bat = dynamic_cast<Bat*>(object);
					bat->SetState(BAT_INACTIVE);
				}
				else if (dynamic_cast<FishMan*>(object)
					&& (object->GetState() == FISHMAN_ACTIVE || object->GetState() == FISHMAN_JUMP))
				{
					auto fishman = dynamic_cast<FishMan*>(object);
					fishman->SetState(FISHMAN_INACTIVE);
				}
			}
		}
}

void Scenes::UpdateCameraPosition()
{
	if (isBossFighting == true)				// Boss fight -> not moving camera
		return;

	if (boss->GetState() == BOSS_ACTIVE)
	{
		isBossFighting = true;
		return;
	}

	if (simon->x + SIMON_BBOX_WIDTH > SCENE_2_1_x &&
		simon->x + SIMON_BBOX_WIDTH < SCENE_2_1_WIDTH) return;
	if (simon->x + SIMON_BBOX_WIDTH > SCENE_2_2_x &&
		simon->x + SIMON_BBOX_WIDTH < SCENE_2_2_WIDTH) return;
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
	//switch (state)
	//{
	//	case GAMESTATE_1:
	//		simon->SetState(STAND);
	//		simon->SetPosition(0, 302);
	//		game->SetCameraPosition(0, 0);
	//		break;

	//	case GAMESTATE_2:
	//		simon->SetState(STAND);
	//		simon->SetPosition(0, 272);
	//		game->SetCameraPosition(0, 0);
	//		break;

	//	case GAMESTATE_2_1:
	//		simon->SetState(STAIR_UP);
	//		simon->SetPosition(3140, 338);
	//		simon->SetOrientation(-1);
	//		simon->isStandOnStair = true;
	//		game->SetCameraPosition(3056, 0);
	//		break;

	//	case GAMESTATE_2_2:
	//		simon->SetState(STAIR_UP);
	//		simon->SetPosition(3780, 336); //-2 y
	//		simon->SetOrientation(-1);
	//		simon->isStandOnStair = true;
	//		game->SetCameraPosition(3056, 0);
	//		break;
	//
	//	case GAMESTATE_2_3:
	//		simon->SetState(STAND);
	//		simon->SetPosition(5100, 48); //-2 y
	//		simon->SetOrientation(1);
	//		game->SetCameraPosition(4684, 0);
	//		break;

	//	case GAMESTATE_3_1:
	//		simon->SetState(STAIR_DOWN);
	//		simon->SetPosition(100, 16);
	//		simon->needStateMoveDownStair = true;
	//		simon->SetOrientation(1);
	//		simon->isStandOnStair = true;
	//		game->SetCameraPosition(0, 0);
	//		break;

	//	case GAMESTATE_3_2:
	//		simon->SetState(STAIR_DOWN);
	//		simon->SetPosition(732, 16);
	//		simon->SetOrientation(1);
	//		game->SetCameraPosition(0, 0);
	//		break;


	//	default:
	//		break;
	//}
}

void Scenes::SetEnemiesSpawnPositon()
{
	float distanceZombie = 0.0f; //  khoảng cách giữa các zombie
	for (auto obj : listObjects)
	{
		if (!obj->IsEnable()) continue;
		if (dynamic_cast<Zombie*>(obj))
		{
			Zombie * zombie = dynamic_cast<Zombie*>(obj);

			if (zombie->GetState() == ZOMBIE_ACTIVE && zombie->isSettedPosition == false)
			{
				zombie->isSettedPosition = true;

				float simon_x, simon_y;
				simon->GetPosition(simon_x, simon_y);

				int nx = zombie->GetEntryPosition().x < simon_x ? 1 : -1;
				zombie->SetOrientation(nx);

				float x, y;
				y = zombie->GetEntryPosition().y;
				if (nx == -1)
					x = game->GetCameraPositon().x + SCREEN_WIDTH + distanceZombie;
				else
					x = game->GetCameraPositon().x - distanceZombie;

				distanceZombie += 50.0f;
				zombie->SetPosition(x, y);
				zombie->SetState(ZOMBIE_ACTIVE);
			}

		}
		else if (dynamic_cast<BlackLeopard*>(obj))
		{
			BlackLeopard * leopard = dynamic_cast<BlackLeopard*>(obj);

			if (leopard->GetState() == BLACK_LEOPARD_INACTIVE)
			{
				if (leopard->IsAbleToActivate() == true && IsInViewport(leopard) == true
					&& abs(simon->x - leopard->GetEntryPosition().x) > 200)
				{
					int nx = leopard->GetEntryPosition().x < simon->x ? 1 : -1;
					leopard->SetOrientation(nx);

					leopard->SetState(BLACK_LEOPARD_IDLE);
				}
			}
		}
		else if (dynamic_cast<Bat*>(obj))
		{
			Bat * bat = dynamic_cast<Bat*>(obj);

			if (bat->GetState() != BAT_INACTIVE && bat->isSettedPosition == false)
			{
				bat->isSettedPosition = true;
				bat->SetOrientation(-(simon->GetOrientation()));

				// set vị trí cho dơi
				// dơi bay ngang tầm simon, từ phía cuối của 2 đầu màn hình)
				float x, y;
				//int randomDistance = rand() % 30;

				y = simon->y - (SIMON_BBOX_HEIGHT / 6) + (rand() % (SIMON_BBOX_HEIGHT / 3)); // y cao hơn đầu simon và thấp tới giữa ngực simon

				if (bat->GetOrientation() == -1)
					x = game->GetCameraPositon().x + SCREEN_WIDTH + ENEMY_DEFAULT_BBOX_WIDTH;
				else
					x = game->GetCameraPositon().x - ENEMY_DEFAULT_BBOX_WIDTH;

				bat->SetPosition(x, y);
				bat->SetState(BAT_ACTIVE);
			}
		}
		else if (dynamic_cast<FishMan*>(obj))
		{
			FishMan * fishman = dynamic_cast<FishMan*>(obj);

			if (fishman->GetState() != FISHMAN_INACTIVE && fishman->isSettedPosition == false)
			{
				fishman->isSettedPosition = true;

				// Set vị trí cho fishman dựa vào vị trí của Simon
				float simon_x, simon_y;
				simon->GetPosition(simon_x, simon_y);

				int nx = simon_x > fishman->GetEntryPosition().x ? 1 : -1;

				float x = fishman->GetEntryPosition().x;
				float y = fishman->GetEntryPosition().y;

				fishman->SetOrientation(nx);
				fishman->SetPosition(x, y);

				fishman->SetState(FISHMAN_JUMP);

				// Thêm bubbles vào water để render bọt nước
				water->AddBubbles(x, y);
			}
		}
	}
}

void Scenes::Simon_Update(DWORD dt)
{
	if (simon->hadSecrect && !hadItemSecrect) {
		hadItemSecrect = true;
		auto item = new Items();
		item->SetEnable(true);
		item->SetPosition(ITEM_SECRECT_X, ITEM_SECRECT_Y);

		item->SetState(MONEY_BAG_BLUE_SECRECT);

		listItems.push_back(item);
		unit = new Unit(grid, item, ITEM_SECRECT_X, ITEM_SECRECT_Y);
	}
	vector<LPGAMEOBJECT> coObjects;
	GetColliableObjects(simon, coObjects);

	simon->Update(dt, &coObjects);
	simon->CheckCollisionWithItem(&listItems);
	simon->CheckCollisionWithEnemyActiveArea(&listObjects);
}

void Scenes::GetColliableObjects(LPGAMEOBJECT Obj, vector<LPGAMEOBJECT>& coObjects)
{
	if (dynamic_cast<BreakWall*>(Obj) && Obj->GetState() == NORMAL) 
		coObjects.push_back(Obj);
	else if (dynamic_cast<Items*>(Obj))
	{
		for (auto obj : listObjects)
		{
			if (dynamic_cast<Ground*>(obj) || dynamic_cast<Water*>(obj) || dynamic_cast<BreakWall*>(obj))
				coObjects.push_back(obj);
		}	
	}
	else if (dynamic_cast<Simon*>(Obj))
	{
		for (LPGAMEOBJECT obj : listObjects)
		{
			if (dynamic_cast<NextSceneObject*>(obj) || dynamic_cast<Ground*>(obj) || dynamic_cast<Door*>(obj) || (dynamic_cast<BreakWall*>(obj) && obj->GetState() == NORMAL))
				coObjects.push_back(obj);
			else if (dynamic_cast<Zombie*>(obj) || dynamic_cast<BlackLeopard*>(obj) && obj->GetState() == ACTIVE 
				|| dynamic_cast<Bat*>(obj) || dynamic_cast<FireBall*>(obj) || dynamic_cast<FishMan*>(obj) || dynamic_cast<Water*>(obj) 
				|| (dynamic_cast<Boss*>(obj) && obj->GetState() == ACTIVE))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Whip*>(Obj))
	{
		for (LPGAMEOBJECT obj : listObjects)
		{
			if (dynamic_cast<Candle*>(obj))
				coObjects.push_back(obj);
			else if (dynamic_cast<Zombie*>(obj) || (dynamic_cast<BlackLeopard*>(obj) && obj->GetState() == ACTIVE) || dynamic_cast<Bat*>(obj)
				|| dynamic_cast<FireBall*>(obj) || dynamic_cast<FishMan*>(obj) || dynamic_cast<BreakWall*>(obj) ||
				(dynamic_cast<Boss*>(obj) && obj->GetState() == ACTIVE))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Weapons*>(Obj))
	{
		for (LPGAMEOBJECT obj : listObjects)
		{
			if (dynamic_cast<Candle*>(obj) || dynamic_cast<Ground*>(obj))
				coObjects.push_back(obj);
			else if (dynamic_cast<Zombie*>(obj) || (dynamic_cast<BlackLeopard*>(obj) && obj->GetState() == ACTIVE) || dynamic_cast<Bat*>(obj)
				|| dynamic_cast<FireBall*>(obj) || dynamic_cast<FishMan*>(obj)
				|| (dynamic_cast<Boss*>(obj) && obj->GetState() == ACTIVE))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Zombie*>(Obj))
	{
		for (auto obj : listObjects)
		{
			if (dynamic_cast<Ground*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<BlackLeopard*>(Obj))
	{
		for (auto obj : listObjects)
		{
			if (dynamic_cast<Ground*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<FishMan*>(Obj))
	{
		for (auto obj : listObjects)
		{
			if (dynamic_cast<Ground*>(obj) || dynamic_cast<Water*>(obj))
				coObjects.push_back(obj);
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
		if (dynamic_cast<Zombie*>(obj))
			listUnits[i]->Move(newPos_x, newPos_y);
	}
}

int Scenes::GetRandomItem()
{
	std::map<int, int> randomRange = {
		{DAGGER,		4},
		{HOLY_WATER,	8},
		{SMALL_HEART,	30},
		{LARGE_HEART,	60},
		{CHAIN,			80},
	};

	bool canDropItem = (rand() % 100) <= 80 ? true : false; // tỉ lệ rớt item là 80/100

	if (canDropItem == false)
		return -1;

	int randomValue = rand() % 100;

	for (auto i = randomRange.begin(); i != randomRange.end(); i++)
	{
		if (((*i).first == CHAIN && whip->GetState() == LONG_CHAIN) || (*i).first == weaponlist[0]->GetState())
			continue; // tránh Powerup LONGCHAIN và trùng weapon

		if (randomValue < (*i).second)
			return (*i).first;
	}
	return -1;
}

bool Scenes::SimonWalkThroughDoor()
{
	// Cập nhật trạng thái Simon đi qua cửa:
	// Di chuyển Camera -> Mở cửa -> AutoWalk -> Di chuyển Camera

	if (simon->isWalkThroughDoor == true && simon->isTouchGround == true)
	{
		simon->isWalkThroughDoor = false;
		//simon->isFalling = false;
		simon->SetOrientation(DIR_RIGHT);
		simon->SetState(STAND);

		isMovingCamera1 = true;
		countDxCamera = 0;
	}

	if (isMovingCamera1 == true)
	{
		if (countDxCamera < 224)			// Di chuyển camera một đoạn 224
		{
			countDxCamera += 2;

			D3DXVECTOR3 cam = game->GetCameraPositon();
			game->SetCameraPosition(cam.x + 2, cam.y);

			return true;
		}

		if (isSetSimonAutoWalk == false)	// AutoWalk
		{
			isSetSimonAutoWalk = true;
			simon->SetState(WALK);
			simon->vy = 0;
			simon->AutoWalk(120, STAND, DIR_RIGHT);
		}
		else
		{
			if (simon->isAutoWalk == false)
			{
				if (countDxCamera < 496)	// Di chuyển camera thêm một đoạn -> 496-224
				{
					countDxCamera += 2;

					D3DXVECTOR3 cam = game->GetCameraPositon();
					game->SetCameraPosition(cam.x + 2, cam.y);

					return true;
				}
				else
				{
					isMovingCamera1 = false;
					isSetSimonAutoWalk = false;
					countDxCamera = 0;
				}
			}
		}
	}

	return false;
}

void Scenes::ShowGround() {
	for (UINT i = 0; i < listObjects.size(); i++)
	{
		LPGAMEOBJECT object = listObjects[i];
		if (dynamic_cast<Ground*>(object) && object->state == INACTIVE)
			object->SetState(ACTIVE);
	}
}

void Scenes::CrossEffect() {
	if (simon->isGotCrossItem == true)
	{
		simon->isGotCrossItem = false;
		crossEffectTimer->Start();

		for (UINT i = 0; i < listObjects.size(); i++)
		{
			// Cross chỉ tác dụng với các object nằm trong viewport
			if (IsInViewport(listObjects[i]) == false)
				continue;

			if (dynamic_cast<Zombie*>(listObjects[i]) && listObjects[i]->GetState() == ACTIVE)
			{
				auto zombie = dynamic_cast<Zombie*>(listObjects[i]);
				zombie->SetState(ZOMBIE_DESTROYED);
			}
			else if (dynamic_cast<BlackLeopard*>(listObjects[i]) && listObjects[i]->GetState() == ACTIVE)
			{
				auto leopard = dynamic_cast<BlackLeopard*>(listObjects[i]);
				leopard->SetState(BLACK_LEOPARD_DESTROYED);
			}
			else if (dynamic_cast<Bat*>(listObjects[i]) && listObjects[i]->GetState() == ACTIVE)
			{
				auto bat = dynamic_cast<Bat*>(listObjects[i]);
				bat->SetState(BAT_DESTROYED);
			}
			else if (dynamic_cast<FishMan*>(listObjects[i]) && listObjects[i]->GetState() == ACTIVE)
			{
				auto fishman = dynamic_cast<FishMan*>(listObjects[i]);
				fishman->SetState(FISHMAN_DESTROYED);
			}
		}
	}
}
