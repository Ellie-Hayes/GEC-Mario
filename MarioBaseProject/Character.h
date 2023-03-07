#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
#include "Commons.h"
#include <iostream>

class Texture2D; 

class Character
{
public: 
	Character(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position);
	~Character();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	void SetPosition(Vector2D new_position);
	Vector2D GetPosition();

private:
protected:
	SDL_Renderer* m_renderer;
	Vector2D m_position; 
	Texture2D* m_texture; 
};

#endif
