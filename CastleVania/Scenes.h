#pragma once

#include "Game.h"
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
	vector<Unit*> listUnits;

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
	
	void Init();						// init simon position, camera position, grid..
	void LoadObjectsFromFile(LPCWSTR FilePath);	// load all objects (position, state, isEnable) from file 
	
	// Update
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

	// Set
	void SetInactivationByPosition();			// Nếu object ra khỏi vùng viewport thì set unable / inactive
	void SetDropItems();	// Xét rơi item cho các object bị huỷ	
	void SetGameState();	// Set vị trí của simon, camera

	// Get
	Simon * GetSimon() { return this->simon; }
	vector<Weapons*> * GetWeaponList() { return &weaponlist; }
	void GetObjectFromGrid();
	void GetColliableObjects(LPGAMEOBJECT curObj, vector<LPGAMEOBJECT>&coObjects);
};

