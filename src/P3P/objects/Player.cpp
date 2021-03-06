#include "P3P/objects/Player.hpp"
#include <P3P/objects/Npc.hpp>
#include <P3P/Level.hpp>
#include <P3P/Moveable.hpp>
#include <P3P/objects/Door.hpp>
#include <P3P/objects/Collectable.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <JCPPEngine/KeyEvent.hpp>
#include <JCPPEngine/InputManager.hpp>
#include <JCPPEngine/Random.hpp>
#include <JCPPEngine/SoundManager.hpp>
#include <mge/materials/TextureMaterial.hpp>
#include <mge/core/AbstractGame.hpp>


//Static variables
Player* Player::singletonInstance = nullptr;


//////////////////////////////|	ANIMATION
//Function to be called when animations are stopped. It has to be in global space, or we have to pass our object type as well.
void stopFunctionPlayer (int pAnimIndex, GameObject* pOwner)
{
	Player* player = (Player*)pOwner;
	switch (pAnimIndex)
	{
		case 0:
			player->_noMove = false;
			player->setWorldPosition (glm::vec3 (player->_currentTile [0] * Level::TILESIZE, 0, player->_currentTile [1] * Level::TILESIZE));
			if (player->_funcOwner != nullptr)
			{
				GameObject* tempOwner = player->_funcOwner;
				void (*tempFunc) (int, GameObject*) = player->_stopFunc;
				player->_funcOwner = nullptr;
				player->_stopFunc = nullptr;
				tempFunc (pAnimIndex, tempOwner);
			}
			else if (player->_questFinished)
			{
				player->_noMove = true;
				player->_questFinished = false;
				switch ((int)player->_modelOrientation[1])
				{
				case 0:
					player->_rotationAnimator->playAnimation(13, false, &stopFunctionPlayer, player);
					break;
				case 180:
					player->_rotationAnimator->playAnimation(14, false, &stopFunctionPlayer, player);
					break;
				case 90:
					player->_rotationAnimator->playAnimation(15, false, &stopFunctionPlayer, player);
					break;
				case -90:
					player->_rotationAnimator->playAnimation(16, false, &stopFunctionPlayer, player);
					break;
				}
			}
			break;
		case 1:
			Level::singletonInstance->reloadLevel ();
			break;
		case 12:
			player->_noMove = false;
			break;
		case 13:
		case 14:
		case 15:
		case 16:
			player->_questFinished = false;
			player->_noMove = false;
			break;
		default:
			break;
	}
}
void stopFunctionPlayerRotation (int pAnimIndex, GameObject* pOwner)
{
	Player* player = (Player*)pOwner;
	if (player->movePlayer(player->_movementToComplete[0], player->_movementToComplete[1], true))
	{
		player->_noMove = true;
	}
	else
	{
		player->_noMove = false;
	}
}

//Stop any playing animations
void Player::stopAnimation ()
{
	_wheelAnimator->stopAnimation ();
	_baseAnimator->stopAnimation ();
}

//////////////////////////////|	INSTANCE MANAGEMENT
//Constructor
Player::Player (int pX, int pZ, ProgressTracker* pProgressTracker, int pSkin) : GameObject ()
{
	if (singletonInstance != nullptr)
	{
		singletonInstance->setParent (nullptr);
		delete singletonInstance;
	}
	singletonInstance = this;
        _progressTracker = pProgressTracker;
	registerForEvent (JCPPEngine::Event::EventType::KeyDown);

	//set up model
	_model = new GameObject ();
	_model->translate (glm::vec3 (0, 0.5f, 0));
	_model->rotate (glm::radians (180.0f), glm::vec3 (0, 1, 0));
	_modelOrientation.y = 180;
	_model->setParent (this);//By making the model a child object, we can rotate it without it affecting translation directions.
	GameObject* wheelModel = new GameObject ("Playerwheel.obj");
	wheelModel->setParent (_model);
	wheelModel->setMaterial (new LitMaterial ("PlayerWheel.png"));
	_shadow = new GameObject ("ShadowPlane.obj");
	_shadow->setMaterial (new TextureMaterial ("PlayerShadow.png"));
	_shadow->translate (glm::vec3 (0, -0.43f, 0));
	_shadow->setParent (wheelModel);
	GameObject* baseRotation = new GameObject ();
	baseRotation->rotate (glm::radians (180.0f), glm::vec3 (0, 1, 0));
	baseRotation->setParent (_model);
	_baseModel = new GameObject ("PlayerBase.obj");
	_baseModel->setParent (baseRotation);
	_baseModel->setMaterial (new LitMaterial ("PlayerBase"+to_string (pSkin)+".png"));
	_wheelAnimator = new AnimationBehaviour ({ "PlayerWheel.txt", "PlayerWheelDestruct.txt" });
	_baseAnimator = new AnimationBehaviour ({ "PlayerBase.txt", "PlayerBaseDestruct.txt" });
	wheelModel->setBehaviour (_wheelAnimator);
	_baseModel->setBehaviour (_baseAnimator);
	_rotationAnimator = new AnimationBehaviour
	({
		"PlayerRotationUpLeft.txt","PlayerRotationUpRight.txt","PlayerRotationUpDown.txt",
		"PlayerRotationLeftUp.txt","PlayerRotationLeftRight.txt","PlayerRotationLeftDown.txt",
		"PlayerRotationRightUp.txt","PlayerRotationRightLeft.txt","PlayerRotationRightDown.txt",
		"PlayerRotationDownUp.txt","PlayerRotationDownLeft.txt","PlayerRotationDownRight.txt",
		"PlayerSpawn.txt", 
		"PlayerFunUp.txt","PlayerFunDown.txt","PlayerFunRight.txt","PlayerFunLeft.txt"
	});
	_model->setBehaviour (_rotationAnimator);

	translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
	_currentTile [0] = pX;
	_currentTile [1] = pZ;
	_oldTile [0] = _currentTile [0];
	_oldTile [1] = _currentTile [1];
	_noMove = true;
	_rotationAnimator->playAnimation(12, false, &stopFunctionPlayer, this);
	JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/Spawn.wav")));
}

//Destructor
Player::~Player ()
{
	singletonInstance->setParent (nullptr);
	singletonInstance = nullptr;
	unregisterForEvent (JCPPEngine::Event::EventType::KeyDown);
	if (_progressTracker != nullptr)
	{
		delete _progressTracker;
	}
	inventory.clear ();
	_wheelAnimator = nullptr;
	_baseAnimator = nullptr;
	Level::map->objectTiles [_currentTile [0]] [_currentTile [1]] = (int)nullptr;
	GameObject::~GameObject ();
}


//////////////////////////////|	UPDATING FUNCTIONS
//Move the player by a given amount of tiles
bool Player::movePlayer (int pX, int pZ, bool pAnimate)
{
    //update position
    _oldTile [0] = _currentTile [0];
    _oldTile [1] = _currentTile [1];
    _currentTile [0] += pX;
    _currentTile [1] += pZ;

    if (Level::map->objectTiles [_currentTile [0]] [_currentTile [1]] != (int)nullptr)
    {
        //Check if the new position contains a special object
        Moveable* box = dynamic_cast <Moveable*> ((GameObject*)Level::map->objectTiles [_currentTile [0]] [_currentTile [1]]);
        Door* door = dynamic_cast <Door*> ((GameObject*)Level::map->objectTiles [_currentTile [0]] [_currentTile [1]]);
	Collectable* collectable = dynamic_cast <Collectable*> ((GameObject*)Level::map->objectTiles [_currentTile [0]] [_currentTile [1]]);
	if (Npc::singletonInstance != nullptr && ((int)Npc::singletonInstance) == ((int)Level::map->objectTiles [_currentTile [0]] [_currentTile [1]]))
	{
		//we cannot move into the Npc's space
		_currentTile [0] = _oldTile [0];
		_currentTile [1] = _oldTile [1];
		//Start talking to npc
		Npc::singletonInstance->talk ();
		return false;
	}
	else if (door != nullptr)//The new position contains a door
	{
		door->enter ();
		//we cannot enter the door's space
		_currentTile [0] = _oldTile [0];
		_currentTile [1] = _oldTile [1];
		return false;
	}
	else if (box != nullptr)//The new position contains a box
	{
		int newBoxTile [2] = { _currentTile [0], _currentTile [1] };
		newBoxTile [0] += pX;
		newBoxTile [1] += pZ;
		//Check if we can move the box
		if
		(
			newBoxTile [0] >= 0 && newBoxTile [0] < Level::map->width &&
			newBoxTile [1] >= 0 && newBoxTile [1] < Level::map->height &&
			Level::map->baseTiles [newBoxTile [0]] [newBoxTile [1]] != (int)nullptr &&
			Level::map->objectTiles [newBoxTile [0]] [newBoxTile [1]] == (int)nullptr
		)
		{
			//we can move the box
			if (!box->move (pX, pZ))
			{
				//the object is locked into place
				_currentTile [0] = _oldTile [0];
				_currentTile [1] = _oldTile [1];
				return false;
			}
		}
		else
		{
			JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/Thump.wav")));
			//we cannot move the box
			_currentTile [0] = _oldTile [0];
			_currentTile [1] = _oldTile [1];
			return false;
		}
	}
	else if (collectable != nullptr)//The new position contains a collectable
	{
		//Collect item and move into its space if possible
		if (collectable->collect (_oldTile [0], _oldTile [1]))
		{
			//we cannot move into the occupied space
			_currentTile [0] = _oldTile [0];
			_currentTile [1] = _oldTile [1];
			return false;
		}
		else if (collectable->isQuestItem ())
		{
			_questFinished = true;//When we finish the 'walk' animation, the fun animation will start.
		}
	}
	else
	{
		//we cannot move into the occupied space
		_currentTile [0] = _oldTile [0];
		_currentTile [1] = _oldTile [1];
		return false;
	}
    }

    //update the object array
    Level::map->objectTiles [_currentTile [0]] [_currentTile [1]] = (int)this;
    Level::map->objectTiles [_oldTile [0]] [_oldTile [1]] = (int)nullptr;

    //move
    if (pAnimate)
    {
	_wheelAnimator->playAnimation (0);
	_baseAnimator->playAnimation (0, false, &stopFunctionPlayer, this);
    }
    else
    {
        translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
    }
    sf::Sound* sound = new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/PlayerMoving.wav"));
    sound->setPitch (1 + (JCPPEngine::Random::Value () - 0.5f) * 0.5f);
    JCPPEngine::SoundManager::PlaySound (sound);

    //check for win
    if (_progressTracker->checkWin () && !Level::singletonInstance->levelCompleted)
    {
        Level::singletonInstance->levelCompleted = true;
    }
	Stats::singletonInstance->data.metersWalked++;
	Stats::singletonInstance->refreshText();
    return true;
}
//Move the player by a given amount of tiles, then call the given stop function
bool Player::movePlayer (int pX, int pZ, bool pAnimate, void (*pFuncPtr) (int, GameObject*), GameObject* pFuncOwner)
{
    //update position
    _oldTile [0] = _currentTile [0];
    _oldTile [1] = _currentTile [1];
    _currentTile [0] += pX;
    _currentTile [1] += pZ;

    if (Level::map->objectTiles [_currentTile [0]] [_currentTile [1]] != (int)nullptr)
    {
        //Check if the new position contains a special object
        Moveable* box = dynamic_cast <Moveable*> ((GameObject*)Level::map->objectTiles [_currentTile [0]] [_currentTile [1]]);
        Door* door = dynamic_cast <Door*> ((GameObject*)Level::map->objectTiles [_currentTile [0]] [_currentTile [1]]);
	Collectable* collectable = dynamic_cast <Collectable*> ((GameObject*)Level::map->objectTiles [_currentTile [0]] [_currentTile [1]]);
	if (Npc::singletonInstance != nullptr && ((int)Npc::singletonInstance) == ((int)Level::map->objectTiles [_currentTile [0]] [_currentTile [1]]))
	{
		//we cannot move into the Npc's space
		_currentTile [0] = _oldTile [0];
		_currentTile [1] = _oldTile [1];
		//Start talking to npc
		Npc::singletonInstance->talk ();
		return false;
	}
	else if (door != nullptr)//The new position contains a door
	{
		door->enter ();
		//we cannot enter the door's space
		_currentTile [0] = _oldTile [0];
		_currentTile [1] = _oldTile [1];
		return false;
	}
	else if (box != nullptr)//The new position contains a box
	{
		int newBoxTile [2] = { _currentTile [0], _currentTile [1] };
		newBoxTile [0] += pX;
		newBoxTile [1] += pZ;
		//Check if we can move the box
		if
		(
			newBoxTile [0] >= 0 && newBoxTile [0] < Level::map->width &&
			newBoxTile [1] >= 0 && newBoxTile [1] < Level::map->height &&
			Level::map->baseTiles [newBoxTile [0]] [newBoxTile [1]] != (int)nullptr &&
			Level::map->objectTiles [newBoxTile [0]] [newBoxTile [1]] == (int)nullptr
		)
		{
			//we can move the box
			if (!box->move (pX, pZ))
			{
				//the object is locked into place
				_currentTile [0] = _oldTile [0];
				_currentTile [1] = _oldTile [1];
				return false;
			}
		}
		else
		{
			//we cannot move the box
			_currentTile [0] = _oldTile [0];
			_currentTile [1] = _oldTile [1];
			return false;
		}
	}
	else if (collectable != nullptr)//The new position contains a collectable
	{
		//Collect item and move into its space if possible
		if (collectable->collect (_oldTile [0], _oldTile [1]))
		{
			//we cannot move into the occupied space
			_currentTile [0] = _oldTile [0];
			_currentTile [1] = _oldTile [1];
			return false;
		}
		else if (collectable->isQuestItem ())
		{
			_questFinished = true;//When we finish the 'walk' animation, the fun animation will start.
		}
	}
	else
	{
		//we cannot move into the occupied space
		_currentTile [0] = _oldTile [0];
		_currentTile [1] = _oldTile [1];
		return false;
	}
    }

    //update the object array
    Level::map->objectTiles [_currentTile [0]] [_currentTile [1]] = (int)this;
    Level::map->objectTiles [_oldTile [0]] [_oldTile [1]] = (int)nullptr;

    //move
    if (pAnimate)
    {
	_funcOwner = pFuncOwner;
	_stopFunc = pFuncPtr;
	_wheelAnimator->playAnimation (0);
	_baseAnimator->playAnimation (0, false, &stopFunctionPlayer, this);
    }
    else
    {
        translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
    }
    sf::Sound* sound = new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/PlayerMoving.wav"));
    sound->setPitch (1 + (JCPPEngine::Random::Value () - 0.5f) * 0.5f);
    JCPPEngine::SoundManager::PlaySound (sound);

    //check for win
    if (_progressTracker->checkWin () && !Level::singletonInstance->levelCompleted)
    {
        Level::singletonInstance->levelCompleted = true;
    }
	Stats::singletonInstance->data.metersWalked++;
	Stats::singletonInstance->refreshText();
    return true;
}


//////////////////////////////|	EVENT-BASED FUNCTIONS
//Cause the player to die, and reload the level
void Player::die ()
{
	if (_dead)
	{
		return;
	}
	Level::singletonInstance->hud->disable();
	Stats::singletonInstance->data.deathCount++;
	Stats::singletonInstance->refreshText();
	_noMove = true;
	_dead = true;
	_shadow->setParent (nullptr);
	delete _shadow;
	_baseAnimator->stopAnimation ();
	_wheelAnimator->stopAnimation ();
	_wheelAnimator->playAnimation (1, false, false);
	_baseAnimator->playAnimation (1, false, false, &stopFunctionPlayer, this);
	JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/Death.wav")));
}

//Process input events
void Player::ProcessEvent (JCPPEngine::Event* pEvent)
{
	JCPPEngine::KeyEvent* keyDownEvent = (JCPPEngine::KeyEvent*)pEvent;
	if (keyDownEvent == nullptr || keyDownEvent->keyState () != JCPPEngine::InputManager::KEY_DOWN || _noMove || blockMovement)
	{
		return;
	}

	int movement [2] = { 0, 0 };
	switch (keyDownEvent->key ())
	{
		case sf::Keyboard::Key::Up:
		case sf::Keyboard::Key::W:
			movement [1] --;
			break;
		case sf::Keyboard::Key::Down:
		case sf::Keyboard::Key::S:
			movement [1] ++;
			break;
		case sf::Keyboard::Key::Left:
		case sf::Keyboard::Key::A:
			movement [0] --;
			break;
		case sf::Keyboard::Key::Right:
		case sf::Keyboard::Key::D:
			movement [0] ++;
			break;
		case sf::Keyboard::Key::R:
			die ();
			return;
		case sf::Keyboard::Key::P:
			blockMovement = true;
			AbstractGame::showCursor (true);
			Hud::singletonInstance->pressButton(2);
			return;
		case sf::Keyboard::Key::H:
			Hud::singletonInstance->pressButton(1);
			return;
		case sf::Keyboard::Key::F:

			_noMove = true;
			switch ((int)_modelOrientation[1])
			{
			case 0:
				_rotationAnimator->playAnimation(13, false, &stopFunctionPlayer, this);
				break;
			case 180:
				_rotationAnimator->playAnimation(14, false, &stopFunctionPlayer, this);
				break;
			case 90:
				_rotationAnimator->playAnimation(15, false, &stopFunctionPlayer, this);
				break;
			case -90:
				_rotationAnimator->playAnimation(16, false, &stopFunctionPlayer, this);
				break;
			}
			return;
		default:
			break;
	}
	
	//rotate movement based on controlsMode
	float angle = 0;
	switch (controlsMode)
	{
	case 2:
		angle = -3.14f/2;
		break;
	case 3:
		angle = 3.14f;
		break;
	case 4:
		angle = 3.14f / 2;
		break;
	default:
		break;
	}
	float cosX = cos(angle) * movement[0];
	float sinX = sin(angle) * movement[0];
	float cosY = cos(angle) * movement[1];
	float sinY = sin(angle) * movement[1];
	movement[0] = round(cosX - sinY);
	movement[1] = round(sinX + cosY);
	
	int newTile [2] = { _currentTile [0] + movement [0], _currentTile [1] + movement [1] };
	//Check if new position is valid
	if
	(
		(movement [0] != 0 || movement [1] != 0) &&
		newTile [0] >= 0 && newTile [0] < Level::map->width &&
		newTile [1] >= 0 && newTile [1] < Level::map->height &&
		Level::map->baseTiles [newTile [0]] [newTile [1]] != (int)nullptr
	)
	{
		//Rotate model to the correct orientation
		int temp = (2 + movement [1]);
		if (temp == 3 && movement [0] == 0)
		{
			temp = 180;
		}
		else
		{
			temp *= movement [0] * -45;
		}
		_model->rotate (glm::vec3 (0, temp, 0) - _modelOrientation);

		//play rotation animation
		switch ((int)_modelOrientation[1])
		{
			case 0:
				switch (temp)
				{
					case 90:
						_rotationAnimator->playAnimation(0, false, &stopFunctionPlayerRotation, this);
						break;
					case -90:
						_rotationAnimator->playAnimation(1, false, &stopFunctionPlayerRotation, this);
						break;
					case 180:
						_rotationAnimator->playAnimation(2, false, &stopFunctionPlayerRotation, this);
						break;
				}
				break;
			case 90:
				switch (temp)
				{
					case 0:
						_rotationAnimator->playAnimation(3, false, &stopFunctionPlayerRotation, this);
						break;
					case -90:
						_rotationAnimator->playAnimation(4, false, &stopFunctionPlayerRotation, this);
						break;
					case 180:
						_rotationAnimator->playAnimation(5, false, &stopFunctionPlayerRotation, this);
						break;
				}
				break;
			case -90:
				switch (temp)
				{
					case 0:
						_rotationAnimator->playAnimation(6, false, &stopFunctionPlayerRotation, this);
						break;
					case 90:
						_rotationAnimator->playAnimation(7, false, &stopFunctionPlayerRotation, this);
						break;
					case 180:
						_rotationAnimator->playAnimation(8, false, &stopFunctionPlayerRotation, this);
						break;
				}
				break;
			case 180:
				switch (temp)
				{
					case 0:
						_rotationAnimator->playAnimation(9, false, &stopFunctionPlayerRotation, this);
						break;
					case 90:
						_rotationAnimator->playAnimation(10, false, &stopFunctionPlayerRotation, this);
						break;
					case -90:
						_rotationAnimator->playAnimation(11, false, &stopFunctionPlayerRotation, this);
						break;
				}
				break;
		}

		if (_modelOrientation.y != temp) //have to rotate first
		{
			_noMove = true;
			_modelOrientation.y = temp;
			_movementToComplete[0] = movement[0];
			_movementToComplete[1] = movement[1];
		}
		else //can move without rotation
		{
			if (movePlayer(movement[0], movement[1], true))
			{
				_noMove = true;
			}
		}
	}
}

void Player::setDirection (int pDirection [2])
{
	int temp = (2 + pDirection [1]);
	if (temp == 3 && pDirection [0] == 0)
	{
		temp = 180;
	}
	else
	{
		temp *= pDirection [0] * -45;
	}
	_model->rotate (glm::vec3 (0, temp, 0) - _modelOrientation);
	_modelOrientation.y = temp;
}

//Check if the player has a certain item in their inventory
bool Player::hasItem (std::string pItemName)
{
	for (int i = 0, size = inventory.size (); i < size; i ++)
	{
		if (inventory [i] == pItemName)
		{
			return true;
		}
	}
	return false;
}

glm::vec3 Player::getActualWorldPosition()
{
	if (_baseAnimator->isPlaying() || _oldActualWorldPosition == glm::vec3(0, 0, 0) && !_dead)
	{
		_oldActualWorldPosition = _baseModel->getWorldPosition();
		_oldActualWorldPosition.y = 0;
	}
	return _oldActualWorldPosition;
}