#pragma once

#include <vector>
#include "GameObject.h"

class Unit 
{
	friend class Grid;

private:
	Grid * grid;
	LPGAMEOBJECT obj;
	float x;
	float y;
	
	Unit * prev;
	Unit * next;

public:
	Unit(Grid * grid, LPGAMEOBJECT obj, float x, float y, int cell_x = -1, int cell_y = -1);
	LPGAMEOBJECT GetObj() { return this->obj; }

};

class Grid
{
	int map_width;
	int map_height;

	int cell_width;
	int cell_height;

	int numberOfColumns;
	int numberOfRows;

	vector<vector<Unit*>> cells;

public:
	Grid(int map_width, int map_height, int numberOfRows, int numberOfColumns);
	~Grid();
				
	void Add(Unit * unit, int cell_x, int cell_y);
	void Get(D3DXVECTOR3 camPosition, vector<Unit*>& listUnits); // lấy tất cả các Unit* nằm trong vùng viewport để Update và Render

	//void Out();
};

