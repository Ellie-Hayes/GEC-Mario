#pragma once
#include <SDL.h>
#include <vector>
#include <unordered_set>  
#include <sstream>
#include "Tile.h"
#include "TileTypes.h"
#include "CharacterKoopa.h"
#include "CharacterCoin.h"

class TilePainter
{
public: 
	void Set8BitTileNeighbours(std::vector<Vector2D>& passedVector, int typeCheck);
	void Set4BitTileNeighbours(std::vector<Vector2D>& passedVector, int typeCheck);
	void CreateTiles(Vector2D position, std::string binaryType);
	void CreateWaterLavaTiles(Vector2D position, std::string binaryType, std::string palette);
	void CreatePlatforms(Vector2D position, std::string binaryType);
	void PaintTile(Vector2D position, std::string filePath);
	void PaintDecoTile(Vector2D position, std::string filePath);

};