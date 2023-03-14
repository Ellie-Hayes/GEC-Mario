#include "Character.h"

Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map)
{
	m_renderer = renderer; m_position = start_position;
	m_facing_direction = FACING_RIGHT; 
	m_moving_left = false;
	m_moving_right = false; 

	m_collision_radius = 15.0f;

	m_texture = new Texture2D(m_renderer);
	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load texture!" << std::endl;
	}

	m_current_level_map = map; 
}

Character::~Character()
{
	m_renderer = nullptr; 
}

void Character::Render()
{
	if (m_facing_direction == FACING_RIGHT)
	{
		m_texture->Render(m_position, SDL_FLIP_NONE);
	}
	else
	{
		m_texture->Render(m_position, SDL_FLIP_HORIZONTAL);
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

void Character::MoveLeft(float deltaTime)
{
	m_position.x -= deltaTime * MOVEMENTSPEED;
	m_facing_direction = FACING_LEFT;
}

void Character::MoveRight(float deltaTime)
{
	m_position.x += deltaTime * MOVEMENTSPEED;	
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

void Character::Jump()
{
	if (!m_jumping)
	{
		m_can_jump = false;
		m_jump_force = INITIAL_JUMP_FORCE; m_jumping = true;
	}
	
}

