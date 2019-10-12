#include "Map.h"

CMap::CMap(LPCSTR fileMatrixMap, LPSCTR fileTileSet)
{
	TileWidth = TILE_WIDTH;
	TileHeight = TILE_HEIGHT;
	//current

	if (fileMatrixMap != "") {
		AddTileSet(fileMatrixMap, fileTileSet);
		LoadMatrixBackground(fileMatrixMap);
	}
	else {

	}
}

CMap::~CMap() {
	if (Matrix) {
		for (int i = 0; i < Rows; i++) {
			delete Matrix[i];
		}
		delete Matrix;

		Matrix = nullptr;



	}
}

void CMap::AddTileSet(LPCSTR fileMatrixMap, LPCSTR fileTileSet) {

}

void CMap::LoadMatrixBackground(LPCSTR fileMatrixMap) {
	ifstream ifs(fileMatrixMap);
	ifs >> Columns >> Rows;
	Matrix = new int*[Rows];
	for (int i=0;i<Rows;i++) 
		for (int j = 0; j < Columns; j++) {
			ifs >> Matrix[i][j];
		}
	MapWidth = Columns * TileWidth;
	MapHeight = Rows * TileHeight;
}

void CMap::DrawTileBackground() {
	RECT tileRect;
	D3DXVECTOR3 camPos(0, 0, 0);
	int cameraWidth = 0;
	int cameraHight = 0;

	for (int i=0; i<Rows; i++)
		for (int j = 0; j < Columns; j++) {
			TileSet = new CSprite(MAP, Matrix[i][j] - 1, 49);
			TileSet->SetTexture(CTextureData)
		}
}