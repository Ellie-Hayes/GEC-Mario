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
	delete marioHealth;
	marioHealth = nullptr; 
	delete luigiHealth;
	luigiHealth = nullptr; 
	delete m_mario_text;
	m_mario_text = nullptr;
	delete m_luigi_text; 
	m_luigi_text = nullptr;

	m_enemies.clear();
	m_coins.clear(); 
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

	m_mario_text->Render(250, 80 );
	m_luigi_text->Render(1650, 50);

	
}

void GameScreen1::Update(float deltaTime, SDL_Event e)
{
	

	new_enemy_timer -= deltaTime;
	if (new_enemy_timer <= 0)
	{
		//CreateKoopa(Vector2D(80, 100), FACING_RIGHT, KOOPA_SPEED);
		mario->SetLevelLoaded(true);
		new_enemy_timer = 5000; 
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

	if (mario->GetAlive()) { camera->x = mario->GetPosition().x - SCREEN_WIDTH / 2; }
	else if (luigi->GetAlive()) { camera->x = luigi->GetPosition().x - SCREEN_WIDTH / 2; }
	
	if (camera->x < 0) { camera->x = 0; }
	else if (camera->x > LEVEL_WIDTH - camera->w) { camera->x = LEVEL_WIDTH - camera->w; }

}

bool GameScreen1::SetUpLevel()
{
	levelPalette = "Tiles/Grass/";
	srand(time(NULL));
	SetLevelMap();

	camera = new SDL_Rect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	background = new Background(m_renderer, "Images/BackgroundMB.png", Vector2D(0, 0));
	mario = new CharacterMario(m_renderer, "Images/Characters/PlayerTest.png", Vector2D(80, 400), m_level_map, FACING_RIGHT, MOVEMENTSPEED);
	luigi = new CharacterLuigi(m_renderer, "Images/Characters/CatTest.png", Vector2D(60, 400), m_level_map, FACING_LEFT, MOVEMENTSPEED);
	
	//CreateKoopa(Vector2D(200, 100), FACING_RIGHT, KOOPA_SPEED);
	CreateKoopa(Vector2D(400, 200), FACING_LEFT, KOOPA_SPEED);

	m_pow_block = new PowBlock(m_renderer, m_level_map);
	new_enemy_timer = 10;
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

	inFile.open("Levels/Level2.txt");

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
			case 7:
				PaintTile(Vector2D(j, i), "Tiles/Misc/PlatMiddle.png");
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

	//luigi->SetLevelLoaded(true);
}



