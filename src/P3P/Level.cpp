#include <P3P/Level.hpp>
#include <mge/core/World.hpp>
#include <P3P/objects/Player.hpp>
#include <P3P/objects/Npc.hpp>
#include <P3P/objects/Box.hpp>
#include <P3P/objects/Door.hpp>
#include <P3P/objects/Fan.hpp>
#include <P3P/Quest.hpp>
#include <P3P/objects/Gate.hpp>
#include <P3P/objects/Collectable.hpp>
#include <P3P/objects/base objects/Floor.hpp>
#include <P3P/objects/base objects/BoxSpot.hpp>
#include <P3P/objects/base objects/Button.hpp>
#include <P3P/objects/base objects/Spikes.hpp>
#include <P3P/objects/base objects/BreakingBlock.hpp>
#include <mge/behaviours/FollowBehaviour.hpp>
#include <mge/behaviours/ThirdPersonCameraBehaviour.hpp>
#include <mge/objects/Camera.hpp>
#include <P3P/ProgressTracker.hpp>
#include <mge/materials/LitMaterial.hpp>


//Static variables
const float Level::TILESIZE = 1;
LevelMap* Level::map = nullptr;
Level* Level::singletonInstance = nullptr;


//Constructor
Level::Level (int pLevelNumber)
{
	if (singletonInstance != nullptr)
	{
		delete singletonInstance;
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
	for (int i = 0, size = _activeQuestsCopy.size (); i < size; i ++)
	{
		if (_activeQuestsCopy [i] != nullptr)
		{
			delete _activeQuestsCopy [i];
		}
	}
	//Make sure there are no lingering singletons
	if (Player::singletonInstance != nullptr)
	{
		Player::singletonInstance->setParent (nullptr);
		delete Player::singletonInstance;
	}
	if (Npc::singletonInstance != nullptr)
	{
		Npc::singletonInstance->setParent (nullptr);
		delete Npc::singletonInstance;
	}
	singletonInstance = nullptr;
	setParent (nullptr);
	GameObject::~GameObject ();
}

void Level::update (float pStep, bool pUpdateWorldTransform)
{
	if (_stop)
	{
		return;
	}

	//Remove items from drawbuffer
	drawBuffer.clear();
	GameObject::update(pStep, pUpdateWorldTransform);

	//If we have to load a different level, do that here.
	//By having the reload function set a flag instead of directly reload the level,
	//there won't be any issues with objects being deleted when the function returns.
	if (_nextLevel != -1)
	{
		clear ();
		if (!setMap (_nextLevel))
		{
			_stop = true;
			return;
		}
		loadMap ();
		_nextLevel = -1;
	}
}


void Level::render (sf::RenderWindow* pWindow)
{
	for (int i = 0, size = Level::singletonInstance->drawBuffer.size(); i < size; i++)
	{
		//Draw item in drawbuffer
		pWindow->pushGLStates ();
		pWindow->draw (*Level::singletonInstance->drawBuffer [i]);
		pWindow->popGLStates ();
	}
}

bool Level::isHub ()
{
	return _isHub;
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
bool Level::setMap (int pLevelNumber)
{
	if (pLevelNumber != _levelNumber)
	{
		_levelNumber = pLevelNumber;
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
	}

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
	if (map == nullptr)
	{
		return false;
	}
	return true;
}

//Build level as described in map
void Level::loadMap ()
{
	//Create progresstracker, to track boxspots
	ProgressTracker* progressTracker = new ProgressTracker ();
	GameObject* temp = nullptr;
	GameObject* temp2 = nullptr;

	//Build all base tiles
	for (int x = 0; x < map->width; x++)
	{
		for (int y = 0; y < map->height; y++)
		{
			switch (map->baseTiles [x] [y])
			{
				case 1:
				case 2:
					//Floor tiles (same functionality, different looks)
					temp = new Floor (x, y, map->baseTiles [x] [y]);
					temp->setParent (this);
					map->baseTiles [x] [y] = (int)temp;
					break;
				case 17:
					//Breaking floortile
					temp = new BreakingBlock (x, y);
					temp->setParent (this);
					map->baseTiles [x] [y] = (int)temp;
					break;
				case 18:
					//Box spot
					temp = new BoxSpot (x, y);
					progressTracker->boxSpots.push_back ((BoxSpot*)temp);
					temp->setParent (this);
					map->baseTiles [x] [y] = (int)temp;
					break;
				case 19:
					//Floortile with spikes
					temp = new Spikes (x, y);
					temp->setParent (this);
					map->baseTiles [x] [y] = (int)temp;
					break;
				case 3:
				case 4:
				case 5:
				case 6:
					//Wall corner
					temp = new GameObject ();
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (0, 1, 0));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (0, 2, 0));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (1, 0, -1));
					temp2->rotate (glm::radians (-90.0f), glm::vec3 (0, 1, 0));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (1, 1, -1));
					temp2->rotate (glm::radians (-90.0f), glm::vec3 (0, 1, 0));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (1, 2, -1));
					temp2->rotate (glm::radians (-90.0f), glm::vec3 (0, 1, 0));
					temp2->setParent (temp);
					temp->setParent (this);
					temp->translate (glm::vec3 (TILESIZE * x, 0.5f, TILESIZE * y));
					temp->rotate (glm::radians (-90.0f * (map->baseTiles[x][y]-3)), glm::vec3 (0, 1, 0));
					//This tile should not be walkable
					_nonWalkables.push_back (temp);
					map->baseTiles [x] [y] = (int)nullptr;
					break;
				case 9:
				case 10:
				case 11:
				case 12:
					//Wall pipe 1
					temp = new GameObject ();
					temp2 = new GameObject ("Pipe1.obj");
					temp2->setMaterial (new LitMaterial ("Pipe1.png"));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (-1, 1.55f, 0));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (-1, 2.55f, 0));
					temp2->setParent (temp);
					temp->setParent (this);
					temp->translate (glm::vec3 (TILESIZE * x, -0.05, TILESIZE * y));
					temp->rotate (glm::radians (-90.0f * (map->baseTiles[x][y]-9)), glm::vec3 (0, 1, 0));
					map->baseTiles [x] [y] = (int)temp;
					break;
				case 13:
				case 14:
				case 15:
				case 16:
					//Wall pipe 2
					temp = new GameObject ();
					temp2 = new GameObject ("Pipe2.obj");
					temp2->setMaterial (new LitMaterial ("Pipe2.png"));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (-1, 1.55f, 0));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (0, 1.55f, -1));
					temp2->rotate (glm::radians (-90.0f), glm::vec3 (0, 1, 0));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (-1, 2.55f, 0));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (0, 2.55f, -1));
					temp2->rotate (glm::radians (-90.0f), glm::vec3 (0, 1, 0));
					temp2->setParent (temp);
					temp->setParent (this);
					temp->translate (glm::vec3 (TILESIZE * x, -0.05f, TILESIZE * y));
					temp->rotate (glm::radians (-90.0f * (map->baseTiles[x][y]-13)), glm::vec3 (0, 1, 0));
					map->baseTiles [x] [y] = (int) temp;
					break;
				case 21:
				case 22:
				case 23:
				case 24:
					//Wall
					temp = new GameObject ();
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (0, 1, 0));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (0, 2, 0));
					temp2->setParent (temp);
					temp->setParent (this);
					temp->translate (glm::vec3 (TILESIZE * x, 0.5f, TILESIZE * y));
					temp->rotate (glm::radians (-90.0f * (map->baseTiles[x][y]-21)), glm::vec3 (0, 1, 0));
					//This tile should not be walkable
					_nonWalkables.push_back (temp);
					map->baseTiles [x] [y] = (int)nullptr;
					break;
				case 25:
				case 26:
				case 27:
				case 28:
					//Wall pipe 3
					temp = new GameObject ();
					temp2 = new GameObject ("Pipe3.obj");
					temp2->setMaterial (new LitMaterial ("Pipe3.png"));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (-1, 1.55f, 0));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (0, 1.55f, -1));
					temp2->rotate (glm::radians (-90.0f), glm::vec3 (0, 1, 0));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (-1, 2.55f, 0));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (0, 2.55f, -1));
					temp2->rotate (glm::radians (-90.0f), glm::vec3 (0, 1, 0));
					temp2->setParent (temp);
					temp->setParent (this);
					temp->translate (glm::vec3 (TILESIZE * x, -0.05f, TILESIZE * y));
					temp->rotate (glm::radians (-90.0f * (map->baseTiles[x][y]-25)), glm::vec3 (0, 1, 0));
					map->baseTiles [x] [y] = (int) temp;
					break;
				case 29:
				case 30:
				case 31:
				case 32:
					//Wall pipe 4
					temp = new GameObject ();
					temp2 = new GameObject ("Pipe4.obj");
					temp2->setMaterial (new LitMaterial ("Pipe4.png"));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (-1, 1.55f, 0));
					temp2->setParent (temp);
					temp2 = new GameObject ("WallPipeL.obj");
					temp2->setMaterial (new LitMaterial ("WallPipeL.png"));
					temp2->translate (glm::vec3 (0, 1.55f, -1));
					temp2->rotate (glm::radians (-90.0f), glm::vec3 (0, 1, 0));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->translate (glm::vec3 (-1, 2.55f, 0));
					temp2->setParent (temp);
					temp2 = new GameObject ("WallPipeL.obj");
					temp2->setMaterial (new LitMaterial ("WallPipeL.png"));
					temp2->translate (glm::vec3 (0, 2.55f, -1));
					temp2->rotate (glm::radians (-90.0f), glm::vec3 (0, 1, 0));
					temp2->setParent (temp);
					temp->setParent (this);
					temp->translate (glm::vec3 (TILESIZE * x, -0.05f, TILESIZE * y));
					temp->rotate (glm::radians (-90.0f * (map->baseTiles[x][y]-29)), glm::vec3 (0, 1, 0));
					map->baseTiles [x] [y] = (int) temp;
					break;
				default:
					map->baseTiles [x] [y] = (int)nullptr;
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
				case 33:
					//Player
					temp = new Player (x, y, progressTracker);
					temp->setParent (this);
					map->objectTiles [x] [y] = (int)temp;
					if (_inventoryCopy.size () > 0)
					{
						((Player*)temp)->inventory = std::vector <std::string> (_inventoryCopy);
					}
					break;
				case 34:
					//PowerBox
					temp = new Box (x, y, true);
					temp->setParent (this);
					map->objectTiles [x] [y] = (int)temp;
					break;
				case 35:
					//Npc
					temp = new Npc (x, y);
					temp->setParent (this);
					map->objectTiles [x] [y] = (int)temp;
					if (_activeQuestsCopy.size () > 0)
					{
						((Npc*)temp)->activeQuests = std::vector <Quest*> (_activeQuestsCopy);
					}
					Npc::singletonInstance->questTalks = _questTalksCopy;
					break;
				case 37:
					//Gate
					temp = new Gate (x, y);
					_gates.push_back ((Gate*)temp);//Track all gates to make sure open gates are deleted as well
					temp->setParent (this);
					map->objectTiles [x] [y] = (int)temp;
					break;
				case 39: //Fan right
					temp = new Fan(x, y, 1, 0, false);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 40: //Fan down
					temp = new Fan(x, y, 0, 1, false);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 41: //Fan left
					temp = new Fan(x, y, -1, 0, false);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 42: //Fan up
					temp = new Fan(x, y, 0, -1, false);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 43: //Fan reversed right
					temp = new Fan(x, y, 0, 1, true);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 44: //Fan reversed down
					temp = new Fan(x, y, 0, 1, true);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 45: //Fan reversed left
					temp = new Fan(x, y, -1, 0, true);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 46: //Fan reversed up
					temp = new Fan(x, y, 0, -1, true);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 48:
					//Box
					temp = new Box (x, y, false);
					temp->setParent (this);
					map->objectTiles [x] [y] = (int)temp;
					break;
				case 49:
					//Lights
					temp = new GameObject ("Lights.obj");
					temp->setMaterial (new LitMaterial ("Lights.png"));
					temp->setParent (this);
					temp->translate (glm::vec3 (TILESIZE * x, 0, TILESIZE * y));
					temp->rotate (glm::radians (180.0f), glm::vec3 (0, 1, 0));
					map->objectTiles [x] [y] = (int)temp;
					if (x > 0)
					{
						map->objectTiles [x-1] [y] = (int)new GameObject ();
					}
					break;
				default:
					map->objectTiles[x][y] = (int)nullptr;
					break;
			}
		}
	}
	//XmlObjects are used to we can use properties
	XmlObject* object;
	for (int i = 0, size = map->xmlObjects.size (); i < size; i ++)
	{
		object = map->xmlObjects [i];
		switch (object->type)
		{
			case 8:
				//Camera: property = distance & height or height & intensity
				switch (std::stoi (object->properties [0]))
				{
					case 1:
						World::singletonInstance->getMainCamera ()->setBehaviour (new ThirdPersonCameraBehaviour (std::stof (object->properties [1]), std::stof (object->properties [2])));
						break;
					case 2:
						World::singletonInstance->getMainCamera ()->setBehaviour (new PercentageFollowBehaviour (std::stof (object->properties [2]), (object->x * TILESIZE), std::stof (object->properties [1]), (object->z * TILESIZE)));
						break;
					default:
						break;
				}
				break;
			case 20:
				//Button: property = x & y of the object it (de)activates
				temp = new Button (object->x, object->z, (ButtonTarget*)map->objectTiles [std::stoi (object->properties [0])] [std::stoi (object->properties [1])]);
				temp->setParent (this);
				//If there is an object already in this place, delete it.
				if (map->baseTiles [object->x] [object->z] != (int)nullptr)
				{
					GameObject* block = (GameObject*)map->baseTiles [object->x] [object->z];
					block->setParent (nullptr);
					delete block;
				}
				map->baseTiles [object->x] [object->z] = (int)temp;
				break;
			case 36:
				//Collectable: property = name & stay & dialog
				if (object->properties.size () > 1)
				{
					temp = new Collectable (object->x, object->z, object->properties [0], object->properties [2], (std::stoi (object->properties [1]) > 0));
				}
				else
				{
					temp = new Collectable (object->x, object->z, object->properties [0]);
				}
				temp->setParent (this);
				//If there is an object already in this place, delete it.
				if (map->objectTiles [object->x] [object->z] != (int)nullptr)
				{
					GameObject* block = (GameObject*)map->objectTiles [object->x] [object->z];
					block->setParent (nullptr);
					delete block;
				}
				map->objectTiles [object->x] [object->z] = (int)temp;
				break;
			case 38:
				//Door: property = number of level to load & orientation
				temp = new Door (object->x, object->z, std::stoi (object->properties [0]), std::stoi (object->properties [1]));
				temp->setParent (this);
				//If there is an object already in this place, delete it.
				if (map->objectTiles [object->x] [object->z] != (int)nullptr)
				{
					GameObject* block = (GameObject*)map->objectTiles [object->x] [object->z];
					block->setParent (nullptr);
					delete block;
				}
				map->objectTiles [object->x] [object->z] = (int)temp;
				progressTracker->doors.push_back ((Door*)temp);
				break;
			default:
				break;
		}
	}
}

//Delete all objects in the level
void Level::clear ()
{
	//Copy important lists
	if (Player::singletonInstance != nullptr && Player::singletonInstance->inventory.size () > 0)
	{
		_inventoryCopy.clear ();
		_inventoryCopy = std::vector <std::string> (Player::singletonInstance->inventory);
	}
	if (Npc::singletonInstance != nullptr)
	{
		_questTalksCopy = Npc::singletonInstance->questTalks;
		if (Npc::singletonInstance->activeQuests.size () > 0)
		{
			_activeQuestsCopy.clear ();
			_activeQuestsCopy = std::vector <Quest*> (Npc::singletonInstance->activeQuests);
		}
	}

	//Delete gates
	for (Gate* gate : _gates)
	{
		map->objectTiles [gate->x ()] [gate->z ()] = (int)nullptr;
		gate->setParent (nullptr);
		delete gate;
	}
	_gates.clear ();
	//Delete collectables that have not yet been deleted
	for (Collectable* collectable : deleteBuffer)
	{
		delete collectable;
	}
	deleteBuffer.clear ();

	//Delete decorative, non-walkable baselayer tiles
	for (int i = 0, size = _nonWalkables.size (); i < size; i ++)
	{
		_nonWalkables [i]->setParent (nullptr);
		delete _nonWalkables [i];
	}
	_nonWalkables.clear ();

	//Delete platforms and objects
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
				map->baseTiles [x] [y] = (int)nullptr;
			}
			ptr = map->objectTiles [x] [y];
			if (ptr != (int)nullptr)
			{
				temp = (GameObject*)ptr;
				temp->setParent (nullptr);
				delete temp;
				map->objectTiles [x] [y] = (int)nullptr;
			}
		}
		map->baseTiles [x].clear ();
		map->objectTiles [x].clear ();
	}
	map->baseTiles.clear ();
	map->objectTiles.clear ();

	//Delete xmlobjects
	for (int i = 0, size = map->xmlObjects.size (); i < size; i ++)
	{
		delete map->xmlObjects [i];
	}
	map->xmlObjects.clear ();

	World::singletonInstance->getMainCamera ()->setBehaviour (nullptr);
}

//Clear everything in the level, and build a new level
void Level::loadLevel (int pLevelNumber)
{
	_nextLevel = pLevelNumber;
}

//Reload the current level
void Level::reloadLevel ()
{
	_nextLevel = _levelNumber;
}