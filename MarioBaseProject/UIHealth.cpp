#include "UIHealth.h"

UIHealth::UIHealth(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position)
{
	m_renderer = renderer; m_position = start_position;

	m_texture = new Texture2D(m_renderer);
	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load texture!" << std::endl;
	}

	m_single_sprite_w = m_texture->GetWidth();
	m_single_sprite_h = m_texture->GetHeight() / 6;
	m_health = 5; 
}

UIHealth::~UIHealth()
{
	m_renderer = nullptr; 
	delete m_texture; 
}

void UIHealth::Render()
{
	int spriteY = 0.0f;
	spriteY = m_single_sprite_h * m_health;

	SDL_Rect portion_of_sprite = { 0,spriteY,m_single_sprite_w, m_single_sprite_h };
	SDL_Rect destRect = { (int)(m_position.x), (int)(m_position.y), m_single_sprite_w, m_single_sprite_h };

	m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_NONE);

}
