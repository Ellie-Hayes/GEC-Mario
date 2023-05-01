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

	for (int i = 0; i < m_decoTiles.size(); i++) { m_decoTiles[i]->Render(camera); }
	for (int i = 0; i < m_enemies.size(); i++) { m_enemies[i]->Render(camera); }
	for (int i = 0; i < m_coins.size(); i++) { m_coins[i]->Render(camera); }

	if (mario->GetAlive()) { mario->Render(camera); }
	if (luigi->GetAlive()) { luigi->Render(camera); }
	m_pow_block->Render(camera);

	for (int i = 0; i < m_tiles.size(); i++) { m_tiles[i]->Render(camera); }

	marioHealth->Render(); 
	luigiHealth->Render();

	m_mario_text->Render(250, 40 );
	m_luigi_text->Render(1000, 30);
}

void GameScreen1::Update(float deltaTime, SDL_Event e)
{
	new_enemy_timer -= deltaTime;
	if (new_enemy_timer <= 0)
	{
		//CreateKoopa(Vector2D(80, 100), FACING_RIGHT, KOOPA_SPEED);
		new_enemy_timer = 5; 
	}

	background->Update(deltaTime);
	if (mario->GetAlive()) { mario->Update(deltaTime, e); }
	if (luigi->GetAlive()) { luigi->Update(deltaTime, e); }

	UpdatePowBlock();
	UpdateEnemies(deltaTime, e);
	UpdateCoins(deltaTime, e);

	if (mario->GetHealth() != marioHealth->GetHealth())
	{
		marioHealth->SetHealth(mario->GetHealth());
	}

	if (luigi->GetHealth() != luigiHealth->GetHealth())
	{
		luigiHealth->SetHealth(luigi->GetHealth());
	}
	

	for (int i = 0; i < m_enemies.size(); i++)
	{
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

	if (mario->GetAlive()) { camera->x = mario->GetPosition().x - SCREEN_WIDTH / 2; }
	else if (luigi->GetAlive()) { camera->x = luigi->GetPosition().x - SCREEN_WIDTH / 2; }
	
	if (camera->x < 0) { camera->x = 0; }
	else if (camera->x > LEVEL_WIDTH - camera->w) { camera->x = LEVEL_WIDTH - camera->w; }

}

void GameScreen1::UpdatePowBlock()
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
}

bool GameScreen1::SetUpLevel()
{
	levelPalette = "Tiles/Grass/";
	srand(time(NULL));
	SetLevelMap();

	camera = new SDL_Rect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	background = new Background(m_renderer, "Images/BackgroundMB.png", Vector2D(0, 0));
	mario = new CharacterMario(m_renderer, "Images/Characters/PlayerTest.png", Vector2D(80, 500), m_level_map, FACING_RIGHT, MOVEMENTSPEED);
	luigi = new CharacterLuigi(m_renderer, "Images/Characters/CatTest.png", Vector2D(60, 500), m_level_map, FACING_LEFT, MOVEMENTSPEED);
	
	
	//CreateKoopa(Vector2D(80, 100), FACING_RIGHT, KOOPA_SPEED);
	//CreateKoopa(Vector2D(100, 200), FACING_LEFT, KOOPA_SPEED);

	m_pow_block = new PowBlock(m_renderer, m_level_map);
	new_enemy_timer = 5;
	mario_score = 0; 
	luigi_score = 0;

	m_mario_text = new TextRenderer(m_renderer);
	m_luigi_text = new TextRenderer(m_renderer);

	if (!m_mario_text->LoadFont("Fonts/Pacifico.ttf", 40, scoreMessage + std::to_string(mario_score), { 255, 255, 255 }))
	{
		std::cout << "Failed to load font texture!" << std::endl;
		return false;
	}

	if (!m_luigi_text->LoadFont("Fonts/Pacifico.ttf", 40, scoreMessage + std::to_string(luigi_score), { 255, 255, 255 }))
	{
		std::cout << "Failed to load font texture!" << std::endl;
		return false;
	}

	marioHealth = new UIHealth(m_renderer, "Images/Characters/UIPlayer.png", Vector2D(10, 10));
	luigiHealth = new UIHealth(m_renderer, "Images/Characters/UICat.png", Vector2D(SCREEN_WIDTH - 402, 10));
	return false;
}

void GameScreen1::SetLevelMap()
{
	ifstream inFile;

	inFile.open("Levels/Level1.txt");

	if (!inFile.good())
	{
		cerr << "Can't open text file " << endl;
	}

	int map[MAP_HEIGHT][MAP_WIDTH];
	int inNumber; 

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			inFile >> inNumber;
			map[i][j] = inNumber;
			
			if (inNumber == 0)
			{
				int randomChance = rand() % 201;
				if (randomChance > 199)
				{
					PaintDecoTile(Vector2D(j, i), ("Tiles/Misc/Torch.png"));
				}
			} //Randomly place torches

			switch (inNumber)
			{

			case 1:
				wallPositions.push_back(Vector2D(j, i));
				break;
			case 2:
				waterPositions.push_back(Vector2D(j, i));
				break;
			case 3:
				lavaPositions.push_back(Vector2D(j, i));
				break;
			case 4:
				PaintDecoTile(Vector2D(j, i), "Tiles/Misc/Ladder.png");
				break;
			case 5:
				PaintTile(Vector2D(j, i), "Tiles/Misc/PlatSupportL.png");
				break;
			case 6:
				PaintTile(Vector2D(j, i), "Tiles/Misc/PlatSupportR.png");
				break;
			case 7:
				PaintTile(Vector2D(j, i), "Tiles/Misc/PlatMiddle.png");
				break;
			case 8:
				PaintTile(Vector2D(j, i), "Tiles/Misc/Torch.png");
				break;
			case 9:
				CreateCoin(Vector2D(j, i));
				break;
			default:
				break;
			}
			
		}
	}

	inFile.close();
	



	//clear any old maps
	if (m_level_map != nullptr)
	{
		delete m_level_map;
	}

	//set the new one
	m_level_map = new LevelMap(map);

	//Set walls
	Set8BitTileNeighbours(wallPositions, 1);
	Set4BitTileNeighbours(waterPositions, 2);
	Set4BitTileNeighbours(lavaPositions, 3);
}

void GameScreen1::Set8BitTileNeighbours(std::vector<Vector2D>& passedVector, int typeCheck )
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

void GameScreen1::Set4BitTileNeighbours(std::vector<Vector2D>& passedVector, int typeCheck)
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

void GameScreen1::CreateTiles(Vector2D position, std::string binaryType)
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

void GameScreen1::CreateWaterLavaTiles(Vector2D position, std::string binaryType, std::string palette)
{
	FourBitBinaryTypes* fourBitTypes =  new FourBitBinaryTypes(); 
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

void GameScreen1::PaintTile(Vector2D position, std::string filePath)
{
	
	int xPos = position.x * TILE_WIDTH;
	int yPos = position.y * TILE_HEIGHT;

	Tile* tile = new Tile(m_renderer, filePath, Vector2D(xPos, yPos));
	m_tiles.push_back(tile);
} //Above player

void GameScreen1::PaintDecoTile(Vector2D position, std::string filePath)
{

	int xPos = position.x * TILE_WIDTH;
	int yPos = position.y * TILE_HEIGHT;

	Tile* tile = new Tile(m_renderer, filePath, Vector2D(xPos, yPos));
	m_decoTiles.push_back(tile);
} 


void GameScreen1::DoScreenShake()
{
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

void GameScreen1::CreateCoin(Vector2D position)
{
	int xPos = position.x * TILE_WIDTH;
	int yPos = position.y * TILE_HEIGHT;

	std::cout << "Made coin" << std::endl;
	CharacterCoin* coin = new CharacterCoin(m_renderer, "Images/Coin.png", Vector2D(xPos, yPos), m_level_map, FACING_LEFT, 0);
	m_coins.push_back(coin);
}


