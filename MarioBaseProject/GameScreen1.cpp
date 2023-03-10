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
	delete mario;
	mario = nullptr;
	delete luigi; 
	luigi = nullptr; 
}

void GameScreen1::Render()
{
	m_background_texture->Render(Vector2D(), SDL_FLIP_NONE);
	mario->Render();
	luigi->Render();
}

void GameScreen1::Update(float deltaTime, SDL_Event e)
{
	mario->Update(deltaTime, e);
	luigi->Update(deltaTime, e);

	if (Collisions::Instance()->Circle(mario, luigi))
	{
		std::cout << "Circle hit!" << std::endl;
	}
	if (Collisions::Instance()->Box(mario->GetCollisionBox(), luigi->GetCollisionBox()))
	{
		std::cout << "Box hit!" << std::endl;
	}

}

bool GameScreen1::SetUpLevel()
{
	
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/Squiddy.jpg"))
	{
		std::cout << "Failed to load background texture!" << std::endl;
		return false;
	}

	mario = new CharacterMario(m_renderer, "Images/Mario.png", Vector2D(64, 330));
	luigi = new CharacterLuigi(m_renderer, "Images/Luigi.png", Vector2D(64, 330));
	return false;
}
