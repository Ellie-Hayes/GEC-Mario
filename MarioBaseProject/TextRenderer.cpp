#include "TextRenderer.h"

TextRenderer::TextRenderer(SDL_Renderer* renderer)
{
	m_renderer = renderer;
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::Render(int x, int y)
{
	m_textRect.x = x;
	m_textRect.y = y; 

	SDL_RenderCopy(m_renderer, m_texture, nullptr, &m_textRect);
}

bool TextRenderer::LoadFont(const std::string filePath, int fontSize, const std::string message, SDL_Color fontColor)
{
	font = TTF_OpenFont(filePath.c_str(), fontSize);

	if (!font)
	{
		std::cout << "Font could not be loaded: " << TTF_GetError();
		return false;
	}
	
	SDL_Surface* text_surface = TTF_RenderText_Solid(font, message.c_str(), fontColor);

	if (text_surface == nullptr)
	{
		std::cout << "text surface could not be loaded: " << TTF_GetError();
		return false; 
	}

	m_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);

	if (m_texture == nullptr)
	{
		std::cout << "Unable to create texture from surface. Error: " << TTF_GetError();
		return false;
	}

	TTF_CloseFont(font);
	SDL_FreeSurface(text_surface);
	SDL_QueryTexture(m_texture, nullptr, nullptr, &m_textRect.w, &m_textRect.h);

	return m_texture;
}
