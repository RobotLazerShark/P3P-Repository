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
//Destructor
Level::~Level ()
{
	delete map;
}

void Level::update (float pStep, bool pUpdateWorldTransform)
{
	if (_nextLevel != -1)
	{
		clear ();
		setMap (_nextLevel);
		loadMap ();
		_nextLevel = -1;
	}
	GameObject::update (pStep, pUpdateWorldTransform);
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
	if (map != nullptr)
	{
		delete map;
	}

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
				case 2:
				case 3:
				case 4:
					//Floor tiles (same functionality, different looks)
					temp = new Floor (x, y, map->baseTiles [x] [y]);
					break;
				case 17:
					//Breaking floortile
					temp = new BreakingBlock (x, y);
					break;
				case 18:
					//Box spot
					temp = new BoxSpot (x, y);
					progressTracker->boxSpots.push_back ((BoxSpot*)temp);
					break;
				case 19:
					//Floortile with spikes
					temp = new Spikes (x, y);
					break;
				default:
					break;
			}
			temp->setParent (this);
			map->baseTiles [x] [y] = (int)temp;
		}
	}
	//Build all object tiles
	for (int x = 0; x < map->width; x++)
	{
		for (int y = 0; y < map->height; y++)
		{
			switch (map->objectTiles [x] [y])
			{
				case 33:
					//Player
					temp = new Player (x, y, progressTracker);
					break;
				case 34:
					//Box
					temp = new Box (x, y);
					break;
				case 35:
					//Npc
					temp = new Npc (x, y);
				case 36:
					//Collectable
					temp = new Collectable (x, y);
					break;
				case 37:
					//Gate
					temp = new Gate (x, y);
					_gates.push_back ((Gate*)temp);//Track all gates to make sure open gates are deleted as well
					break;
			//	case 39:
			//	case 40:
			//	case 41:
			//	case 42:
			//		//Fan
			//		temp = new Fan (x, y, (map->objectTiles [x] [y] - 38));
			//		break;
				default:
					break;
			}
			temp->setParent (this);
			map->objectTiles [x] [y] = (int)temp;
		}
	}
	//Doors and buttons need to be objects, so they can have properties
	XmlObject* object;
	for (int i = 0, size = map->xmlObjects.size (); i < size; i ++)
	{
		object = map->xmlObjects [i];
		switch (object->type)
		{
			case 38:
				//Door: property = number of level to load
				temp = new Door (object->x, object->z, object->properties [0]);
				break;
			case 20:
				//Button: property = x & y of the object it (de)activates
				temp = new Button (object->x, object->z, (ButtonTarget*)map->objectTiles [object->properties [0]] [object->properties [1]]);
				break;
			default:
				break;
		}
		temp->setParent (this);
		//If there is an object already in this place, delete it.
		if (map->objectTiles [object->x] [object->z] != (int)nullptr)
		{
			delete (GameObject*)map->objectTiles [object->x] [object->z];
		}
		map->objectTiles [object->x] [object->z] = (int)temp;
	}
}

//Delete all objects in the level
void Level::clear ()
{
	GameObject* temp;
	int ptr;
	for (int x = 0; x < map->width; x ++)
	{
		for (int y = 0; y < map->height; y++)
		{
			ptr = map->baseTiles [x] [y];
			if (ptr != (int)nullptr)
			{
				temp = (GameObject*)ptr;
				temp->setParent (nullptr);
				delete temp;
			}
			ptr = map->objectTiles [x] [y];
			if (ptr != (int)nullptr)
			{
				temp = (GameObject*)ptr;
				temp->setParent (nullptr);
				delete temp;
			}
		}
		map->baseTiles [x].clear ();
		map->objectTiles [x].clear ();
	}
	map->baseTiles.clear ();
	map->objectTiles.clear ();
	for (int i = 0, size = map->xmlObjects.size (); i < size; i ++)
	{
		delete map->xmlObjects [i];
	}
	map->xmlObjects.clear ();
	for (int i = 0, size = _gates.size (); i < size; i ++)
	{
		delete _gates [i];
	}
	_gates.clear ();
}

//Clear everything in the level, and build a new level
void Level::loadLevel (int pLevelNumber)
{
	_nextLevel = pLevelNumber;
}

//Reload the current level
void Level::reloadLevel ()
{
	_nextLevel = 0;
}