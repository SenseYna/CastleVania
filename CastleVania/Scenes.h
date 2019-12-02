#pragma once

#include "Game.h"
#include "NextSceneObject.h"
#include "Grid.h"
#include "GameObject.h"
#include "TileMap.h"
#include "Ground.h"
#include "Candle.h"
#include "Whip.h"
#include "Weapons.h"
#include "Simon.h"
#include "Items.h"
#include "Timer.h"
#include "Door.h"
#include "Stair.h"
#include "Zombie.h"
#include "BlackLeopard.h"
#include "Bat.h"

#include <map>

using namespace std;

class Scenes
{
	int IDScene;

	Game * game;
	Grid * grid;
	Unit * unit;

	vector<LPGAMEOBJECT> listObjects;
	vector<LPGAMEOBJECT> listItems;
	vector<LPGAMEOBJECT> listDoors;
	vector<LPUNIT> listUnits;
	vector<LPGAMEOBJECT> listStairs;
	vector<LPGAMEOBJECT> listZombie;
	//vector<LPUNIT> listEnemys;

	Simon * simon;
	Whip * whip;
	Weapons * weapons;

	TileMaps * tilemaps = TileMaps::GetInstance();
	Textures * textures = Textures::GetInstance();
	Sprites * sprites = Sprites::GetInstance();
	Animations * animations = Animations::GetInstance();

	vector<Weapons*> weaponlist;

public:
	Scenes(Game * game);
	~Scenes();

	Timer * delayChangeScene = new Timer(CHANGE_SCENE_TIME_DELAY);
	
	void Init(int idScene);						// init simon position, camera position, grid..
	void LoadObjectsFromFileToGrid(LPCWSTR FilePath);	// load all objects (position, state, isEnable) from file 
	
	// Update
	void ChangeScene();							// Chuyển đổi scene khi Simon va chạm với ChangeSceneObject
	void Update(DWORD dt);	
	void Simon_Update(DWORD dt);
	void Whip_Update(DWORD dt);
	void Weapon_Update(DWORD dt, int index);
	void UpdateCameraPosition();
	void UpdateGrid();

	// Render
	void Render();
	 
	// Bool
	bool IsInViewport(LPGAMEOBJECT object);
	bool SimonWalkThroughDoor();
	bool isSetSimonAutoWalk = false;
	bool isMovingCamera1;
	bool isMovingCamera2 = false;
	int countDxCamera = 0;

	// Set
	void SetInactivationByPosition();			// Nếu object ra khỏi vùng viewport thì set unable / inactive
	void SetDropItems();	// Xét rơi item cho các object bị huỷ	
	void SetGameState(int state);	// Set vị trí của simon, camera
	void SetEnemiesSpawnPositon();	// Set position cho khởi tạo enemies
	int delayChangScene = -1; // màn hình đen chuyển màn => đợi sinh zombie 

	// Get
	Simon * GetSimon() { return this->simon; }
	vector<Weapons*> * GetWeaponList() { return &weaponlist; }
	void GetObjectFromGrid();
	void GetColliableObjects(LPGAMEOBJECT curObj, vector<LPGAMEOBJECT>&coObjects);
	vector<LPGAMEOBJECT> * GetListStairs() { return &(listStairs); }
	int GetRandomItem();
};

