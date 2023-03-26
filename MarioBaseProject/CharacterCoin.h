#pragma once
#include "Character.h"
#include "constants.h"

class CharacterCoin : public Character
{
public:
	CharacterCoin(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map, FACING start_facing, float movement_speed);
	~CharacterCoin();

	void Update(float deltaTime, SDL_Event e) override;
	void Render() override;

private: 
	float m_single_sprite_w;
	float m_single_sprite_h;

	float m_frame_delay; 
	int m_current_frame;
};

