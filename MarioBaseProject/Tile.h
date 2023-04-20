#pragma once
#include "Texture2D.h"
#include "constants.h"
#include "Commons.h"
#include <iostream>

class Tile
{
public:
    Tile(SDL_Renderer* renderer, std::string path, Vector2D startPosition);
    ~Tile(void);

    void Render(SDL_Rect* camera_rect);
private:
    Texture2D* m_texture;
    SDL_Renderer* m_renderer; 

    //static const int Width;
    //static const int Height;

    //static const Vector2D* Size;
    Vector2D m_position;
  
    SDL_Rect m_source_rect;
    SDL_Rect m_draw_rect;

};

