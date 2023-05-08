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

class MainMenu : GameScreen
{
public: 
	MainMenu(SDL_Renderer* renderer);
	~MainMenu();

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
private:
	bool SetUpLevel();
	void SetLevelMap();

	SDL_Rect* camera;
	Background* background;

	TextRenderer* m_title_text;
	TextRenderer* m_play_text;
	TextRenderer* m_loading_text;

	int m_dots; 
	float dot_timer; 

protected:

};

