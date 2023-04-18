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
	void Update(float deltaTime);
	void SetScreenShake() { m_screenshake = true; }

private:
	SDL_Renderer* m_renderer;
	Vector2D m_position;
	Texture2D* m_texture;

	float m_shake_time;
	float m_wobble;

	SDL_Rect m_source_rect;
	SDL_Rect m_draw_rect;
	bool m_screenshake;
};

