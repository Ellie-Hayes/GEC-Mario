#include "Background.h"

Background::Background(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position)
{
	m_renderer = renderer; m_position = start_position;

	m_texture = new Texture2D(m_renderer);
	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load texture!" << std::endl;
	}

	m_source_rect = { 0, 0, m_texture->GetWidth(), m_texture->GetHeight() };
	m_draw_rect = { 0, 0, m_texture->GetWidth(), m_texture->GetHeight() };
}

Background::~Background()
{
	delete m_texture;
	m_texture = nullptr; 
}

void Background::Render(SDL_Rect* camera_rect)
{
	m_draw_rect = { (int)m_position.x - camera_rect->x, (int)m_position.y - camera_rect->y, m_texture->GetWidth(), m_texture->GetHeight() };

	m_texture->Render(m_source_rect, m_draw_rect, SDL_FLIP_NONE);
}
