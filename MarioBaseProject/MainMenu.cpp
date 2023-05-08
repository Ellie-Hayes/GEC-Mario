#include "MainMenu.h"

MainMenu::MainMenu(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpLevel();
}

MainMenu::~MainMenu()
{
	delete background;
	background = nullptr; 

	m_tiles.clear();
	m_decoTiles.clear();
}

void MainMenu::Render()
{
	background->Render(camera);

	for (int i = 0; i < m_decoTiles.size(); i++) { m_decoTiles[i]->Render(camera); }
	for (int i = 0; i < m_tiles.size(); i++) { m_tiles[i]->Render(camera); }

	m_title_text->Render(250, 40);
	m_play_text->Render(1000, 30);
	m_loading_text->Render(500, 500);

	
}

void MainMenu::Update(float deltaTime, SDL_Event e)
{
	
	background->Update(deltaTime);

	if (camera->x < 0) { camera->x = 0; }
	else if (camera->x > LEVEL_WIDTH - camera->w) { camera->x = LEVEL_WIDTH - camera->w; }
	
	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_SPACE:
			level_complete = true;
			std::cout << "HI" << endl; 
			break;
		}

	}

	dot_timer -= 0.1;
	if (dot_timer <= 0)
	{
		dot_timer = 300.0f;
		stringstream loadingString;
		loadingString << " ";

		for (int i = 0; i < m_dots; i++)
		{
			loadingString << ". ";
		}
		m_loading_text->LoadFont("Fonts/Pacifico.ttf", 40, loadingString.str().c_str(), { 255, 255, 255 });

		m_dots += 1;
		if (m_dots > 3)
		{
			m_dots = 0;
		}
	}

}


bool MainMenu::SetUpLevel()
{
	levelPalette = "Tiles/Grass/";
	srand(time(NULL));
	SetLevelMap();

	camera = new SDL_Rect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	background = new Background(m_renderer, "Images/BackgroundMB.png", Vector2D(0, 0));

	m_title_text = new TextRenderer(m_renderer);
	m_play_text = new TextRenderer(m_renderer);
	m_loading_text = new TextRenderer(m_renderer);

	m_dots = 1; 
	dot_timer = 1.0f; 

	if (!m_title_text->LoadFont("Fonts/Pacifico.ttf", 200, "Totally a game title", {255, 255, 255}))
	{
		std::cout << "Failed to load font texture!" << std::endl;
		return false;
	}

	if (!m_play_text->LoadFont("Fonts/Pacifico.ttf", 50, "Press space to play", {255, 255, 255}))
	{
		std::cout << "Failed to load font texture!" << std::endl;
		return false;
	}

	if (!m_loading_text->LoadFont("Fonts/Pacifico.ttf", 200, " ", { 255, 255, 255 }))
	{
		std::cout << "Failed to load font texture!" << std::endl;
		return false;
	}

	return false;
}

void MainMenu::SetLevelMap()
{
	ifstream inFile;

	inFile.open("Levels/MainMenu.txt");

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



