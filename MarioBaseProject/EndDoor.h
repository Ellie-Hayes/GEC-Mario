#pragma once

#include <SDL.h>
#include "Commons.h"
#include <iostream>
#include "constants.h"
#include "Texture2D.h"

class EndDoor
{
public:
	EndDoor(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position);
	~EndDoor();

	void Render(SDL_Rect* camera_rect);
	void Update(float deltaTime, SDL_Event e);
	Rect2D GetCollisionBox() {
		return Rect2D(m_position.x, m_position.y,
			m_texture->GetWidth(), m_texture->GetHeight());
	}

private: 
	SDL_Renderer* m_renderer;
	Vector2D m_position;
	Texture2D* m_texture;

	SDL_Rect m_source_rect;
	SDL_Rect m_draw_rect;
};

