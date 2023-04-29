#include "GameScreenManager.h"
#include "GameScreen.h"
#include "GameScreen1.h"
#include "MainMenu.h"
GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen)
{
	m_renderer = renderer; 
	m_current_screen = nullptr;
	ChangeScreen(startScreen);
}

GameScreenManager::~GameScreenManager()
{
	m_renderer = nullptr; 
	delete m_current_screen;
	m_current_screen = nullptr;
}

void GameScreenManager::Render()
{
	m_current_screen->Render();
}

void GameScreenManager::Update(float deltaTime, SDL_Event e)
{
	m_current_screen->Update(deltaTime, e);
}

void GameScreenManager::ChangeScreen(SCREENS new_screen)
{
	if (m_current_screen != nullptr)
	{
		delete m_current_screen;
	}

	switch (new_screen)
	{
	case SCREEN_MENU:
		MainMenu* menuScreen;
		menuScreen = new MainMenu(m_renderer);
		m_current_screen = (GameScreen*)menuScreen;
		menuScreen = nullptr;
		break;
	case SCREEN_LEVEL1:
		GameScreen1* tempScreen;
		tempScreen = new GameScreen1(m_renderer);
		m_current_screen = (GameScreen*)tempScreen;
		tempScreen = nullptr;
		break;
	default:;
	}

}
