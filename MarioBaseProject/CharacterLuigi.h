#pragma once
#ifndef CHARACTER_LUIGI_H
#define CHARACTER_LUIGI_H
#include "Character.h"
class CharacterLuigi : public Character
{
public:
	CharacterLuigi(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map);
	void Update(float deltaTime, SDL_Event e) override;
};

#endif
