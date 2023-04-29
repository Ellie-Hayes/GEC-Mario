#include "Character.h"

Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map, FACING start_facing, float movement_speed)
{
	m_renderer = renderer; m_position = start_position;
	m_facing_direction = start_facing; 
	m_moving_left = false;
	m_moving_right = false; 
	can_move_left = false;
	can_move_right = false;
	m_collision_radius = 15.0f;
	m_knockback_force = 400;
	knockback_offset = 0.2; 

	m_health = 5; 
	can_climb = false; 

	m_texture = new Texture2D(m_renderer);
	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load texture!" << std::endl;
	}

	m_current_level_map = map; 

	m_source_rect = { 0, 0, m_texture->GetWidth(), m_texture->GetHeight() };
	m_draw_rect = { 0, 0, m_texture->GetWidth(), m_texture->GetHeight() };

	m_invulnerable_timer = INVULNERABLE_LENGTH; 
}

Character::~Character()
{
	m_renderer = nullptr; 
}

void Character::Render(SDL_Rect* camera_rect)
{
	m_draw_rect = { (int)m_position.x - camera_rect->x, (int)m_position.y - camera_rect->y, m_texture->GetWidth(), m_texture->GetHeight() };

	if (m_facing_direction == FACING_RIGHT)
	{
		m_texture->Render(m_source_rect, m_draw_rect, SDL_FLIP_NONE);
	}
	else
	{
		m_texture->Render(m_source_rect, m_draw_rect, SDL_FLIP_HORIZONTAL);
	}

}

void Character::Update(float deltaTime, SDL_Event e)
{
	
	
	
}

void Character::SetPosition(Vector2D new_position)
{
	m_position = new_position;
}

Vector2D Character::GetPosition()
{
	return m_position; 
}

float Character::GetCollisionRadius()
{
	return m_collision_radius; 
}

void Character::Knockback(int direction)
{
	knockback_directon = direction;

	switch (direction)
	{
	case 0: 
		m_jumping = false;
		Jump(INITIAL_JUMP_FORCE);
		break;
	case 1:
		m_is_knockedBack = true;
		m_is_invulnerable = true;
		Jump(INITIAL_KNOCKBACK_FORCE);
		break;
	case -1:
		m_is_knockedBack = true;
		m_is_invulnerable = true;
		Jump(INITIAL_KNOCKBACK_FORCE);
		break;
	default:
		break;
	}

	 
}

int Character::TakeDamage()
{
	m_health -= 1;
	if (m_health <= 0)
	{
		m_health = 0;
		m_alive = false;
	}

	return m_health; 
}

void Character::Collisions(float deltaTime)
{
	//collision position variables
	int centralX_position = (int)(m_position.x + (m_texture->GetWidth() * 0.5)) / TILE_WIDTH;
	int centralY_position = (int)(m_position.y + (m_texture->GetHeight() * 0.5)) / TILE_HEIGHT;

	int foot_position = (int)(m_position.y + m_texture->GetHeight()) / TILE_HEIGHT;
	int top_position = (int)(m_position.y) / TILE_HEIGHT;

	int left_position = (int)(m_position.x / TILE_WIDTH);
	int right_position = (int)((m_position.x + m_texture->GetWidth()) / TILE_WIDTH);

	int lowerY_position = (int)(m_position.y + (m_texture->GetHeight() * 0.7)) / TILE_HEIGHT;
	
	//DOWN
	if (m_current_level_map->GetTileAt(foot_position, centralX_position) != 1 
		&& m_current_level_map->GetTileAt(foot_position, centralX_position) != 4 
		&& m_current_level_map->GetTileAt(foot_position, centralX_position) != 7) 
	{ AddGravity(deltaTime); }
	else { m_can_jump = true; if (check_knockback) { m_is_knockedBack = false; check_knockback = false; } }

	//DOWN CLIMBING
	if (m_current_level_map->GetTileAt(foot_position, centralX_position) == 4) { can_climb = true; m_jumping = false; }
	else { can_climb = false; }

	//DOWN WATER LAVA
	if (m_current_level_map->GetTileAt(foot_position, centralX_position) == 2 && !m_is_invulnerable) { Knockback(0); TakeDamage(); m_is_invulnerable = true; }
	else if (m_current_level_map->GetTileAt(foot_position, centralX_position) == 3 && !m_is_invulnerable) { Knockback(0); TakeDamage(); m_is_invulnerable = true; }

	//UP
	if (m_current_level_map->GetTileAt(top_position, centralX_position) == 1) { CancelJump(); }

	//LEFT 
	if (m_current_level_map->GetTileAt(centralY_position, left_position) == 1) { can_move_left = false; }
	else if (m_current_level_map->GetTileAt(lowerY_position, left_position) == 1) { can_move_left = false; }
	else { can_move_left = true; }
	
	//RIGHT
	if (m_current_level_map->GetTileAt(centralY_position, right_position) == 1) { can_move_right = false; }
	else if(m_current_level_map->GetTileAt(lowerY_position, right_position) == 1) { can_move_right = false; }
	else { can_move_right = true; }

}

void Character::Climb(float deltaTime)
{
	m_position.y -= deltaTime * CLIMB_SPEED;
}

void Character::MoveLeft(float deltaTime)
{
	m_position.x -= deltaTime * m_movement_speed;
	m_facing_direction = FACING_LEFT;
}

void Character::MoveRight(float deltaTime)
{
	m_position.x += deltaTime * m_movement_speed;
	m_facing_direction = FACING_RIGHT;
}

void Character::AddGravity(float deltaTime)
{
	
	if (m_position.y > SCREEN_HEIGHT - m_texture->GetHeight())
	{
		m_can_jump = true; 
	}
	else
	{
		m_position.y += deltaTime * GRAVITYSTRENGTH;
	}
}

void Character::SetMovingAndJump(float deltaTime)
{
	//deal with jumping first
	if (m_jumping)
	{
		//adjust position
		m_position.y -= m_jump_force * deltaTime;

		//reduce jump force
		m_jump_force -= JUMP_FORCE_DECREMENT * deltaTime;

		//is jump force 0?
		if (m_jump_force <= 0.0f)
		{
			m_jumping = false;
			//m_is_knockedBack = false;
		}
			
	}

	if (m_moving_left && can_move_left)
	{
		MoveLeft(deltaTime);
	}
	else if (m_moving_right && can_move_right)
	{
		MoveRight(deltaTime);
	}

	//Knockback and invulnerability
	if (m_is_knockedBack) 
	{ 
		m_position.x += m_knockback_force * knockback_directon * deltaTime;
		if(!check_knockback) 
		{
			knockback_offset -= deltaTime;
			if (knockback_offset <= 0.0f)
			{
				knockback_offset = 0.2;
				check_knockback = true;
			}
		}
	}

	if (m_is_invulnerable)
	{
		m_invulnerable_timer -= deltaTime;
		if (m_invulnerable_timer <= 0.0f)
		{
			m_invulnerable_timer = INVULNERABLE_LENGTH;
			m_is_invulnerable = false; 
		}
	}

}

void Character::Jump(int forceAmount)
{
	if (!m_jumping)
	{
		m_can_jump = false;
		m_jump_force = forceAmount; m_jumping = true;
	}
	
}

