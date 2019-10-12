#pragma once
#include <fstream>
#include <sstream>
#include <d3dx9.h>
#include <string.h>
#include "Sprites.h"

using namespace std;

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

class CMap
{
private: 
	int ** Matrix; 
	int Columns, Rows;
	int TileWidth, TileHeight;
	int MapWidth, MapHeight;
	LPSPRITE TileSet;
	vector<LPSPRITE> listTileSet;
	vector<LPCSTR> listMapdata;

	void LoadMatrixBackground(LPCSTR fileMatrixMap);



public: 
	void DrawTileBackground();
	CMap(LPCSTR fileMatrixMap, LPCSTR fileTileSet);
	~CMap();

	void AddTileSet(LPCSTR fileMatrixMap, LPSTR fileTileSet);

	void Draw(); 

	int GetMapWidth() { return this->MapWidth; }
	int GetMapHeight() { return this->MapHeight; }

	void Update(DWORD dt);
	};

	typedef CMap * LPMAP;