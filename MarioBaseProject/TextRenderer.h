#pragma once

#include "SDL_ttf.h"
#include <string>
#include <iostream>

class TextRenderer
{
public:
	TextRenderer(SDL_Renderer* renderer);
	~TextRenderer();

	void Render(int x, int y);
	bool LoadFont(const std::string filePath, int fontSize, const std::string message, SDL_Color fontColor);

private:
	SDL_Renderer* m_renderer;
	SDL_Texture* m_texture; 
	SDL_Rect m_textRect; 
	TTF_Font* font;

};

