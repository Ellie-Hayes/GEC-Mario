#include "Tile.h"

Tile::Tile(SDL_Renderer* renderer, std::string path, Vector2D startPosition)
{
	m_renderer = renderer; m_position = startPosition;
	
	m_texture = new Texture2D(m_renderer);
	if (!m_texture->LoadFromFile(path))
	{
		std::cout << "Failed to load texture!" << std::endl;
	}

	std::cout << "tile made" << std::endl; 

	m_source_rect = { 0, 0, TILE_WIDTH, TILE_HEIGHT };
	m_draw_rect = { 0, 0, TILE_WIDTH, TILE_HEIGHT };
}

Tile::~Tile(void)
{
}

void Tile::Render(SDL_Rect* camera_rect)
{
	m_draw_rect = { (int)m_position.x - camera_rect->x, (int)m_position.y - camera_rect->y, TILE_WIDTH, TILE_HEIGHT };

	m_texture->Render(m_source_rect, m_draw_rect, SDL_FLIP_NONE);


}