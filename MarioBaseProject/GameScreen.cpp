#include "GameScreen.h"

GameScreen::GameScreen(SDL_Renderer* renderer)
{
	m_renderer = renderer; 
}

GameScreen::~GameScreen()
{
	m_renderer = nullptr; 

	delete background;
	background = nullptr;
	delete camera;
	camera = nullptr;

	m_tiles.clear();
	m_decoTiles.clear();

}

void GameScreen::Render()
{
}

void GameScreen::Update(float deltaTime, SDL_Event e)
{

}

void GameScreen::UpdatePowBlock()
{
	if (Collisions::Instance()->Box(mario->GetCollisionBox(), m_pow_block->GetCollisionsBox()) && m_pow_block->IsAvailable())
	{
		//collided while jumping
		if (mario->IsJumping())
		{
			DoScreenShake();
			background->SetScreenShake();
			m_pow_block->TakeHit();
			mario->CancelJump();
		}
	}

	if (Collisions::Instance()->Box(luigi->GetCollisionBox(), m_pow_block->GetCollisionsBox()) && m_pow_block->IsAvailable())
	{
		//collided while jumping
		if (mario->IsJumping())
		{
			DoScreenShake();
			background->SetScreenShake();
			m_pow_block->TakeHit();
			mario->CancelJump();
		}
	}
}

void GameScreen::DoScreenShake()
{
	for (int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->TakeDamage();
	}

}

void GameScreen::UpdateEnemies(float deltaTime, SDL_Event e)
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
			//POWBLOCK ENEMY KILL
			if (Collisions::Instance()->Circle(m_enemies[i], mario))
			{
				if (m_enemies[i]->GetInjured())
				{
					m_enemies[i]->SetAlive(false);
				}
			}

			//MARIO DAMAGE
			if (Collisions::Instance()->Box(mario->GetCollisionBox(), m_enemies[i]->GetCollisionBox()))
			{
				if (mario->GetPosition().y <= m_enemies[i]->GetPosition().y - 24)
				{
					m_enemies[i]->SetAlive(false);
				}
				else if (!mario->IsInvulnerable())
				{
					marioHealth->SetHealth(mario->TakeDamage());
					if (mario->GetPosition().x >= m_enemies[i]->GetPosition().x) { mario->Knockback(-1); }
					else { mario->Knockback(1); }

				}
			}

			//LUIGI DAMAGE
			if (Collisions::Instance()->Box(luigi->GetCollisionBox(), m_enemies[i]->GetCollisionBox()))
			{
				if (luigi->GetPosition().y <= m_enemies[i]->GetPosition().y - 18)
				{
					m_enemies[i]->SetAlive(false);
				}
				else if (!luigi->IsInvulnerable())
				{
					luigiHealth->SetHealth(luigi->TakeDamage());
					luigi->Knockback(1);
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

void GameScreen::CreateKoopa(Vector2D position, FACING direction, float speed)
{
	std::cout << "Made koopa" << std::endl;
	CharacterKoopa* koopa = new CharacterKoopa(m_renderer, "Images/Koopa.png", position, m_level_map, direction, speed);
	m_enemies.push_back(koopa);
}

void GameScreen::UpdateCoins(float deltaTime, SDL_Event e)
{
	int coinIndexToDelete = -1;

	for (int i = 0; i < m_coins.size(); i++)
	{
		m_coins[i]->Update(deltaTime, e);

		if (Collisions::Instance()->Circle(m_coins[i], mario))
		{
			mario_score += 15;
			m_coins[i]->SetAlive(false);

			if (m_mario_text != nullptr) { m_mario_text->LoadFont("Fonts/Pacifico.ttf", 40, scoreMessage + std::to_string(mario_score), { 255, 255, 255 }); }
		}

		if (Collisions::Instance()->Circle(m_coins[i], luigi))
		{
			luigi_score += 15;
			m_coins[i]->SetAlive(false);

			if (m_luigi_text != nullptr) { m_luigi_text->LoadFont("Fonts/Pacifico.ttf", 40, scoreMessage + std::to_string(luigi_score), { 255, 255, 255 }); }
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

void GameScreen::CreateCoin(Vector2D position)
{
	int xPos = position.x * TILE_WIDTH;
	int yPos = position.y * TILE_HEIGHT;

	std::cout << "Made coin" << std::endl;
	CharacterCoin* coin = new CharacterCoin(m_renderer, "Images/Coin.png", Vector2D(xPos, yPos), m_level_map, FACING_LEFT, 0);
	m_coins.push_back(coin);
}

void GameScreen::Set8BitTileNeighbours(std::vector<Vector2D>& passedVector, int typeCheck)
{
	direction2D = new Direction2D();

	for (int i = 0; i < passedVector.size(); i++)
	{
		std::string neighbourBinaryPositions = "";
		for (Vector2D TilePos : direction2D->eightDirectionList) {
			int neighbourXpos = passedVector[i].x + TilePos.x;
			int neighbourYpos = passedVector[i].y + TilePos.y;

			if (neighbourXpos < 0 || neighbourXpos > MAP_WIDTH || neighbourYpos < 0 || neighbourYpos > MAP_HEIGHT - 1) { neighbourBinaryPositions += '0'; }
			else
			{
				if (m_level_map->m_map[neighbourYpos][neighbourXpos] == typeCheck) { neighbourBinaryPositions += '1'; }
				else { neighbourBinaryPositions += '0'; }
			}
		}

		CreateTiles(Vector2D(passedVector[i].x, passedVector[i].y), neighbourBinaryPositions);

	}

	delete direction2D;
}

void GameScreen::Set4BitTileNeighbours(std::vector<Vector2D>& passedVector, int typeCheck)
{
	direction2D = new Direction2D();

	for (int i = 0; i < passedVector.size(); i++)
	{
		std::string neighbourBinaryPositions = "";
		for (Vector2D TilePos : direction2D->cardinalDirectionList) {
			int neighbourXpos = passedVector[i].x + TilePos.x;
			int neighbourYpos = passedVector[i].y + TilePos.y;

			if (neighbourXpos < 0 || neighbourXpos > MAP_WIDTH || neighbourYpos < 0 || neighbourYpos > MAP_HEIGHT - 1) { neighbourBinaryPositions += '0'; }
			else
			{
				if (m_level_map->m_map[neighbourYpos][neighbourXpos] == typeCheck) { neighbourBinaryPositions += '1'; }
				else { neighbourBinaryPositions += '0'; }
			}
		}

		switch (typeCheck)
		{
		case 2:
			CreateWaterLavaTiles(Vector2D(passedVector[i].x, passedVector[i].y), neighbourBinaryPositions, "Tiles/Water/");
			break;
		case 3:
			CreateWaterLavaTiles(Vector2D(passedVector[i].x, passedVector[i].y), neighbourBinaryPositions, "Tiles/Lava/");
			break;
		default:
			break;
		}

	}

	delete direction2D;
}

void GameScreen::CreateTiles(Vector2D position, std::string binaryType)
{
	WallBinaryTypes* wallBinaryTileTypes = new WallBinaryTypes();
	bool typeChosen = false;
	stringstream s;

	//Main 
	for (std::string type : wallBinaryTileTypes->wallTop) {
		if (binaryType == type) {
			s << levelPalette << "WallTop.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;

			int randomChance = rand() % 101;
			if (randomChance > 80)
			{
				Vector2D paintPosition = Vector2D(position.x, position.y - 1);
				PaintTile(paintPosition, ("Tiles/Grass/Grass.png"));
			}

			delete wallBinaryTileTypes;
			return;
		}
	}
	for (std::string type : wallBinaryTileTypes->wallRight) {
		if (binaryType == type) {
			s << levelPalette << "WallRight.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;

			delete wallBinaryTileTypes;
			return;
		}
	}
	for (std::string type : wallBinaryTileTypes->wallBottom) {
		if (binaryType == type) {
			s << levelPalette << "WallBottom.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;

			int randomChance = rand() % 101;
			if (randomChance > 80)
			{
				Vector2D paintPosition = Vector2D(position.x, position.y + 1);
				int vineType = rand() % 3;

				switch (vineType)
				{
				case 1:
					PaintTile(paintPosition, ("Tiles/Grass/Vine1.png"));
					break;
				case 2:
					PaintTile(paintPosition, ("Tiles/Grass/Vine2.png"));
					break;
				default:
					break;
				}

			}

			delete wallBinaryTileTypes;
			return;
		}
	}
	for (std::string type : wallBinaryTileTypes->wallLeft) {
		if (binaryType == type) {
			s << levelPalette << "WallLeft.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;

			delete wallBinaryTileTypes;
			return;
		}
	}
	for (std::string type : wallBinaryTileTypes->wallFull) {
		if (binaryType == type) {
			s << levelPalette << "WallFull.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;

			delete wallBinaryTileTypes;
			return;
		}
	}


	//Corner
	for (std::string type : wallBinaryTileTypes->wallCornerLeftTop) {
		if (binaryType == type) {
			s << levelPalette << "WallCornerBottomRight.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;

			delete wallBinaryTileTypes;
			return;
		}
	}
	for (std::string type : wallBinaryTileTypes->wallCornerRightTop) {
		if (binaryType == type) {
			s << levelPalette << "WallCornerBottomLeft.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;

			delete wallBinaryTileTypes;
			return;
		}
	}
	for (std::string type : wallBinaryTileTypes->wallCornerLeftBottom) {
		if (binaryType == type) {
			s << levelPalette << "WallCornerLeftBottom.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;

			delete wallBinaryTileTypes;
			return;
		}
	}
	for (std::string type : wallBinaryTileTypes->wallCornerRightBottom) {
		if (binaryType == type) {
			s << levelPalette << "WallCornerRightBottom.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;

			delete wallBinaryTileTypes;
			return;
		}
	}


	//Diagonals
	for (std::string type : wallBinaryTileTypes->wallDiagonalLeftBottom) {
		if (binaryType == type) {
			s << levelPalette << "WallDiagonalCornerBottomLeft.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;

			delete wallBinaryTileTypes;
			return;
		}
	}
	for (std::string type : wallBinaryTileTypes->wallDiagonalLeftTop) {
		if (binaryType == type) {
			s << levelPalette << "WallDiagonalCornerTopLeft.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;

			delete wallBinaryTileTypes;
			return;
		}
	}
	for (std::string type : wallBinaryTileTypes->wallDiagonalRightBottom) {
		if (binaryType == type) {
			s << levelPalette << "WallDiagonalCornerBottomRight.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;

			delete wallBinaryTileTypes;
			return;
		}
	}
	for (std::string type : wallBinaryTileTypes->wallDiagonalRightTop) {
		if (binaryType == type) {
			s << levelPalette << "WallDiagonalCornerTopRight.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;

			delete wallBinaryTileTypes;
			return;
		}
	}


	if (!typeChosen) { std::cout << "Tile at (" << position.x << ", " << position.y << ") has type: " << binaryType << std::endl;  PaintTile(position, "Tiles/Test/ErrorTile.png"); }

}

void GameScreen::CreateWaterLavaTiles(Vector2D position, std::string binaryType, std::string palette)
{
	FourBitBinaryTypes* fourBitTypes = new FourBitBinaryTypes();
	bool typeChosen = false;
	stringstream s;


	for (std::string type : fourBitTypes->BitStart) {
		if (binaryType == type) {
			s << palette << "Start.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;
		}
	}

	for (std::string type : fourBitTypes->BitMiddle) {
		if (binaryType == type) {
			s << palette << "Middle.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;
		}
	}

	for (std::string type : fourBitTypes->BitTop) {
		if (binaryType == type) {
			s << palette << "Top.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;
		}
	}

	for (std::string type : fourBitTypes->BitFull) {
		if (binaryType == type) {
			s << palette << "Full.png";
			PaintTile(position, s.str().c_str()); typeChosen = true;
		}
	}

	if (!typeChosen) { std::cout << "Tile at (" << position.x << ", " << position.y << ") has type: " << binaryType << std::endl;  PaintTile(position, "Tiles/Test/ErrorTile.png"); }

	delete fourBitTypes;
}

void GameScreen::PaintTile(Vector2D position, std::string filePath)
{

	int xPos = position.x * TILE_WIDTH;
	int yPos = position.y * TILE_HEIGHT;

	Tile* tile = new Tile(m_renderer, filePath, Vector2D(xPos, yPos));
	m_tiles.push_back(tile);
} //Above player

void GameScreen::PaintDecoTile(Vector2D position, std::string filePath)
{

	int xPos = position.x * TILE_WIDTH;
	int yPos = position.y * TILE_HEIGHT;

	Tile* tile = new Tile(m_renderer, filePath, Vector2D(xPos, yPos));
	m_decoTiles.push_back(tile);
}
