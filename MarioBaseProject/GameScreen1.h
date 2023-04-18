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
#include "TextRenderer.h"
#include <vector>

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

	SDL_Rect* camera;
	CharacterMario* mario;
	CharacterLuigi* luigi; 
	LevelMap* m_level_map; 
	PowBlock* m_pow_block;
	Background* background;
	std::vector<CharacterKoopa*> m_enemies;
	std::vector<CharacterCoin*> m_coins; 
	float new_enemy_timer; 
	
	TextRenderer* m_text;
	string scoreMessage;
	int score; 
	int old_score; 

	void DoScreenShake();
	void UpdateEnemies(float deltaTime, SDL_Event e);
	void CreateKoopa(Vector2D position, FACING direction, float speed);
	void UpdateCoins(float deltaTime, SDL_Event e);
	void CreateCoin(Vector2D position);


	
protected:
	

};

#endif
