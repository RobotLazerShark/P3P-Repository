#include <P3P/Level.hpp>
#include <mge/core/World.hpp>
#include <P3P/objects/Player.hpp>
#include <P3P/objects/Boss.hpp>
#include <P3P/objects/Npc.hpp>
#include <P3P/objects/Box.hpp>
#include <P3P/objects/Door.hpp>
#include <P3P/objects/Fan.hpp>
#include <P3P/Quest.hpp>
#include <P3P/objects/Gate.hpp>
#include <P3P/objects/Collectable.hpp>
#include <P3P/objects/Plug.hpp>
#include <P3P/objects/Socket.hpp>
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
#include <mge/materials/TextureMaterial.hpp>


//Static variables
const float Level::TILESIZE = 1;
LevelMap* Level::map = nullptr;
Level* Level::singletonInstance = nullptr;


//Constructor
Level::Level (int pPlayerSkin)
{
	if (singletonInstance != nullptr)
	{
		delete singletonInstance;
	}
	singletonInstance = this;
	_playerSkin = pPlayerSkin;
	setParent (World::singletonInstance);

	hud = new Hud();
	setMap (0);
	loadMap ();
}
//Destructor
Level::~Level ()
{
	delete map;
	delete hud;
	//delete hints
	for (Hint * hint : hints)
	{
		hint->setParent(nullptr);
		delete hint;
	}
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
	for (sf::Drawable * drawable : hud->getAllDrawables())
	{
		drawBuffer.push_back(drawable);
	}
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
		_reloading = false;
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
		if (_levelNumber == 0 || _levelNumber == _bossLevelNumber)
		{
			_isHub = true;
			levelCompleted = (_levelNumber == 0);

			hud->setState(1);
		}
		else
		{
			_isHub = false;
			levelCompleted = false;

			hud->setState(2);
		}
	}

	if (map != nullptr)
	{
		delete map;
	}

	if (_levelNumber == 0)
	{
		map = LevelImporter::ReadFile ("Hub.tmx");
		//map = LevelImporter::ReadFile("Level2.tmx");
	}
	else if (_levelNumber == _bossLevelNumber)
	{
		map = LevelImporter::ReadFile ("BossLevel.tmx");
	}
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
				case 3:
					//Floor tiles (same functionality, different looks)
					temp = new Floor (x, y, map->baseTiles [x] [y]);
					temp->setParent (this);
					map->baseTiles [x] [y] = (int)temp;
					break;
				case 5: //Breaking floortile
					temp = new BreakingBlock (x, y);
					temp->setParent (this);
					map->baseTiles [x] [y] = (int)temp;
					break;
				case 6: //Box spot
					temp = new BoxSpot (x, y);
					progressTracker->boxSpots.push_back ((BoxSpot*)temp);
					temp->setParent (this);
					map->baseTiles [x] [y] = (int)temp;
					break;
				case 7: //Floortile with spikes
					temp = new Spikes (x, y);
					temp->setParent (this);
					map->baseTiles [x] [y] = (int)temp;
					break;
				case 8: // Mirror
					temp = new Mirror(x, y);
					temp->setParent(this);
					map->baseTiles[x][y] = (int)temp;
					break;
				case 9:
				case 10:
				case 11:
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
				case 17:
				case 18:
					//Wall corner
					temp = new GameObject();
					temp2 = new GameObject("Wall.obj");
					temp2->setMaterial(new LitMaterial("Wall.png"));
					temp2->setParent(temp);
					temp2 = new GameObject("Wall.obj");
					temp2->setMaterial(new LitMaterial("Wall.png"));
					temp2->translate(glm::vec3(0, 1, 0));
					temp2->setParent(temp);
					temp2 = new GameObject("Wall.obj");
					temp2->setMaterial(new LitMaterial("Wall.png"));
					temp2->translate(glm::vec3(0, 2, 0));
					temp2->setParent(temp);
					temp2 = new GameObject("Wall.obj");
					temp2->setMaterial(new LitMaterial("Wall.png"));
					temp2->translate(glm::vec3(1, 0, -1));
					temp2->rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0));
					temp2->setParent(temp);
					temp2 = new GameObject("Wall.obj");
					temp2->setMaterial(new LitMaterial("Wall.png"));
					temp2->translate(glm::vec3(1, 1, -1));
					temp2->rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0));
					temp2->setParent(temp);
					temp2 = new GameObject("Wall.obj");
					temp2->setMaterial(new LitMaterial("Wall.png"));
					temp2->translate(glm::vec3(1, 2, -1));
					temp2->rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0));
					temp2->setParent(temp);
					temp->setParent(this);
					temp->translate(glm::vec3(TILESIZE * x, 0.5f, TILESIZE * y));
					temp->rotate(glm::radians(-90.0f * (map->baseTiles[x][y] - 17)), glm::vec3(0, 1, 0));
					//This tile should not be walkable
					_nonWalkables.push_back(temp);
					map->baseTiles[x][y] = (int)nullptr;
					break;
				case 19:
				case 20:
					//Wall corner
					temp = new GameObject();
					temp2 = new GameObject("Wall.obj");
					temp2->setMaterial(new LitMaterial("Wall.png"));
					temp2->translate (glm::vec3 (-1, 0, 1));
					temp2->setParent(temp);
					temp2 = new GameObject("Wall.obj");
					temp2->setMaterial(new LitMaterial("Wall.png"));
					temp2->translate(glm::vec3(-1, 1, 1));
					temp2->setParent(temp);
					temp2 = new GameObject("Wall.obj");
					temp2->setMaterial(new LitMaterial("Wall.png"));
					temp2->translate(glm::vec3(-1, 2, 1));
					temp2->setParent(temp);
					temp2 = new GameObject("Wall.obj");
					temp2->setMaterial(new LitMaterial("Wall.png"));
					temp2->translate(glm::vec3(0, 0, 0));
					temp2->rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0));
					temp2->setParent(temp);
					temp2 = new GameObject("Wall.obj");
					temp2->setMaterial(new LitMaterial("Wall.png"));
					temp2->translate(glm::vec3(0, 1, 0));
					temp2->rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0));
					temp2->setParent(temp);
					temp2 = new GameObject("Wall.obj");
					temp2->setMaterial(new LitMaterial("Wall.png"));
					temp2->translate(glm::vec3(0, 2, 0));
					temp2->rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0));
					temp2->setParent(temp);
					temp->setParent(this);
					temp->translate(glm::vec3(TILESIZE * x, 0.5f, TILESIZE * y));
					temp->rotate(glm::radians(-90.0f * (map->baseTiles[x][y] - 19)), glm::vec3(0, 1, 0));
					//This tile should not be walkable
					_nonWalkables.push_back(temp);
					map->baseTiles[x][y] = (int)nullptr;
					break;
				case 21:
				case 22:
				case 23:
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
				case 32:
					//Wall pipe 5
					temp = new GameObject ();
					temp = new GameObject ("Pipe5.obj");
					temp->setMaterial (new LitMaterial ("Pipe5.png"));
					temp->translate (glm::vec3 (TILESIZE * x, -0.05f, TILESIZE * y));
					temp->rotate (glm::radians (-270.0f), glm::vec3 (0, 1, 0));
					temp->setParent (this);
					//This tile should not be walkable
					_nonWalkables.push_back (temp);
					map->baseTiles [x] [y] = (int)nullptr;
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
					temp = new Player (x, y, progressTracker, _playerSkin);
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
					//Box
					temp = new Box(x, y, false);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 36:
					//Npc
					temp = new Npc (x, y);
					temp->setParent (this);
					map->objectTiles [x] [y] = (int)temp;
					if (_activeQuestsCopy.size () > 0)
					{
						((Npc*)temp)->activeQuests = std::vector <Quest*> (_activeQuestsCopy);
					}
					break;
				case 37:
					//Gate
					temp = new Gate (x, y);
					_gates.push_back ((Gate*)temp);//Track all gates to make sure open gates are deleted as well
					temp->setParent (this);
					map->objectTiles [x] [y] = (int)temp;
					break;
				case 38: //Boss
					temp = new Boss(x, y);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 39:
					//Lights
					temp = new GameObject("Lights.obj");
					temp->setMaterial(new LitMaterial("Lights.png"));
					temp->setParent(this);
					temp2 = new GameObject ("ShadowPlane.obj");
					temp2->setMaterial (new TextureMaterial ("LightsShadow.png"));
					temp2->badScale (glm::vec3 (2, 1, 1));
					temp2->rotate (glm::radians (180.0f), glm::vec3 (0,1,0));
					temp2->translate (glm::vec3 (-0.25f, 0.08f, 0));
					temp2->setParent (temp);
					temp->translate(glm::vec3(TILESIZE * x, 0, TILESIZE * y));
					temp->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
					map->objectTiles[x][y] = (int)temp;
					if (x > 0)
					{
						map->objectTiles[x - 1][y] = (int)new GameObject();
					}
					break;
				case 40:
					temp = new GameObject("cube_flat.obj");
					temp->setMaterial(new LitMaterial("Wall.jpg"));
					temp->setParent(this);
					temp->translate(glm::vec3(TILESIZE * x, 0.5f, TILESIZE * y));
					map->objectTiles[x][y] = (int)temp;
					break;
				case 41: //Fan right
					temp = new Fan(x, y, 1, 0, false);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 42: //Fan down
					temp = new Fan(x, y, 0, 1, false);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 43: //Fan left
					temp = new Fan(x, y, -1, 0, false);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 44: //Fan up
					temp = new Fan(x, y, 0, -1, false);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 45: //Fan reversed right
					temp = new Fan(x, y, 0, 1, true);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 46: //Fan reversed down
					temp = new Fan(x, y, 0, 1, true);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 47: //Fan reversed left
					temp = new Fan(x, y, -1, 0, true);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 48: //Fan reversed up
					temp = new Fan(x, y, 0, -1, true);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 49: //Plug up
					temp = new Plug(x, y, 1);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 50: //Plug down
					temp = new Plug(x, y, 2);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 51: //Plug right
					temp = new Plug(x, y, 3);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				case 52: //Plug left
					temp = new Plug(x, y, 4);
					temp->setParent(this);
					map->objectTiles[x][y] = (int)temp;
					break;
				default:
					map->objectTiles[x][y] = (int)nullptr;
					break;
			}
		}
	}
	//XmlObjects are used to we can use properties
	XmlObject* object;
	ThirdPersonCameraBehaviour* behaviour;
	for (int i = 0, size = map->xmlObjects.size (); i < size; i ++)
	{
		object = map->xmlObjects [i];
		switch (object->type)
		{
			case 58:
				//Camera: property = distance & height or height & intensity
				switch (std::stoi (object->properties [0]))
				{
					case 1:
						behaviour = new ThirdPersonCameraBehaviour (std::stof (object->properties [1]), std::stof (object->properties [2]));
						World::singletonInstance->getMainCamera ()->setBehaviour (behaviour);
						if (_reloading)
						{
							behaviour->startTransition (Player::singletonInstance->getLocalPosition (), 1.0f);
						}
						break;
					case 2:
						World::singletonInstance->getMainCamera ()->setBehaviour (new PercentageFollowBehaviour (std::stof (object->properties [2]), (object->x * TILESIZE), std::stof (object->properties [1]), (object->z * TILESIZE)));
						break;
					default:
						break;
				}
				break;
			case 60:
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
			case 59:
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
			case 57:
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
			case 61: //Socket up
				temp = new Socket(object->x, object->z, 1, std::stoi(object->properties[0]), std::stoi(object->properties[1]));
				temp->setParent(this);
				if (map->objectTiles[object->x][object->z] != (int)nullptr)
				{
					GameObject* block = (GameObject*)map->objectTiles[object->x][object->z];
					block->setParent(nullptr);
					delete block;
				}
				map->objectTiles[object->x][object->z] = (int)temp;
				break;
			case 62: //Socket down
				temp = new Socket(object->x, object->z, 2, std::stoi(object->properties[0]), std::stoi(object->properties[1]));
				temp->setParent(this);
				if (map->objectTiles[object->x][object->z] != (int)nullptr)
				{
					GameObject* block = (GameObject*)map->objectTiles[object->x][object->z];
					block->setParent(nullptr);
					delete block;
				}
				map->objectTiles[object->x][object->z] = (int)temp;
				break;
			case 63: //Socket right
				temp = new Socket(object->x, object->z, 3, std::stoi(object->properties[0]), std::stoi(object->properties[1]));
				temp->setParent(this);
				if (map->objectTiles[object->x][object->z] != (int)nullptr)
				{
					GameObject* block = (GameObject*)map->objectTiles[object->x][object->z];
					block->setParent(nullptr);
					delete block;
				}
				map->objectTiles[object->x][object->z] = (int)temp;
				break;
			case 64: //Socket left
				temp = new Socket(object->x, object->z, 4, std::stoi(object->properties[0]), std::stoi(object->properties[1]));
				temp->setParent(this);
				if (map->objectTiles[object->x][object->z] != (int)nullptr)
				{
					GameObject* block = (GameObject*)map->objectTiles[object->x][object->z];
					block->setParent(nullptr);
					delete block;
				}
				map->objectTiles[object->x][object->z] = (int)temp;
				break;
			case 65: //hints
				temp = new Hint(object->x, object->z, object->properties[0]);
				temp->setParent(this);
				hints.push_back((Hint*)temp);
				break;
			default:
				break;
		}
	}

	hud->enable();
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
	if (Boss::singletonInstance != nullptr)
	{
		delete Boss::singletonInstance;
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

	//Delete hints
	for (int i = 0, size = hints.size(); i < size; i++)
	{
		hints[i]->setParent(nullptr);
		delete hints[i];
	}
	hints.clear();

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
	_reloading = true;
}