#pragma once
#include <SDL.h>
#include "Commons.h"
#include <iostream>
#include "constants.h"
#include "Texture2D.h"

class UIHealth
{
public:
	UIHealth(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position);
	~UIHealth(); 

	void Render();
	bool GetIsDead() { return m_health <= 0;  }
	void TakeDamage();
private:

	SDL_Renderer* m_renderer;
	Vector2D m_position;
	Texture2D* m_texture;

	float m_single_sprite_w;
	float m_single_sprite_h;

	int m_health;
};

