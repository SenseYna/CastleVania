#include "Grid.h"

Unit::Unit(Grid * grid, LPGAMEOBJECT obj, float x, float y, int cell_x, int cell_y)
{
	this->grid = grid;
	this->obj = obj;

	this->x = x;
	this->y = y;

	this->prev = NULL; 
	this->next = NULL;

	grid->Add(this, cell_x, cell_y);
}

Grid::Grid(int map_width, int map_height, int numberOfRows, int numberOfColumns)
{
	this->map_width = map_width;
	this->map_height = map_height;

	this->numberOfRows = numberOfRows;
	this->numberOfColumns = numberOfRows;

	this->cell_width = (int)map_width / numberOfColumns;
	this->cell_height = (int)map_height / numberOfRows;

	cells.resize(numberOfRows);

	for (int i = 0; i < numberOfRows; i++)
		cells[i].resize(numberOfColumns);

	for (int i = 0; i < numberOfRows; i++)
		for (int j = 0; j < numberOfColumns; j++)
			cells[i][j] = NULL;
}

Grid::~Grid()
{
}

void Grid::Add(Unit * unit, int cell_x, int cell_y)
{
	if (cell_x == -1) { // trường hợp add unit không từ file vd: item rớt ra từ candle
		cell_x = (int)(unit->y / cell_height);
		cell_y = (int)(unit->x / cell_width);
	}

	// thêm vào đầu cell - add head
	unit->prev = NULL;
	unit->next = cells[cell_x][cell_y];
	cells[cell_x][cell_y] = unit;

	if (unit->next != NULL)
		unit->next->prev = unit;
}

void Grid::Get(D3DXVECTOR3 camPosition, vector<Unit*>& listUnits)
{
	
	int start_col = (int)(camPosition.x / cell_width);
	int end_col = ceil((camPosition.x + SCREEN_WIDTH) / cell_width);

	for (int i = 0; i < numberOfRows; i++)
	{
		for (int j = start_col; j < end_col; j++)
		{
			Unit * unit = cells[i][j];

			while (unit != NULL)			
			{
				if (unit->GetObj()->IsEnable() == true)
					listUnits.push_back(unit);
				unit = unit->next;
			}
		}
	}
}
