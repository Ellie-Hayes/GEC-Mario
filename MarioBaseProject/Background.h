#pragma once
#include <SDL.h>
#include "Commons.h"
#include <iostream>
#include "constants.h"
#include "Texture2D.h"

class Background
{
public: 
	Background(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position);
	~Background();

	void Render(SDL_Rect* camera_rect);

private:
	SDL_Renderer* m_renderer;
	Vector2D m_position;
	Texture2D* m_texture;

	SDL_Rect m_source_rect;
	SDL_Rect m_draw_rect;
};

