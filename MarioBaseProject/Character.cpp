#include "Character.h"

Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map, FACING start_facing, float movement_speed)
{
	m_renderer = renderer; m_position = start_position;
	m_facing_direction = start_facing; 
	m_moving_left = false;
	m_moving_right = false; 

	m_collision_radius = 15.0f;

	m_texture = new Texture2D(m_renderer);
	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load texture!" << std::endl;
	}

	m_current_level_map = map; 

	m_source_rect = { 0, 0, m_texture->GetWidth(), m_texture->GetHeight() };
	m_draw_rect = { 0, 0, m_texture->GetWidth(), m_texture->GetHeight() };

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

void Character::Collisions(float deltaTime)
{
	//collision position variables
	int centralX_position = (int)(m_position.x + (m_texture->GetWidth() * 0.5)) / TILE_WIDTH;
	int centralY_position = (int)(m_position.y + (m_texture->GetHeight() * 0.5)) / TILE_HEIGHT;

	int foot_position = (int)(m_position.y + m_texture->GetHeight()) / TILE_HEIGHT;
	int top_position = (int)(m_position.y) / TILE_HEIGHT;

	int left_position = (int)(m_position.x + (m_texture->GetWidth() / LEVEL_WIDTH));

	if (m_current_level_map->GetTileAt(foot_position, centralX_position) == 0) // DOWN!!
	{
		AddGravity(deltaTime);
	}
	else
	{
		//collided with ground so we can jump again
		m_can_jump = true;
	}

	if (m_current_level_map->GetTileAt(top_position, centralX_position) == 1)
	{
		CancelJump();
	}

	if (m_current_level_map->GetTileAt(left_position, centralY_position) == 0) // DOWN!!
	{
		//cout << "left" << endl;
	}

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
		//m_position.y = SCREEN_HEIGHT - m_texture->GetHeight();
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
			m_jumping = false;
	}

	if (m_moving_left)
	{
		MoveLeft(deltaTime);
	}
	else if (m_moving_right)
	{
		MoveRight(deltaTime);
	}


}

void Character::Jump()
{
	if (!m_jumping)
	{
		m_can_jump = false;
		m_jump_force = INITIAL_JUMP_FORCE; m_jumping = true;
	}
	
}

