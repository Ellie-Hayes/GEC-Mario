//xcopy "$(ProjectDir)SDL2\DLL*.*" "$(OutDir)"

#include <iostream>

#include "constants.h"
#include "Texture2D.h"
#include "Commons.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

using namespace std;
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
Texture2D* g_texture = nullptr; 

bool InitSDL();
void CLoseSDL();
bool Update(); 
void Render();

int main(int argc, char* args[])
{
	InitSDL();

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
	case SDL_MOUSEBUTTONDOWN:
		return true;
		break;

	}

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

			g_texture = new Texture2D(g_renderer);
			if (!g_texture->LoadFromFile("Images/Squiddy.jpg"))
			{
				return false;
			}

		}
		else
		{
			cout << "Renderer could not initialise. Error: " << SDL_GetError();
			return false;
		}

	}

}

void Render()
{
	SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(g_renderer);

	g_texture->Render(Vector2D(), SDL_FLIP_NONE);

	SDL_RenderPresent(g_renderer);
}

void CLoseSDL()
{
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_DestroyRenderer(g_renderer);
	g_renderer = nullptr;
	delete g_texture;
	g_texture = nullptr;


	IMG_Quit();
	SDL_Quit();

}
