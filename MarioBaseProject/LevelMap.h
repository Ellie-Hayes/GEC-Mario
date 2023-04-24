#pragma once
#ifndef _LEVELMAP_H
#define _LEVELMAP_H

#include "constants.h"

using namespace std;
#include <iostream>
#include <fstream>

class LevelMap
{

public:
	LevelMap(int map[MAP_HEIGHT][MAP_WIDTH]);
	~LevelMap();

	int GetTileAt(unsigned int h, unsigned int w);
	void ChangeTileAt(unsigned int row, unsigned int column, unsigned int new_value);
	void LoadLevelMap(std::string path);

	int** m_map;
protected:
	

};

#endif

