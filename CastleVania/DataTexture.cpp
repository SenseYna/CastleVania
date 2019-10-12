#include "DataTexture.h"

DataTexture* DataTexture::_instance = NULL;

void DataTexture::LoadResources()
{
	for (int i = 1; i <= NUMBER_MAP_LEVEL; ++i)
	{
		_maps.push_back(new Map(i));
	}
}

CMap * DataTexture::GetMap(int level)
{
	return _maps[level - 1];
}

DataTexture * DataTexture::GetInstance()
{
	if (_instance == NULL)
		_instance = new DataTexture();
	return _instance;
}