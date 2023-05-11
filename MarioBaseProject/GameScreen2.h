#pragma once
#include "GameScreen.h"
class Texture2D;
class Character;
class PowBlock;

class GameScreen2 : public GameScreen
{

public:

	GameScreen2(SDL_Renderer* renderer);
	~GameScreen2();

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
private:
	bool SetUpLevel();
	void SetLevelMap();

	bool level_loaded;


protected:


};
