#include "CharacterLuigi.h"

CharacterLuigi::CharacterLuigi(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map, FACING start_facing, float movement_speed) : Character(renderer, imagePath, start_position, map, start_facing, movement_speed)
{
	m_facing_direction = start_facing;
	m_movement_speed = movement_speed;
	m_current_level_map = map;

	m_source_rect = { 0, 0, m_texture->GetWidth(), m_texture->GetHeight() };
	m_draw_rect = { 0, 0, m_texture->GetWidth(), m_texture->GetHeight() };
}

void CharacterLuigi::Update(float deltaTime, SDL_Event e)
{

	int centralX_position = (int)(m_position.x + (m_texture->GetWidth() * 0.5));
	int foot_position = (int)(m_position.y + m_texture->GetHeight());

	if (m_current_level_map->GetTileAt(foot_position, centralX_position) == 0)
	{
		AddGravity(deltaTime);
	}
	else
	{
		//collided with ground so we can jump again
		m_can_jump = true;
	}

	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			m_moving_left = true;
			break;

		case SDLK_RIGHT:
			m_moving_right = true;
			break;
		case SDLK_UP:
			if (m_can_jump)
			{
				Jump();
			}
			break;
		}

		break;
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			m_moving_left = false;
			break;

		case SDLK_RIGHT:
			m_moving_right = false;
			break;
		}
	}

	SetMovingAndJump(deltaTime);
	

}
