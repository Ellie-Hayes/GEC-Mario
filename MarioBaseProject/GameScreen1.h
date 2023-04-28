#pragma once
#ifndef _GAMESCREEN1_H
#define _GAMESCREEN1_H

#
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

class Texture2D;
class Character;
class PowBlock; 

class GameScreen1 : GameScreen
{
	
public: 
	
	
	GameScreen1(SDL_Renderer* renderer);
	~GameScreen1();

	void Render() override; 
	void Update(float deltaTime, SDL_Event e) override;
	void UpdatePowBlock(); 

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
	CharacterMario* mario;
	CharacterLuigi* luigi; 
	LevelMap* m_level_map; 
	PowBlock* m_pow_block;
	Background* background;
	std::vector<CharacterKoopa*> m_enemies;
	std::vector<CharacterCoin*> m_coins; 
	std::vector<Tile*> m_tiles;
	std::vector<Tile*> m_decoTiles;
	float new_enemy_timer; 
	
	TextRenderer* m_mario_text;
	TextRenderer* m_luigi_text;
	string scoreMessage;
	int mario_score;
	int luigi_score;
	int old_score; 

	void DoScreenShake();
	void UpdateEnemies(float deltaTime, SDL_Event e);
	void CreateKoopa(Vector2D position, FACING direction, float speed);
	void UpdateCoins(float deltaTime, SDL_Event e);
	void CreateCoin(Vector2D position);
	
	Direction2D* direction2D;
	std::vector<Vector2D> wallPositions;
	std::vector<Vector2D> waterPositions;
	std::vector<Vector2D> lavaPositions;
	std::vector<Vector2D> platformPositions;
	std::string levelPalette; 

	UIHealth* marioHealth;
	UIHealth* luigiHealth; 
	
protected:
	

};

#endif
