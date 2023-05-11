#pragma once
#ifndef _GAMESCREEN_H
#define _GAMESCREEN_H
#include <SDL.h>
#include "Background.h"
#include "Commons.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "CharacterKoopa.h"
#include "CharacterCoin.h"
#include "Collisions.h"
#include "Character.h"
#include "EndDoor.h"
#include "LevelMap.h"
#include "PowBlock.h"
#include "Tile.h"
#include "TileTypes.h"
#include "TextRenderer.h"
#include "UIHealth.h"

#include <vector>
#include <unordered_set>  
#include <sstream>
#include <SDL_mixer.h>

class GameScreen
{
public:
	GameScreen(SDL_Renderer* renderer);
	~GameScreen();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	bool GetCompleted() { return level_complete; }
	bool GetRestart() { return level_restart; }
private:

protected:

	//GAME LOGIC
	virtual void DoScreenShake();
	virtual void UpdateEnemies(float deltaTime, SDL_Event e);
	virtual void CreateKoopa(Vector2D position, FACING direction, float speed);
	virtual void UpdateCoins(float deltaTime, SDL_Event e);
	virtual void CreateCoin(Vector2D position);
	virtual void UpdatePowBlock();

	SDL_Renderer* m_renderer;
	SDL_Rect* camera;
	Background* background;

	bool level_complete;
	bool level_restart;
	
	//Characters 
	CharacterMario* mario;
	CharacterLuigi* luigi;
	PowBlock* m_pow_block;

	std::vector<CharacterKoopa*> m_enemies;
	std::vector<CharacterCoin*> m_coins;
	float new_enemy_timer;
	EndDoor* m_end_door;

	bool mario_finished;
	bool luigi_finished;
	//UI
	UIHealth* marioHealth;
	UIHealth* luigiHealth;
	TextRenderer* m_mario_text;
	TextRenderer* m_luigi_text;
	string scoreMessage;

	int mario_score;
	int luigi_score;

	//LEVEL MAP
	virtual void Set8BitTileNeighbours(std::vector<Vector2D>& passedVector, int typeCheck);
	virtual void Set4BitTileNeighbours(std::vector<Vector2D>& passedVector, int typeCheck);
	virtual void CreateTiles(Vector2D position, std::string binaryType);
	virtual void CreateWaterLavaTiles(Vector2D position, std::string binaryType, std::string palette);
	virtual void PaintTile(Vector2D position, std::string filePath);
	virtual void PaintDecoTile(Vector2D position, std::string filePath);
	virtual void SetEnemyPositions(std::vector<Vector2D>& passedVector);
	virtual void EndDoorCollisions();
	virtual void PlaySound(Mix_Chunk* sound);

	LevelMap* m_level_map;
	std::vector<Tile*> m_tiles;
	std::vector<Tile*> m_decoTiles;

	Direction2D* direction2D;
	std::vector<Vector2D> wallPositions;
	std::vector<Vector2D> waterPositions;
	std::vector<Vector2D> lavaPositions;
	std::vector<Vector2D> platformPositions;
	std::vector<Vector2D> enemyPositions; 
	std::string levelPalette;

	Mix_Chunk* coinSound;
	
};

#endif
