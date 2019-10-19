﻿#pragma once

// main
#define WINDOW_CLASS_NAME		L"Castlevania"
#define MAIN_WINDOW_TITLE		L"Castlevania"
#define BACKGROUND_COLOR				D3DCOLOR_XRGB(0,0,0)
#define BACKGROUND_CROSS_EFFECT_COLOR	D3DCOLOR_XRGB(188,188,188)
#define TILEMAP_TRANSPARENT_COLOR		D3DCOLOR_XRGB(5,5,5)
#define SCREEN_WIDTH			512
#define SCREEN_HEIGHT			480
#define DEFAULT_CELL_WIDTH		256
#define DEFAULT_CELL_HEIGHT		240
#define MAX_FRAME_RATE			150

// Input
#define DIRECTINPUT_VERSION		0x0800
#define KEYBOARD_BUFFER_SIZE	1024

// ID TEX
#define ID_TEX_BBOX					-1
#define ID_TEX_SIMON				0
#define ID_TEX_GROUND				1

// Font
#define FILEPATH_FONT					L"Font\\prstart.ttf"

// Scenes
#define FILEPATH_OBJECTS_SCENE_1		L"Scenes\\Scene1_objects.txt"

// Game
#define GAMESTATE_1		0   // SCENE_1

// Collision direction
#define	CDIR_BOTTOM		-1.0f

// Scene
#define SCENE_1			0

// Simon
#define STAND			0
#define WALK			1
#define SIT				2
#define JUMP			3

// Simon
#define SIMON_UNTOUCHABLE_TIME		2000
#define SIMON_INVISIBILITY_TIME		4000
#define SIMON_WALKING_SPEED			0.13f
#define SIMON_WALKING_SPEED_LOWER	0.05f
#define SIMON_JUMP_SPEED_Y			0.5f
#define SIMON_SPEED_Y_LOWER_ZONE    0.2f
#define SIMON_GRAVITY				0.002f
#define SIMON_GRAVITY_LOWER			0.001f

// Simon BBOX
#define SIMON_BBOX_WIDTH			30
#define SIMON_BBOX_HEIGHT			62

// Ground
#define	GROUND				1

// Ground BBOX
#define GROUND_BBOX_WIDTH			32
#define GROUND_BBOX_HEIGHT			32



