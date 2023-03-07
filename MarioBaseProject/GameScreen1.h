#pragma once
#ifndef _GAMESCREEN1_H
#define _GAMESCREEN1_H

#include "GameScreen.h"
#include "Commons.h"
#include "Character.h"

class Texture2D;
class Character;

class GameScreen1 : GameScreen
{
	
public: 
	
	
	GameScreen1(SDL_Renderer* renderer);
	~GameScreen1();

	void Render() override; 
	void Update(float deltaTime, SDL_Event e) override;

private:
	bool SetUpLevel();
	Texture2D* m_background_texture;
	Character* my_character;
	
protected:
	

};

#endif
