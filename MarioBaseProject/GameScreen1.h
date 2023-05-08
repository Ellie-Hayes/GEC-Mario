#pragma once
#ifndef _GAMESCREEN1_H
#define _GAMESCREEN1_H

#include "GameScreen.h"


class Texture2D;
class Character;
class PowBlock; 

class GameScreen1 : public GameScreen
{
	
public: 
	
	GameScreen1(SDL_Renderer* renderer);
	~GameScreen1();

	void Render() override; 
	void Update(float deltaTime, SDL_Event e) override;
private:
	bool SetUpLevel();
	void SetLevelMap();

	bool level_loaded; 
	
	
protected:
	

};

#endif
