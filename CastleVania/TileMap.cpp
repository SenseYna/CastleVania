#include "TileMap.h"
#include "Define.h"

TileMap::TileMap(int ID, LPCWSTR filePath_tex, LPCWSTR filePath_data, int map_width, int map_height, int tile_width, int tile_height)
{
	this->ID = ID;

	this->filePath_tex = filePath_tex;
	this->filePath_data = filePath_data;

	this->map_Width = map_width;
	this->map_Height = map_height;

	this->tile_Width = tile_width;
	this->tile_Height = tile_height;

	nums_row = map_Height / tile_Height;
	nums_col = map_Width / tile_Width;

	LoadResources();
	LoadMap();
}

void TileMap::LoadResources()
{
	Textures * textures = Textures::GetInstance();

	textures->Add(ID, filePath_tex, TILEMAP_TRANSPARENT_COLOR);
	
	//IDirect3DTexture9: Sử dụng thuộc tính của .. để thao tác với Texture: surfaceDesc => bên dưới
	LPDIRECT3DTEXTURE9 texTileMap = textures->Get(ID);  

	// 1 cái struct chứa thông tin về kích thước: width, height, ... ; của texture (filePath_tex) **Describes Surface: Mô tả bề mặt
	D3DSURFACE_DESC surfaceDesc;
	int level = 0; 

	// Sử dụng thuộc tính của IDirect3DTexture9 để lấy thông tin width, height của texture bất kì và lưu vào &surfaceDesc
	texTileMap->GetLevelDesc(level, &surfaceDesc);
	
	// tính toán số hàng, số cột cần thiết để đọc các ô tile từ file
	int nums_rowToRead = surfaceDesc.Height / tile_Height;
	int nums_colToRead = surfaceDesc.Width / tile_Width;

	// thực hiện lưu danh sách các tile vô sprites theo thứ tự id_sprite
	int id_sprite = 1;

	for (UINT i = 0; i < nums_rowToRead; i++)
	{
		for (UINT j = 0; j < nums_colToRead; j++)
		{
			string idTile = "map_" + to_string(ID) + "_tile_" + to_string(id_sprite);
			sprites->Add(idTile, tile_Width * j, tile_Height * i, tile_Width * (j + 1), tile_Height * (i + 1), texTileMap);
			id_sprite++;
		}
	}
}

void TileMap::LoadMap()
{
	fstream fs;
	fs.open(filePath_data, ios::in);

	if (fs.fail())
	{
		DebugOut(L"[ERROR] TileMap::Load_MapData failed: ID=%d", ID);
		fs.close();
		return;
	}

	string line;

	while (!fs.eof())
	{
		getline(fs, line);
		vector<LPSPRITE> spriteline; 
		// tách xâu
		stringstream ss(line); 
		int n;

		while (ss >> n)
		{
			string idTile = "map_" + to_string(ID) + "_tile_" + to_string(n);
			spriteline.push_back(sprites->Get(idTile));
		}
		tilemap.push_back(spriteline);
	}

	fs.close();
}

void TileMap::Draw(D3DXVECTOR3 camPosition)
{
	int start_col_to_draw = (int)camPosition.x / 32;
	int end_col_to_draw = start_col_to_draw + SCREEN_WIDTH / 32;
	
	for (UINT i = 0; i < nums_row; i++)
	{
		for (UINT j = start_col_to_draw; j <= end_col_to_draw; j++)
		{
			// +(int)camPosition.x % 32 để giữ cho camera chuyển động mượt
			float x = tile_Width * (j - start_col_to_draw) + camPosition.x -(int)camPosition.x % 32;
			float y = tile_Height * i + 48;

			tilemap[i][j]->Draw(-1, x, y);
		}
	}
}

TileMaps * TileMaps::_instance = NULL;

void TileMaps::Add(int ID, LPCWSTR filePath_tex, LPCWSTR filePath_data, int map_width, int map_height, int tile_width, int tile_height)
{
	LPTILEMAP tilemap = new TileMap(ID, filePath_tex, filePath_data, map_width, map_height, tile_width, tile_height);
	tilemaps[ID] = tilemap;
}

TileMaps * TileMaps::GetInstance()
{
	if (_instance == NULL) _instance = new TileMaps();
	return _instance;
}
