﻿#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Define.h"
#include "Debug.h"
#include "Game.h"
#include "Input.h"
#include "Scenes.h"
#include "Player.h"

using namespace std;
Game * game;
Input * input;
Scenes * scene;

TileMaps * tilemaps = TileMaps::GetInstance(); 
Textures * textures = Textures::GetInstance();
Sprites * sprites = Sprites::GetInstance();
Animations * animations = Animations::GetInstance();

// Read data of sprites and animations from file.
void LoadSprites(int id, LPCWSTR tex, LPCWSTR sprite_data, LPCWSTR animation_data)
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

wchar_t* ConvertToWideChar(char* p) // Covert string sang wchar_t*
{
	wchar_t *r;
	r = new wchar_t[strlen(p) + 1];

	char *tempsour = p;
	wchar_t *tempdest = r;
	while (*tempdest++ = *tempsour++);

	return r;
}

void LoadSpritesFromFile(LPCWSTR FilePath)
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

void LoadTileMapFromFile(LPCWSTR FilePath)
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
void LoadResources()
{
	// for render bounding box
	textures->Add(ID_TEX_BBOX, FILEPATH_BBOX_RESOURCE);

	//id tex, texture, sprite, animation
	LoadSpritesFromFile(FILEPATH_TEXTURE_RESOURCE);

	//tile map
	LoadTileMapFromFile(FILEPATH_TILE_MAP_RESOURCE);
}	

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void Update(DWORD dt)
{
	scene->Update(dt);
}

void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// clear back buffer with background color
		// cross effect, random to render background color changed
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		scene->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		MessageBox(GetActiveWindow(), L"CreateWindow failed!", L"ERROR", MB_OK);
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD gamePlayStartTime = GetTickCount();
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = Game::GetInstance();
	game->Init(hWnd);

	LoadResources();

	scene = new Scenes(game);
	scene->Init(0);

	input = new Input(game, scene);
	game->InitKeyboard(input);

	Run();


	return 0;
}