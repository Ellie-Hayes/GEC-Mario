#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
#include "Commons.h"
#include <iostream>
#include "constants.h"
#include "Texture2D.h"
#include "LevelMap.h"

class Texture2D; 

class Character
{
public: 
	Character(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map, FACING start_facing, float movement_speed);
	~Character();

	virtual void Render(SDL_Rect* camera_rect);
	virtual void Update(float deltaTime, SDL_Event e);
	void SetPosition(Vector2D new_position);
	Vector2D GetPosition();
	float GetCollisionRadius();
	Rect2D GetCollisionBox() {
		return Rect2D(m_position.x, m_position.y,
			m_texture->GetWidth(), m_texture->GetHeight());
	}

	bool IsJumping() { return m_jumping; }
	void CancelJump() { m_jumping = false; }

	bool GetAlive() { return m_alive; }
	void SetAlive(bool isAlive) { m_alive = isAlive; }
	void Knockback(int direction);
	bool IsInvulnerable() { return m_is_invulnerable; }
	int TakeDamage();
	int GetHealth() { return m_health; }
	void SetLevelLoaded(bool loadComplete) { level_loaded = loadComplete; }
	FACING m_facing_direction;
	float m_movement_speed;
private:
	
protected:
	SDL_Renderer* m_renderer;
	Vector2D m_position; 
	Texture2D* m_texture; 
	LevelMap* m_current_level_map;

	SDL_Rect m_source_rect; 
	SDL_Rect m_draw_rect; 
	float m_collision_radius;

	bool level_loaded; 
	bool can_move_left;
	bool can_move_right; 
	bool m_moving_left;
	bool m_moving_right;
	bool m_alive;
	bool m_is_knockedBack;
	bool m_is_invulnerable; 
	int knockback_directon;

	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);
	virtual void AddGravity(float deltaTime);
	virtual void SetMovingAndJump(float deltaTime);
	virtual void Collisions(float deltaTime);
	virtual void Climb(float deltaTime);

	bool m_jumping;
	bool m_can_jump;
	float m_jump_force;
	float m_knockback_force;
	float m_invulnerable_timer;
	float knockback_offset; 
	bool check_knockback; 

	bool can_climb;
	bool m_climbing;
	int m_health; 

	virtual void Jump(int forceAmount);

};

#endif
