#pragma once
#ifndef POW_BLOCK_H
#define POW_BLOCK_H
#include <SDL.h>
#include "LevelMap.h"
#include "Commons.h"
#include "Texture2D.h"
class PowBlock
{
public:
	PowBlock(SDL_Renderer* renderer, LevelMap* map);
	~PowBlock(); 

	void Render(SDL_Rect* camera_rect);

	Rect2D GetCollisionsBox() {
		return Rect2D(m_position.x, m_position.y,
			m_texture->GetWidth()/3, m_texture->GetHeight());
	};

	void TakeHit(); 
	bool IsAvailable() { return m_num_hits_left > 0; };

private: 
	SDL_Renderer* m_renderer;
	Vector2D m_position;
	Texture2D* m_texture;
	LevelMap* m_level_map;

	float m_single_sprite_w;
	float m_single_sprite_h;
	int m_num_hits_left;

	
};

#endif
