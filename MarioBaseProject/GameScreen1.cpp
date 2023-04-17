#include "GameScreen1.h"
#include <iostream>
#include "Texture2D.h"


GameScreen1::GameScreen1(SDL_Renderer* renderer): GameScreen(renderer)
{
	SetUpLevel();
}

GameScreen1::~GameScreen1()
{
	delete mario;
	mario = nullptr;
	delete luigi; 
	luigi = nullptr;
	delete m_pow_block;
	m_pow_block = nullptr;

	m_enemies.clear();

}

void GameScreen1::Render()
{
	background->Render(camera);

	for (int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->Render(camera);
	}

	for (int i = 0; i < m_coins.size(); i++)
	{
		m_coins[i]->Render(camera);
	}

	mario->Render(camera);
	luigi->Render(camera);
	m_pow_block->Render(camera);
	m_text->Render(10, 10);
}

void GameScreen1::Update(float deltaTime, SDL_Event e)
{
	if (m_screenshake)
	{
		m_shake_time -= deltaTime;
		m_wobble++;
		m_background_yPos = sin(m_wobble);
		m_background_yPos *= 3.0f;

		//end shake after duration
		if (m_shake_time <= 0.0f)
		{
			m_shake_time = false;
			m_background_yPos = 0.0f;
		}
	}

	new_enemy_timer -= deltaTime;
	if (new_enemy_timer <= 0)
	{
		CreateKoopa(Vector2D(80, 100), FACING_RIGHT, KOOPA_SPEED);
		new_enemy_timer = 5; 
	}

	mario->Update(deltaTime, e);
	luigi->Update(deltaTime, e);

	UpdatePowBlock();
	UpdateEnemies(deltaTime, e);
	UpdateCoins(deltaTime, e);
	
	if (Collisions::Instance()->Circle(mario, luigi))
	{
		//std::cout << "Circle hit!" << std::endl;
	}

	//for (int i = 0; i < MAP_HEIGHT; i++)
	//{
	//	for (int j = 0; j < MAP_WIDTH; j++)
	//	{
	//		//if (Collisions::Instance()->Box(mario->GetCollisionBox(), ))
	//		//{
	//		//	//std::cout << "Box hit!" << std::endl;
	//		//}
	//	}
	//}

	if (Collisions::Instance()->Box(mario->GetCollisionBox(), luigi->GetCollisionBox()))
	{
		//std::cout << "Box hit!" << std::endl;
	}
	
	camera->x = mario->GetPosition().x - SCREEN_WIDTH / 2;
	//camera -> x = 100; 
	if (camera->x < 0) { camera->x = 0; }
	else if (camera->x > LEVEL_WIDTH - camera->w) { camera->x = LEVEL_WIDTH - camera->w; }

	if (m_text != nullptr && score != old_score)
	{
		old_score = score;
		m_text->LoadFont("Fonts/Pacifico.ttf", 40, scoreMessage + std::to_string(score), { 255, 255, 255 });
	}


}

void GameScreen1::UpdatePowBlock()
{
	if (Collisions::Instance()->Box(mario->GetCollisionBox(), m_pow_block->GetCollisionsBox()) && m_pow_block->IsAvailable())
	{
		//collided while jumping
		if (mario->IsJumping())
		{
			DoScreenShake();
			m_pow_block->TakeHit();
			mario->CancelJump();
		}

	}
}

bool GameScreen1::SetUpLevel()
{
	SetLevelMap();

	camera = new SDL_Rect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	background = new Background(m_renderer, "Images/BackgroundMB.png", Vector2D(0, m_background_yPos));
	mario = new CharacterMario(m_renderer, "Images/Mario.png", Vector2D(80, 100), m_level_map, FACING_RIGHT, MOVEMENTSPEED);
	luigi = new CharacterLuigi(m_renderer, "Images/Luigi.png", Vector2D(60, 100), m_level_map, FACING_LEFT, MOVEMENTSPEED);
	
	CreateKoopa(Vector2D(80, 100), FACING_RIGHT, KOOPA_SPEED);
	CreateKoopa(Vector2D(100, 200), FACING_LEFT, KOOPA_SPEED);

	CreateCoin(Vector2D(150, 300));
	CreateCoin(Vector2D(200, 300));
	CreateCoin(Vector2D(250, 300));

	m_pow_block = new PowBlock(m_renderer, m_level_map);
	m_screenshake = false; 
	m_background_yPos = 0.0f; 
	new_enemy_timer = 5;
	score = 0; 

	m_text = new TextRenderer(m_renderer);

	if (!m_text->LoadFont("Fonts/Pacifico.ttf", 40, scoreMessage + std::to_string(score), { 255, 255, 255 }))
	{
		std::cout << "Failed to load background texture!" << std::endl;
		return false;
	}
	return false;
}

void GameScreen1::SetLevelMap()
{
	int map[MAP_HEIGHT][MAP_WIDTH] = { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{ 1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{ 0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
										{ 1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{ 1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} };


	/*ifstream inFile;

	inFile.open("Levels/Level1.txt");

	if (!inFile.good())
	{
		cerr << "Can't open text file " << endl;
	}

	int map[MAP_HEIGHT][MAP_WIDTH];
	int tempNum;

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			inFile >> tempNum;
			map[i][j] = tempNum;
		}
	}

	inFile.close();*/

	//clear any old maps
	if (m_level_map != nullptr)
	{
		delete m_level_map;
	}

	//set the new one
	m_level_map = new LevelMap(map);

}

void GameScreen1::DoScreenShake()
{
	m_screenshake = true;
	m_shake_time = SHAKE_DURATION;
	m_wobble = 0.0f;

	for (int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->TakeDamage();
	}

}

void GameScreen1::UpdateEnemies(float deltaTime, SDL_Event e)
{
	int enemyIndexToDelete = -1;

	for (int i = 0; i < m_enemies.size(); i++)
	{
		
		//check if the enemy is on the bottom row of tiles
		if (m_enemies[i]->GetPosition().y > 300)
		{
			//is the enemy off screen to the left / right?
			if (m_enemies[i]->GetPosition().x < (float)(-m_enemies[i]->GetCollisionBox().width * 0.5f) || m_enemies[
				i]->GetPosition().x > SCREEN_WIDTH + (float)(m_enemies[i]->GetCollisionBox().width * 0.55f))
				m_enemies[i]->SetAlive(false);
		}

		//now do the update
		m_enemies[i]->Update(deltaTime, e);

		//check to see if enemy collides with player
		if ((m_enemies[i]->GetPosition().y > 300.0f || m_enemies[i]->GetPosition().y <= 64.0f) && (m_enemies[i]->
			GetPosition().x < 64.0f || m_enemies[i]->GetPosition().x > SCREEN_WIDTH - 96.0f))
		{
			//ignore collisions if behind pipe
		}
		else
		{
			if (Collisions::Instance()->Circle(m_enemies[i], mario))
			{
				if (m_enemies[i]->GetInjured())
				{
					m_enemies[i]->SetAlive(false);
				}
				else
				{
					//kill mario
				}

			}
		}

		//if the enemy is no longer alive then schedule it for deletion
		if (!m_enemies[i]->GetAlive())
		{
			enemyIndexToDelete = i;
		}
	}

	//remove dead enemies -1 each update

	if (enemyIndexToDelete != -1)
	{
		std::cout << "delete" << std::endl; 
		m_enemies.erase(m_enemies.begin() + enemyIndexToDelete);
	}
}



void GameScreen1::CreateKoopa(Vector2D position, FACING direction, float speed)
{
	std::cout << "Made koopa" << std::endl;
	CharacterKoopa* koopa = new CharacterKoopa(m_renderer, "Images/Koopa.png", position, m_level_map, direction, speed);
	m_enemies.push_back(koopa);
}

void GameScreen1::UpdateCoins(float deltaTime, SDL_Event e)
{
	int coinIndexToDelete = -1;

	for (int i = 0; i < m_coins.size(); i++)
	{
		m_coins[i]->Update(deltaTime, e);

		if (Collisions::Instance()->Circle(m_coins[i], mario))
		{
			score += 15; 
			m_coins[i]->SetAlive(false);
		}

		//if the coin is collected then schedule it for deletion
		if (!m_coins[i]->GetAlive())
		{
			coinIndexToDelete = i;
		}
	}

	if (coinIndexToDelete != -1)
	{
		std::cout << "delete" << std::endl;
		m_coins.erase(m_coins.begin() + coinIndexToDelete);
	}
}

void GameScreen1::CreateCoin(Vector2D position)
{
	std::cout << "Made coin" << std::endl;
	CharacterCoin* coin = new CharacterCoin(m_renderer, "Images/Coin.png", position, m_level_map, FACING_LEFT, 0);
	m_coins.push_back(coin);
}
