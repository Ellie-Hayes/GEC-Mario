#include "CharacterMario.h"
CharacterMario::CharacterMario(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map, FACING start_facing, float movement_speed) : Character(renderer, imagePath, start_position, map, start_facing, movement_speed)
{
	m_facing_direction = start_facing;
	m_movement_speed = movement_speed;
	m_current_level_map = map; 
	m_health = 5;
	m_alive = true;

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
			if (m_can_jump && !can_climb) { Jump(INITIAL_JUMP_FORCE); }
			else if (can_climb) { m_climbing = true; }
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
		case SDLK_w:
			m_climbing = false;
			break;
		}
	}

	if (m_position.x < 0) { can_move_left = false; }
	else if (m_position.x > (MAP_WIDTH * TILE_WIDTH) - m_texture->GetWidth()) { can_move_right = false; }

	if (m_climbing) { Climb(deltaTime); }
	SetMovingAndJump(deltaTime);

}
