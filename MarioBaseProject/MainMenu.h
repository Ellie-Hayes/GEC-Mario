#pragma once
#include "GameScreen.h"
#include "Background.h"
#include "Commons.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "CharacterKoopa.h"
#include "CharacterCoin.h"
#include "Collisions.h"
#include "Character.h"
#include "LevelMap.h"
#include "PowBlock.h"
#include "Tile.h"
#include "TileTypes.h"
#include "TextRenderer.h"
#include "UIHealth.h"

#include <vector>
#include <unordered_set>  
#include <sstream>
class MainMenu : public GameScreen
{
	/*MainMenu(SDL_Renderer* renderer);
	~MainMenu();

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;

private:
	bool SetUpLevel();
	void SetLevelMap();

	void Set8BitTileNeighbours(std::vector<Vector2D>& passedVector, int typeCheck);
	void Set4BitTileNeighbours(std::vector<Vector2D>& passedVector, int typeCheck);
	void CreateTiles(Vector2D position, std::string binaryType);
	void CreateWaterLavaTiles(Vector2D position, std::string binaryType, std::string palette);
	void CreatePlatforms(Vector2D position, std::string binaryType);
	void PaintTile(Vector2D position, std::string filePath);
	void PaintDecoTile(Vector2D position, std::string filePath);

	SDL_Rect* camera;
	LevelMap* m_level_map;
	Background* background;
	std::vector<Tile*> m_tiles;
	std::vector<Tile*> m_decoTiles;

	TextRenderer* m_title_text;
	TextRenderer* m_play_text;

	Direction2D* direction2D;
	std::vector<Vector2D> wallPositions;
	std::vector<Vector2D> waterPositions;
	std::vector<Vector2D> lavaPositions;
	std::vector<Vector2D> platformPositions;
	std::string levelPalette;*/


protected:

};

