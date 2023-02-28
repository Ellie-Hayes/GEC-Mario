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
}

void GameScreen1::Render()
{
	m_background_texture->Render(Vector2D(), SDL_FLIP_NONE);
}

void GameScreen1::Update(float deltaTime, SDL_Event e)
{

}

bool GameScreen1::SetUpLevel()
{
	/// <summary>
	/// AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
	/// </summary>
	/// <returns></returns>
	m_background_texture = new Texture2D*();
	if (!m_background_texture->LoadFromFile("Images/Squiddy.jpg"))
	{
		std::cout << "Failed to load background texture!" << std::endl;
		return false;
	}

	return false;
}
