#include "GameScreen1.h"
#include <iostream>
#include "Texture2D.h"


GameScreen1::GameScreen1(SDL_Renderer* renderer): GameScreen(renderer)
{
	SetUpLevel();
}

GameScreen1::~GameScreen1()
{
	delete m_background_texture;
	m_background_texture = nullptr; 
	delete my_character;
	my_character = nullptr;
}

void GameScreen1::Render()
{
	m_background_texture->Render(Vector2D(), SDL_FLIP_NONE);
	my_character->Render();
}

void GameScreen1::Update(float deltaTime, SDL_Event e)
{
	my_character->Update(deltaTime, e);
}

bool GameScreen1::SetUpLevel()
{
	
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/Squiddy.jpg"))
	{
		std::cout << "Failed to load background texture!" << std::endl;
		return false;
	}

	my_character = new Character(m_renderer, "Images/Mario.png", Vector2D(64, 330));

	return false;
}
