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
#include <mge/behaviours/BossCameraBehaviour.hpp>
#include <mge/objects/Camera.hpp>
#include <P3P/ProgressTracker.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <mge/materials/TextureMaterial.hpp>
#include <JCPPEngine/Random.hpp>
#include <JCPPEngine/TextureManager.hpp>
#include "P3P/BossCameraAnchor.hpp"
#include <JCPPEngine/SoundManager.hpp>
#include <P3P/SceneFader.hpp>
#include <mge/objects/Light.hpp>
#include <mge/util/ShaderDataUtil.hpp>
#include <P3P/Background.hpp>
#include <mge/core/AbstractGame.hpp>

//Static variables
const float Level::TILESIZE = 1;
LevelMap* Level::map = nullptr;
Level* Level::singletonInstance = nullptr;


//Constructor
Level::Level (int pPlayerSkin, sf::RenderWindow* pWindow)
{
	if (singletonInstance != nullptr)
	{
		delete singletonInstance;
	}
	singletonInstance = this;
	_hudOverlay = new sf::Sprite (*JCPPEngine::TextureManager::GetTexture ("images/Hud.png"));
	_playerSkin = pPlayerSkin;
	JCPPEngine::SoundManager::PlayMusicLoop ("sounds/BackgroundLoop.wav");

	_background = new Background ();
	_endScreen = new sf::Sprite (*JCPPEngine::TextureManager::GetTexture ("images/EndScreen.png"));
	_endScreen->setColor (sf::Color (255, 255, 255, 0));

	//Add light
	ShaderDataUtil::SetAmbientLight (glm::vec3 (0.8f, 0.9f, 1), 0.1f);
	Light* dirLight = new Light (glm::vec3 (1, 1, 1), 0.45f);
	dirLight->rotate (glm::radians (25.0f), glm::vec3 (0, 1, 0));
	dirLight->rotate (glm::radians (45.0f), glm::vec3 (1, 0, 0));
	dirLight->setParent (World::singletonInstance);

	setParent (World::singletonInstance);
	//Add transparency layers (transparent objects need to be rendered in correct order, before non-transparent objects)
	transparencyLayer1 = new GameObject ();
	transparencyLayer1->setParent (World::singletonInstance);
	transparencyLayer2 = new GameObject ();
	transparencyLayer2->setParent (World::singletonInstance);
	transparencyLayer3 = new GameObject ();
	transparencyLayer3->setParent (World::singletonInstance);

	hud = new Hud(pWindow);
	hud->setParent(this);
	setMap (_bossLevelNumber);
	loadMap ();
}
//Destructor
Level::~Level ()
{
	delete map;
	delete hud;
	delete _hudOverlay;
	delete _background;
	//delete bossPuzzletracker
	for (ProgressTracker * bossPuzzleTracker : bossPuzzlesTrackers)
	{
		delete bossPuzzleTracker;
	}
	//delete hints
	for (Hint * hint : hints)
	{
		hint->setParent(nullptr);
		delete hint;
	}
	hints.clear ();
	_activeQuestsCopy.clear ();
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

	if (_fading)
	{
		_endTimer += pStep;
		drawBuffer.clear();
		for (sf::Drawable * drawable : hud->getAllDrawables())
		{
			drawBuffer.push_back(drawable);
		}
		drawBuffer.push_back (_hudOverlay);
		if (_endTimer >= 0.05f)
		{
			int alpha = _endScreen->getColor ().a + 1;
			if (alpha <= 255)
			{
				_endScreen->setColor (sf::Color (255, 255, 255, alpha));
				_endTimer = 0;
			}
			else
			{
				AbstractGame::singletonInstance->Stop ();
			}
		}
		drawBuffer.push_back (_endScreen);
		return;
	}

	//if it's boss level check mini puzzles
	for (int i = 0; i < bossPuzzlesTrackers.size(); i++)
	{
		bossPuzzlesTrackers[i]->checkWin();
	}

	//Remove items from drawbuffer
	drawBuffer.clear();
	drawBuffer.push_back (SceneFader::singletonInstance->sprite);
	for (sf::Drawable * drawable : hud->getAllDrawables())
	{
		drawBuffer.push_back(drawable);
	}
	drawBuffer.push_back (_hudOverlay);
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


void Level::renderBackground (sf::RenderWindow* pWindow)
{
	Level::singletonInstance->_background->render (pWindow);
}
void Level::render (sf::RenderWindow* pWindow)
{
	for (int i = 0, size = Level::singletonInstance->drawBuffer.size(); i < size; i++)
	{
		if (Level::singletonInstance->drawBuffer [i] == nullptr)
		{
			continue;
		}
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
			if (!_reloading)
			{
				hud->setState(1);
			}
		}
		else
		{
			_isHub = false;
			levelCompleted = false;
			if (!_reloading)
			{
				hud->setState(2);
			}
		}
	}

	if (map != nullptr)
	{
		delete map;
	}

	if (_levelNumber == 0)
	{
		map = LevelImporter::ReadFile ("Hub.tmx");
	}
	else if (_levelNumber == _bossLevelNumber)
	{
		JCPPEngine::SoundManager::StopMusicLoop (0);
		map = LevelImporter::ReadFile ("BossLevelAlternativeCamera.tmx");
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
	ProgressTracker * bossPuzzleTracker = nullptr;
	LitMaterial* mat = nullptr;
	float rotation;
	glm::vec3 position;

	//Build all base tiles
	for (int y = 0; y < map->height; y++)
	{
		for (int x = 0; x < map->width; x++)
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
					position = glm::vec3 (TILESIZE * x, -0.05f, TILESIZE * y);
					rotation = -90.0f * (map->baseTiles[x][y]-9);
					temp = new GameObject ();
					temp2 = new GameObject ("Pipe1.obj");
					temp2->setMaterial (new LitMaterial ("Pipe1.png"));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation), glm::vec3 (0, 1, 0));
					temp2->translate (glm::vec3 (-1, 1.55f, 0));
					temp2->setParent (transparencyLayer2);
					temp2 = new GameObject ("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation), glm::vec3 (0, 1, 0));
					temp2->translate (glm::vec3 (-1, 2.55f, 0));
					temp2->setParent (transparencyLayer3);
					temp->setParent (this);
					temp->translate (position);
					temp->rotate (glm::radians (rotation), glm::vec3 (0, 1, 0));
					map->baseTiles [x] [y] = (int)temp;
					break;
				case 13:
				case 14:
					//Wall pipe 2
					position = glm::vec3 (TILESIZE * x, -0.05f, TILESIZE * y);
					rotation = -90.0f * (map->baseTiles[x][y]-13);
					temp = new GameObject ();
					temp2 = new GameObject ("Pipe2.obj");
					temp2->setMaterial (new LitMaterial ("Pipe2.png"));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation), glm::vec3 (0,1,0));
					temp2->translate (glm::vec3 (-1, 1.55f, 0));
					temp2->setParent (transparencyLayer2);
					temp2 = new GameObject ("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation -90.0f), glm::vec3 (0, 1, 0));
					temp2->translate (glm::vec3 (-1, 1.55f, 0));
					temp2->setParent (transparencyLayer2);
					temp2 = new GameObject ("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation), glm::vec3 (0,1,0));
					temp2->translate (glm::vec3 (-1, 2.55f, 0));
					temp2->setParent (transparencyLayer3);
					temp2 = new GameObject ("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation -90.0f), glm::vec3 (0, 1, 0));
					temp2->translate (glm::vec3 (-1, 2.55f, 0));
					temp2->setParent (transparencyLayer3);
					temp->setParent (this);
					temp->translate (position);
					temp->rotate (glm::radians (rotation), glm::vec3 (0, 1, 0));
					map->baseTiles [x] [y] = (int) temp;
					break;
				case 17:
				case 18:
					//Wall corner
					position = glm::vec3(TILESIZE * x, 0.5f, TILESIZE * y);
					rotation = -90.0f * (map->baseTiles[x][y] - 17);
					temp = new GameObject();
					temp2 = new GameObject("Wall.obj");
					temp2->setMaterial(new LitMaterial("Wall.png"));
					temp2->setParent(temp);
					temp2 = new GameObject("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation), glm::vec3 (0,1,0));
					temp2->translate(glm::vec3(0, 1, 0));
					temp2->setParent(transparencyLayer2);
					temp2 = new GameObject("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation), glm::vec3 (0,1,0));
					temp2->translate(glm::vec3(0, 2, 0));
					temp2->setParent(transparencyLayer3);
					temp2 = new GameObject("Wall.obj");
					temp2->setMaterial(new LitMaterial("Wall.png"));
					temp2->translate(glm::vec3(1, 0, -1));
					temp2->rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0));
					temp2->setParent(temp);
					temp2 = new GameObject("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate(glm::radians(rotation-90.0f), glm::vec3(0, 1, 0));
					temp2->translate(glm::vec3(-1, 1, -1));
					temp2->setParent(transparencyLayer2);
					temp2 = new GameObject("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate(glm::radians(rotation-90.0f), glm::vec3(0, 1, 0));
					temp2->translate(glm::vec3(-1, 2, -1));
					temp2->setParent(transparencyLayer3);
					temp->setParent(this);
					temp->translate(position);
					temp->rotate(glm::radians(rotation), glm::vec3(0, 1, 0));
					//This tile should not be walkable
					_nonWalkables.push_back(temp);
					map->baseTiles[x][y] = (int)nullptr;
					break;
				case 19:
				case 20:
					//Wall corner
					position = glm::vec3(TILESIZE * x, 0.5f, TILESIZE * y);
					rotation = -90.0f * (map->baseTiles[x][y] - 19);
					temp = new GameObject();
					temp2 = new GameObject("Wall.obj");
					temp2->setMaterial(new LitMaterial("Wall.png"));
					temp2->translate (glm::vec3 (-1, 0, 1));
					temp2->setParent(temp);
					temp2 = new GameObject("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation), glm::vec3 (0,1,0));
					temp2->translate(glm::vec3(-1, 1, 1));
					temp2->setParent(transparencyLayer2);
					temp2 = new GameObject("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation), glm::vec3 (0,1,0));
					temp2->translate(glm::vec3(-1, 2, 1));
					temp2->setParent(transparencyLayer3);
					temp2 = new GameObject("Wall.obj");
					temp2->setMaterial(new LitMaterial("Wall.png"));
					temp2->rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0));
					temp2->setParent(temp);
					temp2 = new GameObject("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate(glm::radians(rotation-90.0f), glm::vec3(0, 1, 0));
					temp2->translate(glm::vec3(0, 1, 0));
					temp2->setParent(transparencyLayer2);
					temp2 = new GameObject("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate(glm::radians(rotation-90.0f), glm::vec3(0, 1, 0));
					temp2->translate(glm::vec3(0, 2, 0));
					temp2->setParent(transparencyLayer3);
					temp->setParent(this);
					temp->translate(position);
					temp->rotate(glm::radians(rotation), glm::vec3(0, 1, 0));
					//This tile should not be walkable
					_nonWalkables.push_back(temp);
					map->baseTiles[x][y] = (int)nullptr;
					break;
				case 21:
				case 22:
				case 23:
					//Wall
					position = glm::vec3 (TILESIZE * x, 0.5f, TILESIZE * y);
					rotation = -90.0f * (map->baseTiles[x][y]-21);
					temp = new GameObject ();
					temp2 = new GameObject ("Wall.obj");
					temp2->setMaterial (new LitMaterial ("Wall.png"));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation), glm::vec3 (0,1,0));
					temp2->translate (glm::vec3 (0, 1, 0));
					temp2->setParent (transparencyLayer2);
					temp2 = new GameObject ("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation), glm::vec3 (0,1,0));
					temp2->translate (glm::vec3 (0, 2, 0));
					temp2->setParent (transparencyLayer3);
					temp->setParent (this);
					temp->translate (position);
					temp->rotate (glm::radians (rotation), glm::vec3 (0, 1, 0));
					//This tile should not be walkable
					_nonWalkables.push_back (temp);
					map->baseTiles [x] [y] = (int)nullptr;
					break;
				case 25:
				case 26:
					//Wall pipe 3
					position  = glm::vec3 (TILESIZE * x, -0.05f, TILESIZE * y);
					rotation = -90.0f * (map->baseTiles[x][y]-25);
					temp = new GameObject ();
					temp2 = new GameObject ("Pipe3.obj");
					temp2->setMaterial (new LitMaterial ("Pipe3.png"));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation), glm::vec3 (0, 1, 0));
					temp2->translate (glm::vec3 (-1, 1.55f, 0));
					temp2->setParent (transparencyLayer2);
					temp2 = new GameObject ("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation-90.0f), glm::vec3 (0, 1, 0));
					temp2->translate (glm::vec3 (-1, 1.55f, 0));
					temp2->setParent (transparencyLayer2);
					temp2 = new GameObject ("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation), glm::vec3 (0, 1, 0));
					temp2->translate (glm::vec3 (-1, 2.55f, 0));
					temp2->setParent (transparencyLayer3);
					temp2 = new GameObject ("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation-90.0f), glm::vec3 (0, 1, 0));
					temp2->translate (glm::vec3 (-1, 2.55f, 0));
					temp2->setParent (transparencyLayer3);
					temp->setParent (this);
					temp->translate (position);
					temp->rotate (glm::radians (rotation), glm::vec3 (0, 1, 0));
					map->baseTiles [x] [y] = (int) temp;
					break;
				case 29:
				case 30:
					//Wall pipe 4
					position = glm::vec3 (TILESIZE * x, -0.05f, TILESIZE * y);
					rotation = -90.0f * (map->baseTiles[x][y]-29);
					temp = new GameObject ();
					temp2 = new GameObject ("Pipe4.obj");
					temp2->setMaterial (new LitMaterial ("Pipe4.png"));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation), glm::vec3 (0, 1, 0));
					temp2->translate (glm::vec3 (-1, 1.55f, 0));
					temp2->setParent (transparencyLayer2);
					temp2 = new GameObject ("WallPipeL.obj");
					mat = new LitMaterial ("WallPipeL.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation-90.0f), glm::vec3 (0, 1, 0));
					temp2->translate (glm::vec3 (-1, 1.55f, 0));
					temp2->setParent (transparencyLayer2);
					temp2 = new GameObject ("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation), glm::vec3 (0, 1, 0));
					temp2->translate (glm::vec3 (-1, 2.55f, 0));
					temp2->setParent (transparencyLayer3);
					temp2 = new GameObject ("WallPipeL.obj");
					mat = new LitMaterial ("WallPipeL.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation-90.0f), glm::vec3 (0, 1, 0));
					temp2->translate (glm::vec3 (-1, 2.55f, 0));
					temp2->setParent (transparencyLayer3);
					temp->setParent (this);
					temp->translate (position);
					temp->rotate (glm::radians (rotation), glm::vec3 (0, 1, 0));
					map->baseTiles [x] [y] = (int) temp;
					break;
				case 31:
					//Wall with plant
					position = glm::vec3 (TILESIZE * x, -0.05f, TILESIZE * y);
					rotation = -90.0f;
					temp = new GameObject ();
					temp2 = new GameObject ("Plant.obj");
					temp2->setMaterial (new LitMaterial ("Plant.png"));
					temp2->setParent (temp);
					temp2 = new GameObject ("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation), glm::vec3 (0, 1, 0));
					temp2->translate (glm::vec3 (-1, 1.55f, 0));
					temp2->setParent (transparencyLayer2);
					temp2 = new GameObject ("Wall.obj");
					mat = new LitMaterial ("Wall.png");
					mat->SetFade (true, _fadeMin, _fadeMax, _distribution);
					temp2->setMaterial (mat);
					temp2->translate (position);
					temp2->rotate (glm::radians (rotation), glm::vec3 (0, 1, 0));
					temp2->translate (glm::vec3 (-1, 2.55f, 0));
					temp2->setParent (transparencyLayer3);
					temp->setParent (this);
					temp->translate (position);
					temp->rotate (glm::radians (rotation), glm::vec3 (0, 1, 0));
					map->baseTiles [x] [y] = (int)temp;
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
	for (int y = 0; y < map->height; y++)
	{
		for (int x = 0; x < map->width; x++)
		{
			switch (map->objectTiles [x] [y])
			{
				case 33:
					//Player
					temp = new Player (x, y, progressTracker, _playerSkin);
					temp->setParent (transparencyLayer1);
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
					map->objectTiles [x] [y] = (int)nullptr;
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
					//ServerRack
					temp = new GameObject ("ServerRack.obj");
					temp->setMaterial (new LitMaterial ("ServerRack.png"));
					temp->setParent (this);
					temp->translate(glm::vec3(TILESIZE * x, 0, TILESIZE * y));
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
					temp = new Fan(x, y, 1, 0, true);
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
				case 53: // interior wall corner
				case 54:
				case 55:
				case 56:
					temp = new GameObject("WallInteriorCorner.obj");
					temp->setMaterial(new LitMaterial("InteriorWall.png"));
					temp->setParent(this);
					temp->translate(glm::vec3(TILESIZE * x, 0, TILESIZE * y));
					temp->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
					temp->rotate(glm::radians(-90.0f)*(map->objectTiles[x][y] - 53), glm::vec3(0, 1, 0));
					map->objectTiles[x][y] = (int)temp;
					break;
				case 57: // interior wall stop
				case 58:
				case 59:
				case 60:
					temp = new GameObject("WallInteriorStop.obj");
					temp->setMaterial(new LitMaterial("InteriorWall.png"));
					temp->setParent(this);
					temp->translate(glm::vec3(TILESIZE * x, 0, TILESIZE * y));
					temp->rotate(glm::radians(90.0f), glm::vec3(0, 1, 0));
					temp->rotate(glm::radians(-90.0f)*(map->objectTiles[x][y] - 57), glm::vec3(0, 1, 0));
					map->objectTiles[x][y] = (int)temp;
					break;
				case 61: // interior wall hallway
				case 62:
					temp = new GameObject("WallInteriorHallway.obj");
					temp->setMaterial(new LitMaterial("InteriorWall.png"));
					temp->setParent(this);
					temp->translate(glm::vec3(TILESIZE * x, 0, TILESIZE * y));
					temp->rotate(glm::radians(-90.0f)*(map->objectTiles[x][y] - 61), glm::vec3(0, 1, 0));
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
	std::vector<BossCameraAnchor*> anchors;
	for (int i = 0, size = map->xmlObjects.size (); i < size; i ++)
	{
		object = map->xmlObjects [i];
		switch (object->type)
		{
			case 66:
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
					case 3:
						World::singletonInstance->getMainCamera()->setBehaviour(new BossCameraBehaviour());
						World::singletonInstance->getMainCamera()->setWorldPosition(glm::vec3(object->x, 6, object->z));
						break;
					default:
						break;
				}
				break;
			case 68:
				//Button: property = target type & x & y of the object it (de)activates
				if (std::stoi (object->properties [0]) > 0)
				{
					//Targettype = object tile
					temp = new Button (object->x, object->z, (ButtonTarget*)map->objectTiles [std::stoi (object->properties [1])] [std::stoi (object->properties [2])]);
				}
				else
				{
					//Targettype = base tile
					temp = new Button (object->x, object->z, (ButtonTarget*)map->baseTiles [std::stoi (object->properties [1])] [std::stoi (object->properties [2])]);
				}
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
			case 67:
				//Collectable: property = name & stay & dialog
				if (object->properties.size () > 1)
				{
					if (std::stoi (object->properties [1]) > 0)//Create tutorial computer
					{
						temp = new Collectable (object->x, object->z, object->properties [0], object->properties [2], true, _levelNumber == 1);
					}
					else//Create dialog pickup
					{
						temp = new Collectable (object->x, object->z, object->properties [0], object->properties [2], false, _levelNumber == 3);
					}
				}
				else//Create quest item
				{
					temp = new Collectable (object->x, object->z, object->properties [0], true);
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
			case 65:
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
			case 69: //Socket up
				if (std::stoi (object->properties [0]) > 0)
				{
					//Targettype = object tile
					temp = new Socket (object->x, object->z, 1, (ButtonTarget*)map->objectTiles [std::stoi (object->properties [1])] [std::stoi (object->properties [2])]);
				}
				else
				{
					//Targettype = base tile
					temp = new Socket (object->x, object->z, 1, (ButtonTarget*)map->baseTiles [std::stoi (object->properties [1])] [std::stoi (object->properties [2])]);
				}
				temp->setParent(this);
				if (map->objectTiles[object->x][object->z] != (int)nullptr)
				{
					GameObject* block = (GameObject*)map->objectTiles[object->x][object->z];
					block->setParent(nullptr);
					delete block;
				}
				map->objectTiles[object->x][object->z] = (int)temp;
				break;
			case 70: //Socket down
				if (std::stoi (object->properties [0]) > 0)
				{
					//Targettype = object tile
					temp = new Socket (object->x, object->z, 2, (ButtonTarget*)map->objectTiles [std::stoi (object->properties [1])] [std::stoi (object->properties [2])]);
				}
				else
				{
					//Targettype = base tile
					temp = new Socket (object->x, object->z, 2, (ButtonTarget*)map->baseTiles [std::stoi (object->properties [1])] [std::stoi (object->properties [2])]);
				}
				temp->setParent(this);
				if (map->objectTiles[object->x][object->z] != (int)nullptr)
				{
					GameObject* block = (GameObject*)map->objectTiles[object->x][object->z];
					block->setParent(nullptr);
					delete block;
				}
				map->objectTiles[object->x][object->z] = (int)temp;
				break;
			case 71: //Socket right
				if (std::stoi (object->properties [0]) > 0)
				{
					//Targettype = object tile
					temp = new Socket (object->x, object->z, 3, (ButtonTarget*)map->objectTiles [std::stoi (object->properties [1])] [std::stoi (object->properties [2])]);
				}
				else
				{
					//Targettype = base tile
					temp = new Socket (object->x, object->z, 3, (ButtonTarget*)map->baseTiles [std::stoi (object->properties [1])] [std::stoi (object->properties [2])]);
				}
				temp->setParent(this);
				if (map->objectTiles[object->x][object->z] != (int)nullptr)
				{
					GameObject* block = (GameObject*)map->objectTiles[object->x][object->z];
					block->setParent(nullptr);
					delete block;
				}
				map->objectTiles[object->x][object->z] = (int)temp;
				break;
			case 72: //Socket left
				if (std::stoi (object->properties [0]) > 0)
				{
					//Targettype = object tile
					temp = new Socket (object->x, object->z, 4, (ButtonTarget*)map->objectTiles [std::stoi (object->properties [1])] [std::stoi (object->properties [2])]);
				}
				else
				{
					//Targettype = base tile
					temp = new Socket (object->x, object->z, 4, (ButtonTarget*)map->baseTiles [std::stoi (object->properties [1])] [std::stoi (object->properties [2])]);
				}
				temp->setParent(this);
				if (map->objectTiles[object->x][object->z] != (int)nullptr)
				{
					GameObject* block = (GameObject*)map->objectTiles[object->x][object->z];
					block->setParent(nullptr);
					delete block;
				}
				map->objectTiles[object->x][object->z] = (int)temp;
				break;
			case 73: //hints
				temp = new Hint(object->x, object->z, object->properties[0]);
				temp->setParent(this);
				hints.push_back((Hint*)temp);
				break;
			case 74: //progress tracker for mini puzzle in boss level
				bossPuzzleTracker = new ProgressTracker();
				if (dynamic_cast <Mirror*> ((GameObject*)map->baseTiles[object->x][object->z]) != nullptr)
				{
					bossPuzzleTracker->_targetMirror = static_cast <Mirror*> ((GameObject*)map->baseTiles[object->x][object->z]);
				}
				else
				{
					std::cout <<"cant find mirror" << endl;
				}
				for (int i = 0; i < object->properties.size(); i += 2)
				{
					if (i + 1 < object->properties.size())
					{
						BoxSpot * boxSpot = dynamic_cast <BoxSpot*> ((GameObject*)map->baseTiles[std::stoi(object->properties[i])][std::stoi(object->properties[i + 1])]);
						if (boxSpot != nullptr)
						{
							bossPuzzleTracker->boxSpots.push_back(boxSpot);
						}
						else
						{
							std::cout << "cant find boxspot "<< i/2 <<"at"<<object->x<<", "<<object->z<<std::endl;
						}
					}
				}
				bossPuzzlesTrackers.push_back(bossPuzzleTracker);
				break;
			case 75:
				anchors.push_back(new BossCameraAnchor(object->x*Level::TILESIZE, std::stoi(object->properties[1]),object->z*Level::TILESIZE, //pos
					std::stoi(object->properties[3]), std::stoi(object->properties[2]), //rangeX
					std::stoi(object->properties[5]), std::stoi(object->properties[4]), //rangeY
					std::stoi(object->properties[0]) //controlsMode
					));
				//sdf
				break;
			case 76:
				temp = new Light (glm::vec3(1, 1, 0.8f), std::stof (object->properties [1])*10, glm::vec3 (object->x, 2.5f, object->z), glm::radians (std::stof (object->properties [0])), glm::vec2 (0.25f, 0.75f));
				temp->setParent (transparencyLayer3);
				break;
			default:
				break;
		}
	}

	for (BossCameraAnchor * anchor : anchors)
	{
		((BossCameraBehaviour*)World::singletonInstance->getMainCamera()->getBehaviour())->anchors.push_back(anchor);
	}
	anchors.clear();

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

	//Delete transparent objects
	transparencyLayer1->removeAll ();
	transparencyLayer2->removeAll ();
	transparencyLayer3->removeAll ();

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

	//Delete bossPuzzleTrackers
	for (ProgressTracker * bossPuzzleTracker : bossPuzzlesTrackers)
	{
		delete bossPuzzleTracker;
	}
	bossPuzzlesTrackers.clear();

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

//Fade to endscreen
void Level::startFade ()
{
	_fading = true;
}