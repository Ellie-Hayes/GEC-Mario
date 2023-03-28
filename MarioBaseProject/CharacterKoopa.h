#pragma once
#ifndef CHARACTER_KOOPA_H
#define CHARACTER_KOOPA_H

#include "Character.h"
class CharacterKoopa : public Character
{
public:
	CharacterKoopa(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map, FACING start_facing, float movement_speed);
	~CharacterKoopa();
	void Update(float deltaTime, SDL_Event e) override;
	void Render(SDL_Rect* camera_rect) override;

	void TakeDamage();
	void Jump();

	bool GetInjured() { return m_injured; }

private: 
	float m_single_sprite_w; 
	float m_single_sprite_h;

	bool m_injured; 
	float m_injured_time; 

	void FlipRightWayUp();


};

#endif

