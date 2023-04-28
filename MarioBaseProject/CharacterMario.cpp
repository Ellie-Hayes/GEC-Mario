#include "CharacterMario.h"
CharacterMario::CharacterMario(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map, FACING start_facing, float movement_speed) : Character(renderer, imagePath, start_position, map, start_facing, movement_speed)
{
	m_facing_direction = start_facing;
	m_movement_speed = movement_speed;
	m_current_level_map = map; 

	m_source_rect = { 0, 0, m_texture->GetWidth(), m_texture->GetHeight() };
	m_draw_rect = { 0, 0, m_texture->GetWidth(), m_texture->GetHeight() };
}

void CharacterMario::Update(float deltaTime, SDL_Event e)
{
	Collisions(deltaTime);


	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_a:
			m_moving_left = true;
			break;

		case SDLK_d:
			m_moving_right = true;
			break;
		case SDLK_w:
			if (m_can_jump)
			{
				Jump(INITIAL_JUMP_FORCE);
			}
			break;
		}

		break;
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_a:
			m_moving_left = false;
			break;

		case SDLK_d:
			m_moving_right = false;
			break;
		}
	}

	SetMovingAndJump(deltaTime);

}
