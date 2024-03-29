#include "CharacterKoopa.h"

CharacterKoopa::CharacterKoopa(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position, LevelMap* map, FACING start_facing, float movement_speed) : Character(renderer, imagePath, start_position, map, start_facing, movement_speed)
{
	m_facing_direction = start_facing;
	m_movement_speed = movement_speed;
	m_position = start_position;
	m_injured = false;
	m_alive = true; 
	m_single_sprite_w = m_texture->GetWidth() / 2;
	m_single_sprite_h = m_texture->GetHeight();
	m_current_level_map = map;

	m_source_rect = { 0, 0, m_texture->GetWidth(), m_texture->GetHeight() };
	m_draw_rect = { 0, 0, m_texture->GetWidth(), m_texture->GetHeight() };
}

CharacterKoopa::~CharacterKoopa()
{
}

void CharacterKoopa::Update(float deltaTime, SDL_Event e)
{
	Collisions(deltaTime);
	
	if (!m_injured)
	{
		//enemy is not injured so move
		if (level_loaded)
		{
			if (m_facing_direction == FACING_LEFT)
			{
				m_moving_left = true;
				m_moving_right = false;
			}
			else if (m_facing_direction == FACING_RIGHT)
			{
				m_moving_right = true;
				m_moving_left = false;
			}
		}
		
	}
	else
	{
		//we should not be moving when injured
		m_moving_right = false;
		m_moving_left = false;

		//count down the injured time
		m_injured_time -= deltaTime;

		if (m_injured_time <= 0.0)
			FlipRightWayUp();

	}

	if (m_position.x < 0 || can_move_left == false)
	{
		m_facing_direction == FACING_RIGHT;
		m_moving_right = true;
		m_moving_left = false;

		can_move_left = true;
	}
	else if (m_position.x > SCREEN_WIDTH - 25 || can_move_right == false)
	{
		m_facing_direction == FACING_LEFT;
		m_moving_left = true;
		m_moving_right = false;

		can_move_right = true;
	}
	
	
	SetMovingAndJump(deltaTime); 

	
}

void CharacterKoopa::Render(SDL_Rect* camera_rect)
{
	//variable to hold the left position of the sprite we want to draw
	int left = 0.0f;

	//if injured move the left position to be the left position of the second image of the sprite sheet
	if (m_injured)
		left = m_single_sprite_w;


	//get the portion of the sprite sheet you want to draw
	//							   {xPos, yPos, width of sprite, height of sprite}
	SDL_Rect portion_of_sprite = { left,0,m_single_sprite_w, m_single_sprite_h };

	//determine where you want it drawn
	SDL_Rect destRect = { (int)(m_position.x - camera_rect->x), (int)(m_position.y - camera_rect->y), m_single_sprite_w, m_single_sprite_h };
	//SDL_Rect destRect = { (int)(camera_rect->x), (int)(camera_rect->y), m_single_sprite_w, m_single_sprite_h };
	//then draw it facing the correct direction
	if (m_facing_direction == FACING_RIGHT)
	{
		m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_NONE);
	}
	else
	{
		m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_HORIZONTAL);
	}

}

void CharacterKoopa::Collisions(float deltaTime)
{
	std::cout << m_facing_direction << std::endl;
	//collision position variables
	int centralX_position = (int)(m_position.x + (m_texture->GetWidth() * 0.5)) / TILE_WIDTH;
	int centralY_position = (int)(m_position.y + (m_texture->GetHeight() * 0.5)) / TILE_HEIGHT;

	int foot_position = (int)(m_position.y + m_texture->GetHeight()) / TILE_HEIGHT;
	int top_position = (int)(m_position.y) / TILE_HEIGHT;

	int left_position = (int)(m_position.x / TILE_WIDTH);
	int right_position = (int)((m_position.x + m_texture->GetWidth()) / TILE_WIDTH);

	int lowerY_position = (int)(m_position.y + (m_texture->GetHeight() * 0.7)) / TILE_HEIGHT;

	//DOWN
	if (m_current_level_map->GetTileAt(foot_position, centralX_position) != 1) { AddGravity(deltaTime); }

	if (m_current_level_map->GetTileAt(foot_position, centralX_position) == 2) { Knockback(0); }
	else if (m_current_level_map->GetTileAt(foot_position, centralX_position) == 3) { Knockback(0); }

	//UP
	if (m_current_level_map->GetTileAt(top_position, centralX_position) == 1) { CancelJump(); }

	//LEFT 
	if (m_current_level_map->GetTileAt(centralY_position, left_position) == 1 && m_facing_direction == FACING_LEFT) { can_move_left = false; }
	//else if (m_current_level_map->GetTileAt(lowerY_position, left_position) == 1 && m_facing_direction == FACING_RIGHT) { can_move_left = false; }
	else { can_move_left = true; }

	//RIGHT
	if (m_current_level_map->GetTileAt(centralY_position, right_position) == 1 && m_facing_direction == FACING_RIGHT) { can_move_right = false; }
	//else if (m_current_level_map->GetTileAt(lowerY_position, right_position) == 1 && m_facing_direction == FACING_LEFT) { can_move_right = false; }
	else { can_move_right = true; }
}

void CharacterKoopa::TakeDamage()
{
	m_injured = true; 
	m_injured_time = INJURED_TIME; 
	Jump(); 
}

void CharacterKoopa::Jump()
{
	if (!m_jumping)
	{
		m_jump_force = INITIAL_JUMP_FORCE;
		m_jumping = true;
		m_can_jump = false;
	}

}

void CharacterKoopa::FlipRightWayUp()
{
	if (m_facing_direction == FACING_LEFT) { m_facing_direction = FACING_RIGHT; }
	else { m_facing_direction = FACING_LEFT; }

	m_injured = false; 
	Jump();
}
