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
//#include <map>

using namespace std;

class Scenes
{
	int IDScene;

	Game * game;
	Grid * grid;
	Unit * unit;

	vector<LPGAMEOBJECT> listObjects;
	vector<Unit*> listUnits;

	Simon * simon;
	Whip * whip;
	Weapons * weapons;

	TileMaps * tilemaps = TileMaps::GetInstance();
	Textures * textures = Textures::GetInstance();
	Sprites * sprites = Sprites::GetInstance();
	Animations * animations = Animations::GetInstance();
	//Simon * simon = Simon::GetInstance();
	vector<Weapons*> weaponlist;


	int countDxCamera = 0;

public:


	Scenes(Game * game);
	~Scenes();
	
	void Init();						// init simon position, camera position, grid..
	void LoadObjectsFromFile(LPCWSTR FilePath);	// load all objects (position, state, isEnable) from file 

	void GetObjectFromGrid();
	void Simon_Update(DWORD dt);
	
	void Update(DWORD dt);						
				
	void Whip_Update(DWORD dt);
	void Weapon_Update(DWORD dt, int index);

	void GetColliableObjects(LPGAMEOBJECT curObj, vector<LPGAMEOBJECT>&coObjects);
	void UpdateCameraPosition();		

	bool IsInViewport(LPGAMEOBJECT object);
	void SetInactivationByPosition();			// Nếu object ra khỏi vùng viewport thì set unable / inactive
	void UpdateGrid();

	void Render();

	
	//bool IsInViewport(LPGAMEOBJECT object);

	void SetGameState();	// Set vị trí của simon, camera theo id state

	// Get, Set
	
	Simon * GetSimon() { return this->simon; }
	vector<Weapons*> * GetWeaponList() { return &weaponlist; }

};

