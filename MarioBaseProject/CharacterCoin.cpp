#include "CharacterCoin.h"

CharacterCoin::CharacterCoin(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map, FACING start_facing, float movement_speed) : Character(renderer, imagePath, start_position, map, start_facing, movement_speed)
{
	m_position = start_position;

	m_single_sprite_w = m_texture->GetWidth() / 3;
	m_single_sprite_h = m_texture->GetHeight();
	m_alive = true;
	m_frame_delay = ANIMATION_DELAY; 

	m_source_rect = { 0, 0, m_texture->GetWidth() / 3, m_texture->GetHeight() };
	m_draw_rect = { 0, 0, m_texture->GetWidth() / 3, m_texture->GetHeight() };
}

CharacterCoin::~CharacterCoin()
{

}

void CharacterCoin::Update(float deltaTime, SDL_Event e)
{
	m_frame_delay -= deltaTime;
	if (m_frame_delay <= 0.0f)
	{
		//reset frame delay count
		m_frame_delay = ANIMATION_DELAY;

		//move the frame over
		m_current_frame++;

		//loop frame around if it goes beyond the number of frames
		if (m_current_frame > 2)
			m_current_frame = 0;
	}

}

void CharacterCoin::Render(SDL_Rect* camera_rect)
{
	SDL_Rect portion_of_sprite = { m_single_sprite_w * m_current_frame, 0,m_single_sprite_w, m_single_sprite_h };
	SDL_Rect destRect = { (int)(m_position.x - camera_rect->x), (int)(m_position.y - camera_rect->y), m_single_sprite_w, m_single_sprite_h };

	m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_NONE);
}
