#pragma once

// main
#define WINDOW_CLASS_NAME		L"Castlevania"
#define MAIN_WINDOW_TITLE		L"Castlevania"
#define BACKGROUND_COLOR				D3DCOLOR_XRGB(0,0,0)
#define BACKGROUND_CROSS_EFFECT_COLOR	D3DCOLOR_XRGB(188,188,188)
#define TILEMAP_TRANSPARENT_COLOR		D3DCOLOR_XRGB(5,5,5)
#define SCREEN_WIDTH			512
#define SCREEN_HEIGHT			480
#define MAX_FRAME_RATE			150


// Input
#define DIRECTINPUT_VERSION		0x0800
#define KEYBOARD_BUFFER_SIZE	1024

// ID TEX
#define ID_TEX_BBOX					-1
#define ID_TEX_BBOX_2				-2
#define ID_TEX_SIMON				0
#define ID_TEX_GROUND				1
#define ID_TEX_CANDLE				2
#define ID_TEX_WHIP					3
#define ID_TEX_SPARK				4
#define ID_TEX_EFFECT				5
#define ID_TEX_SUBWEAPONS			6
#define ID_TEX_ITEMS				7
#define ID_TEX_DOOR					8
#define ID_TEX_STAIR				9

// ID Object
#define	CANDLE				0
#define	GROUND				1
#define	DOOR				2
#define NEXT_SCENE_OBJECT	3
#define STAIR	4

// Font
#define FILEPATH_FONT					L"Font\\prstart.ttf"

// Scenes
#define FILEPATH_OBJECTS_SCENE_1		L"Scenes\\Scene1_objects.txt"
#define FILEPATH_OBJECTS_SCENE_2		L"Scenes\\Scene2_objects.txt"

// Texture Resource
#define FILEPATH_TEXTURE_RESOURCE L"Textures\\Textures_Resource.txt"

// TileMap Resource
#define FILEPATH_TILE_MAP_RESOURCE L"Textures\\TileMap_Resource.txt"

// BBox Filepath
#define FILEPATH_BBOX_RESOURCE L"Textures\\BBox.png"

// Game
#define GAMESTATE_1		0   // SCENE_1
#define GAMESTATE_2		1	// SCENE_2

// Orientation
#define DIR_LEFT		-1
#define DIR_RIGHT		1

// Collision direction
#define	CDIR_BOTTOM		-1.0f
#define	CDIR_TOP		1.0f

// Scene
#define SCENE_1			0
#define SCENE_2 		1


// Simon
#define STAND			0
#define WALK			1
#define SIT				2
#define JUMP			3
#define HIT_STAND		4
#define HIT_SIT			5
#define POWER			6
#define STAIR_UP		7
#define STAIR_DOWN		8
#define HIT_STAIR_UP	9
#define HIT_STAIR_DOWN	10

// Simon
#define SIMON_WALKING_SPEED			0.13f
#define SIMON_WALKING_SPEED_LOWER	0.05f
#define SIMON_JUMP_SPEED_Y			0.5f
#define SIMON_SPEED_Y_LOWER_ZONE    0.2f
#define SIMON_GRAVITY				0.002f
#define SIMON_GRAVITY_LOWER			0.001f
#define SIMON_STAIR_SPEED_X			0.079f
#define SIMON_STAIR_SPEED_Y			0.079f

// Simon BBOX
#define SIMON_BBOX_WIDTH			30
#define SIMON_BBOX_HEIGHT			62

// Whip
#define NORMAL_WHIP		0
#define SHORT_CHAIN		1
#define LONG_CHAIN		2

// Whip BBOX
#define LONG_CHAIN_BBOX_WIDTH		85
#define WHIP_BBOX_WIDTH				55
#define WHIP_BBOX_HEIGHT			15

// Weapons
#define WEAPONS_DAGGER		0
#define WEAPONS_HOLY_WATER  1
#define WEAPONS_HOLY_WATER_SHATTERED 2

// Weapons BOXX
#define WEAPONS_DAGGER_BBOX_WIDTH		34
#define WEAPONS_DAGGER_BBOX_HEIGHT		18
#define WEAPONS_HOLY_WATER_BBOX_WIDTH	32
#define WEAPONS_HOLY_WATER_BBOX_HEIGHT	26

// Weapons Speed
#define WEAPONS_DAGGER_SPEED		0.4f

#define WEAPONS_HOLY_WATER_SPEED_X	0.5f
#define WEAPONS_HOLY_WATER_SPEED_Y	0.05f
#define WEAPONS_HOLY_WATER_GRAVITY	0.001f
#define WEAPONS_HOLY_WATER_TIME_EFFECT	500

// Ground BBOX
#define GROUND_BBOX_WIDTH			32
#define GROUND_BBOX_HEIGHT			32

// Candle
#define BIG_CANDLE		0
#define SMALL_CANDLE	1
#define CANDLE_DESTROYED	2

// Candle BBOX
#define CANDLE_BBOX_WIDTH			32
#define CANDLE_BBOX_HEIGHT			64

#define SMALL_CANDLE_BBOX_WIDTH		16
#define SMALL_CANDLE_BBOX_HEIGHT	32

// Animation Delay
#define EFFECT_ANI_TIME_DELAY		300
#define SPARK_ANI_TIME_DELAY		100
#define HIT_ANI_TIME_DELAY			400
#define DEFLECT_ANI_TIME_DELAY		600
#define POWER_ANI_TIME_DELAY		450
#define STAIR_WALK_ANI_TIME_DELAY	200


// Item 
#define ITEM_FALLING_SPEED_X			0.0f
#define ITEM_FALLING_SPEED_Y			0.15f

#define ITEM_SMALLHEART_FALLING_SPEED_X_MAX			0.25f
#define ITEM_FALLING_SPEED_X_VARIATION	0.05f
#define ITEM_SMALLHEART_FALLING_SPEED_Y	0.03f

#define ITEM_TIME_DESTROYED				4000

// Items
#define DAGGER			0
#define HOLY_WATER		1
#define SMALL_HEART		2
#define LARGE_HEART		3
#define CHAIN			4


// Door
#define DOOR_1			0

// Door BBOX
#define DOOR_BBOX_WIDTH				32
#define DOOR_BBOX_HEIGHT			96

// Next Scene Object
#define NEXTSCENEOBJECT_BBOX_WIDTH			44
#define NEXTSCENEOBJECT_BBOX_HEIGHT			126

// Stair BBOX
#define STAIR_BBOX_WIDTH			32
#define STAIR_BBOX_HEIGHT			32
#define STAIR_BBOX_WIDTH_BONUS		24

