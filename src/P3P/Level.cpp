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
	setMap (pLevelNumber);
	loadMap ();
}


//////////////////////////////|	LEVEL ACCESS
//Increase the amount of levels the player has access to
void Level::increaseLevelKey ()
{
	_levelKey ++;
}
//Get the current levelkey
int Level::levelKey ()
{
	return _levelKey;
}


//////////////////////////////|	LEVEL BUILDING
//Load a map by the level number
void Level::setMap (int pLevelNumber)
{
	_levelNumber = pLevelNumber;
	if (_levelNumber == 0)
	{
		map = LevelImporter::ReadFile ("Hub.tmx");
	}//else if (_levelNumber == bossLevel) { ReadFile ("HubLevel.tmx"); }
	else
	{
		map = LevelImporter::ReadFile ("Level" + std::to_string (pLevelNumber) + ".tmx");
	}
}

//Build level as described in map
void Level::loadMap ()
{
	if (_levelNumber == 0)
	{
		_isHub = true;
		levelCompleted = true;
	}
	else
	{
		_isHub = false;
		levelCompleted = false;
	}

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
					//Floor tile type 1
					temp = new Floor (x, y, 1);
					temp->setParent (this);
					map->baseTiles [x] [y] = (int)temp;
					break;
				case 2:
					//Floor tile type 2
					temp = new Floor (x, y, 2);
					temp->setParent (this);
					map->baseTiles [x] [y] = (int)temp;
					break;
				case 3:
					//Breaking block
					temp = new BreakingBlock(x, y);
					temp->setParent(this);
					map->baseTiles[x][y] = (int)temp;
					break;
				case 5:
					//Box spot
					temp = new BoxSpot (x, y);
					temp->setParent (this);
					map->baseTiles [x] [y] = (int)temp;
					progressTracker->boxSpots.push_back ((BoxSpot*)temp);
					break;
				case 9:
					//Spikes
					temp = new Spikes(x, y);
					temp->setParent(this);
					map->baseTiles[x][y] = (int)temp;
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
					//Npc
					temp = new Npc(x, y);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 8:
					//Collectible A
					temp = new Collectible(x, y, "itemA");
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 7:
					//Collectible B
					temp = new Collectible(x, y, "itemB");
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				default:
					break;
			}
		}
	}
	//Doors need to be objects, so they can have a property indicating what level they should load
	DoorObject* doorObject;
	for (int i = 0, size = map->doorObjects.size (); i < size; i ++)
	{
		doorObject = map->doorObjects [i];
		temp = new Door (doorObject->x (), doorObject->z (), doorObject->nextLevelNumber ());
		temp->setParent (this);
		if (map->objectTiles [doorObject->x ()] [doorObject->z ()] != (int)nullptr)//If there is already an object in this space, delete it. Doors should have priority.
		{
			delete (GameObject*)map->objectTiles [doorObject->x ()] [doorObject->z ()];
		}
		map->objectTiles [doorObject->x ()] [doorObject->z ()] = (int)temp;
	}
}

//Delete all objects in the level
void Level::clear ()
{
	GameObject* temp;
	for (int x = 0; x < map->width; x ++)
	{
		for (int y = 0; y < map->height; y++)
		{
			temp = (GameObject*)map->baseTiles [x] [y];
			if (temp != nullptr)
			{
				temp->setParent (nullptr);
				delete temp;
			}
			temp = (GameObject*)map->objectTiles [x] [y];
			if (temp != nullptr)
			{
				temp->setParent (nullptr);
				delete temp;
			}
		}
		map->baseTiles [x].clear ();
		map->objectTiles [x].clear ();
	}
	map->baseTiles.clear ();
	map->objectTiles.clear ();
	for (int i = 0, size = map->doorObjects.size (); i < size; i ++)
	{
		delete map->doorObjects [i];
	}
	map->doorObjects.clear ();
}

//Clear everything in the level, and build a new level
void Level::loadLevel (int pLevelNumber)
{
	clear ();
	setMap (pLevelNumber);
	loadMap ();
}

//Reload the current level
void Level::reloadLevel ()
{
	clear ();
	setMap (_levelNumber);
	loadMap ();
}