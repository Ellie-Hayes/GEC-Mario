#pragma once
#ifndef CHARACTER_MARIO_H
#define CHARACTER_MARIO_H

#include "Character.h"

class CharacterMario : public Character
{
public: 
	CharacterMario(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map);
	void Update(float deltaTime, SDL_Event e) override;
};

#endif