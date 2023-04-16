//xcopy "$(ProjectDir)SDL2\DLL*.*" "$(OutDir)"

#include <iostream>

#include "constants.h"
#include "Texture2D.h"
#include "Commons.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "GameScreenManager.h"

using namespace std;
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
Mix_Music* g_music = nullptr;
GameScreenManager* game_screen_manager; 
Uint32 g_old_time; 

bool InitSDL();
void CLoseSDL();
bool Update(); 
void Render();

void LoadMusic(string filePath);

int main(int argc, char* args[])
{
	if (InitSDL())
	{
		LoadMusic("Music/Mario.mp3");
		if (Mix_PlayingMusic() == 0)
		{
			Mix_PlayMusic(g_music, -1);
		}

		game_screen_manager = new GameScreenManager(g_renderer, SCREEN_LEVEL1);
		//set the time
		g_old_time = SDL_GetTicks();

	}
	

	//flag to check if we wish to quit
	bool quit = false;

	//Game Loop
	while (!quit)
	{
		Render();
		quit = Update();
	}



	CLoseSDL();

	return 0;


}

bool Update()
{
	Uint32 new_time = SDL_GetTicks();
	SDL_Event e;
	SDL_PollEvent(&e);
	
	switch (e.type)
	{
	case SDL_QUIT:
		return true; 
		break;
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			return true;
			break;
		}
	/*case SDL_MOUSEBUTTONDOWN:
		return true;
		break;*/

	}

	game_screen_manager->Update((float)(new_time - g_old_time) / 1000.0f, e);
	g_old_time = new_time;
	return false; 
}

bool InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL did not initialise. Error: " << SDL_GetError();
		return false;
	}
	else
	{
		//setup passed so create window
		g_window = SDL_CreateWindow("Games Engine Creation",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);
		g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		//did the window get created?
		if (g_window == nullptr)
		{
			//window failed
			cout << "Window was not created. Error: " << SDL_GetError();
			return false;
		}

		if (g_renderer != nullptr)
		{
			//init PNG loading
			int imageFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imageFlags) & imageFlags))
			{
				cout << "SDL_Image could not initialise. Error: " << IMG_GetError();
				return false;
			}

		}
		else
		{
			cout << "Renderer could not initialise. Error: " << SDL_GetError();
			return false;
		}

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			cout << "Mixer could not init. Error: " << Mix_GetError();
			return false;
		}

		if (TTF_Init() == -1)
		{
			cout << "TTF could not init. Error: " << TTF_GetError();
			return false;
		}
	}

	return true; 
}

void Render()
{
	SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(g_renderer);

	game_screen_manager->Render(); 
	SDL_RenderPresent(g_renderer);
}

void LoadMusic(string filePath)
{
	g_music = Mix_LoadMUS(filePath.c_str());
	if (g_music == nullptr)
	{
		cout << "Failed to load music. Error: " << Mix_GetError() << endl;
	}

}

void CLoseSDL()
{
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_DestroyRenderer(g_renderer);
	g_renderer = nullptr;

	delete game_screen_manager;
	game_screen_manager = nullptr;

	Mix_FreeMusic(g_music);
	g_music = nullptr;


	IMG_Quit();
	SDL_Quit();
	TTF_Quit();

}
