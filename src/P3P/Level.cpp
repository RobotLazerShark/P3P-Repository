#include <P3P/Level.hpp>


//Static variables
const float Level::TILESIZE = 1;
LevelMap* Level::map = nullptr;
Level* Level::singletonInstance = nullptr;


//Constructor
Level::Level (int pLevelNumber)
{
	if (singletonInstance != nullptr)
	{
		return;
	}
	singletonInstance = this;
	setParent (World::singletonInstance);
	map = LevelImporter::ReadFile ("Level" + std::to_string (pLevelNumber) + ".tmx");
	loadMap ();
	_levelNumber = pLevelNumber;
}


//////////////////////////////|	LEVEL BUILDING
//Load a map by the level number
void Level::setMap (int pLevelNumber)
{
	map = LevelImporter::ReadFile ("Level" + std::to_string (pLevelNumber) + ".tmx");
	_levelNumber = pLevelNumber;
}

//Build level as described in map
void Level::loadMap ()
{
	//Create progresstracker, to track boxspots
	ProgressTracker* progressTracker = new ProgressTracker ();
	GameObject* temp = nullptr;

	//Build all base tiles
	for (int x = 0; x < map->width; x++)
	{
		for (int y = 0; y < map->height; y++)
		{
			switch (map->baseTiles [x] [y])
			{
				case 1:
					//Floor tile
					temp = new Floor (x, y);
					temp->setParent (this);
					map->baseTiles [x] [y] = (int)temp;
					break;
				case 2:
					//Floor tile 2
					temp = new Floor (x, y);
					temp->setParent (this);
					map->baseTiles [x] [y] = (int)temp;
					break;
				case 5:
					//Box spot
					temp = new BoxSpot (x, y);
					temp->setParent (this);
					map->baseTiles [x] [y] = (int)temp;
					progressTracker->boxSpots.push_back ((BoxSpot*)temp);
					break;
				default:
					break;
			}
		}
	}
	//Build all object tiles
	for (int x = 0; x < map->width; x++)
	{
		for (int y = 0; y < map->height; y++)
		{
			switch (map->objectTiles [x] [y])
			{
				case 3:
					//Player
					temp = new Player (x, y, progressTracker);
					temp->setParent (this);
					map->objectTiles [x] [y] = (int)temp;
					break;
				case 4:
					//Box
					temp = new Box (x, y);
					temp->setParent (this);
					map->objectTiles [x] [y] = (int)temp;
					break;
				case 6:
					//Door
					temp = new Door (x, y, _levelNumber);
					temp->setParent (this);
					map->objectTiles [x] [y] = (int)temp;
					break;
				default:
					break;
			}
		}
	}
}

//Delete all objects in the level
void Level::clear ()
{
	for (int x = 0; x < map->width; x ++)
	{
		for (int y = 0; y < map->height; y++)
		{
			delete (GameObject*)map->baseTiles [x] [y];
			delete (GameObject*)map->objectTiles [x] [y];
		}
		map->baseTiles [x].clear ();
		map->objectTiles [x].clear ();
	}
	map->baseTiles.clear ();
	map->objectTiles.clear ();
}