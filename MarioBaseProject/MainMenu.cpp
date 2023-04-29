#include "MainMenu.h"

MainMenu::MainMenu(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpLevel();
}

MainMenu::~MainMenu()
{

}

void MainMenu::Render()
{
	background->Render(camera);

	for (int i = 0; i < m_decoTiles.size(); i++) { m_decoTiles[i]->Render(camera); }
	for (int i = 0; i < m_tiles.size(); i++) { m_tiles[i]->Render(camera); }


	m_title_text->Render(250, 40);
	m_play_text->Render(1000, 30);
}

void MainMenu::Update(float deltaTime, SDL_Event e)
{
	
	background->Update(deltaTime);

	if (camera->x < 0) { camera->x = 0; }
	else if (camera->x > LEVEL_WIDTH - camera->w) { camera->x = LEVEL_WIDTH - camera->w; }

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

void MainMenu::Set8BitTileNeighbours(std::vector<Vector2D>& passedVector, int typeCheck)
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

void MainMenu::Set4BitTileNeighbours(std::vector<Vector2D>& passedVector, int typeCheck)
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

void MainMenu::CreateTiles(Vector2D position, std::string binaryType)
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

void MainMenu::CreateWaterLavaTiles(Vector2D position, std::string binaryType, std::string palette)
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

void MainMenu::PaintTile(Vector2D position, std::string filePath)
{

	int xPos = position.x * TILE_WIDTH;
	int yPos = position.y * TILE_HEIGHT;

	Tile* tile = new Tile(m_renderer, filePath, Vector2D(xPos, yPos));
	m_tiles.push_back(tile);
} //Above player

void MainMenu::PaintDecoTile(Vector2D position, std::string filePath)
{

	int xPos = position.x * TILE_WIDTH;
	int yPos = position.y * TILE_HEIGHT;

	Tile* tile = new Tile(m_renderer, filePath, Vector2D(xPos, yPos));
	m_decoTiles.push_back(tile);
}





