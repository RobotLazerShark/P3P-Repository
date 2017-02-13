#include "P3P/objects/Player.hpp"
#include <P3P/Level.hpp>


//Static variables
Player* Player::singletonInstance = nullptr;

//////////////////////////////|	INSTANCE MANAGEMENT
//Constructor
Player::Player (int pX, int pZ, ProgressTracker* pProgressTracker) : GameObject ()
{
	if (singletonInstance != nullptr)
	{
		return;
	}
	singletonInstance = this;
        _progressTracker = pProgressTracker;
	registerForEvent (JCPPEngine::Event::EventType::KeyDown);

	//set up model
	_model = new GameObject ();
	_model->translate (glm::vec3 (0, 0.5f, 0));
	_model->setParent (this);//By making the model a child object, we can rotate it without it affecting translation directions.
	GameObject* wheelModel = new GameObject ("cylinder_lowpoly.obj");
	wheelModel->setParent (_model);
	wheelModel->setMaterial (new LitMaterial ("Default.png"));
	GameObject* baseModelOffset = new GameObject ();
	baseModelOffset->translate (glm::vec3 (0, 0.5f, 0));
	baseModelOffset->setParent (_model);
	GameObject* baseModel = new GameObject ("cube_flat.obj");
	baseModel->setParent (baseModelOffset);
	baseModel->setMaterial (new LitMaterial (glm::vec3 (1, 1, 1)));
	_wheelAnimator = new AnimationBehaviour ({ "PlayerWheel.txt" });
	_baseAnimator = new AnimationBehaviour ({ "PlayerBase.txt" });
	wheelModel->setBehaviour (_wheelAnimator);
	baseModel->setBehaviour (_baseAnimator);

	translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
	_currentTile [0] = pX;
	_currentTile [1] = pZ;
	_oldTile [0] = _currentTile [0];
	_oldTile [1] = _currentTile [1];
}

//Destructor
Player::~Player ()
{
	singletonInstance = nullptr;
	unregisterForEvent (JCPPEngine::Event::EventType::KeyDown);
	if (_progressTracker != nullptr)
	{
		delete _progressTracker;
	}
	for (int i = 0, size = inventory.size (); i < size; i ++)
	{
		delete inventory [i];
	}
	_wheelAnimator = nullptr;
	_baseAnimator = nullptr;
	GameObject::~GameObject ();
}


//////////////////////////////|	UPDATING FUNCTIONS
//Move the player by a given amount of tiles
bool Player::movePlayer (int pX, int pZ, bool pTranslate)
{
    //update position
    _oldTile [0] = _currentTile [0];
    _oldTile [1] = _currentTile [1];
    _currentTile [0] += pX;
    _currentTile [1] += pZ; //std::cout<<_oldTile[0]<<", "<<_oldTile[1]<<"\t"<<_currentTile[0]<<", "<<_currentTile[1]<<std::endl;

	//Check if old position contains a breaking block
	BreakingBlock * breakingBlock = dynamic_cast <BreakingBlock*> ((GameObject*)Level::map->baseTiles[_oldTile[0]][_oldTile[1]]);
	if (breakingBlock != nullptr)//old position contains a breaking block
	{
		std::cout << "found" << std::endl;
		breakingBlock->breakBlock();
	}

    if (Level::map->objectTiles [_currentTile [0]] [_currentTile [1]] != (int)nullptr)
    {
        //Check if the new position contains a box or a door
        Box* box = dynamic_cast <Box*> ((GameObject*)Level::map->objectTiles [_currentTile [0]] [_currentTile [1]]);
        Door* door = dynamic_cast <Door*> ((GameObject*)Level::map->objectTiles [_currentTile [0]] [_currentTile [1]]);
		Collectable* collectable = dynamic_cast <Collectable*> ((GameObject*)Level::map->objectTiles[_currentTile[0]][_currentTile[1]]);
		Npc* npc = dynamic_cast <Npc*> ((GameObject*)Level::map->objectTiles[_currentTile[0]][_currentTile[1]]);
		Gate* gate = dynamic_cast <Gate*> ((GameObject*)Level::map->objectTiles[_currentTile[0]][_currentTile[1]]);

        if (door != nullptr)//The new position contains a door
        {
			if (!door->enter ())
			{
				//we cannot enter the door
				_currentTile [0] = _oldTile [0];
				_currentTile [1] = _oldTile [1];
				return false;
			}
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
				box->moveBox (pX, pZ);
			}
			else
			{
				//we cannot move the box
				_currentTile [0] = _oldTile [0];
				_currentTile [1] = _oldTile [1];
				return false;
			}
		}
		else if (collectable != nullptr)//The new position contains a collectible
		{
			//collectable->collect();
		}
		else if (npc != nullptr)//The new position contains a npc
		{
			npc->talk();
			//dont move
			_currentTile[0] = _oldTile[0];
			_currentTile[1] = _oldTile[1];
			return false;
		}
		else if (gate != nullptr)//The new position contains a gate
		{
			//dont move
			_currentTile[0] = _oldTile[0];
			_currentTile[1] = _oldTile[1];
			return false;
		}
    }

    //update the object array
    Level::map->objectTiles [_currentTile [0]] [_currentTile [1]] = (int)this;
    Level::map->objectTiles [_oldTile [0]] [_oldTile [1]] = (int)nullptr;

    //move
    if (pTranslate)
    {
        translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
    }

    //check for win
    if (_progressTracker->checkWin () && !Level::singletonInstance->levelCompleted)
    {
        Level::singletonInstance->levelCompleted = true;
        if (_progressTracker->boxSpots.size () > 0)
        {
            Level::singletonInstance->increaseLevelKey ();
        }
    }
    return true;
}


//////////////////////////////|	EVENT-BASED FUNCTIONS
//Function to be called when animations are stopped. It has to be in global space, or we have to pass our object type as well.
void stopFunction (int pAnimIndex)
{
	switch (pAnimIndex)
	{
		case 0:
			Player::singletonInstance->_moving = false;
			Player::singletonInstance->setWorldPosition (glm::vec3 (Player::singletonInstance->_currentTile [0] * Level::TILESIZE, 0, Player::singletonInstance->_currentTile [1] * Level::TILESIZE));
			break;
		default:
			break;
	}
}

//Cause the player to die, and reload the level
void Player::die ()
{
	//Play death animation/sound
	Level::singletonInstance->reloadLevel ();
}

//Process input events
void Player::ProcessEvent (JCPPEngine::Event* pEvent)
{
	JCPPEngine::KeyEvent* keyDownEvent = (JCPPEngine::KeyEvent*)pEvent;
	if (keyDownEvent == nullptr || keyDownEvent->keyState () != JCPPEngine::InputManager::KEY_DOWN || _moving)
	{
		return;
	}

	int movement [2] = { 0, 0 };
	switch (keyDownEvent->key ())
	{
		case sf::Keyboard::Key::Up:
			movement [1] --;
			break;
		case sf::Keyboard::Key::Down:
			movement [1] ++;
			break;
		case sf::Keyboard::Key::Left:
			movement [0] --;
			break;
		case sf::Keyboard::Key::Right:
			movement [0] ++;
			break;
		case sf::Keyboard::Key::R:
			Level::singletonInstance->reloadLevel ();
			return;
		default:
			break;
	}
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
		_modelOrientation.y = temp;
		if (movePlayer (movement [0], movement [1], false))
		{
			_moving = true;
			_wheelAnimator->playAnimation (0);
			_baseAnimator->playAnimation (0, false, &stopFunction);
		}
	}
}