#include "P3P/objects/Player.hpp"
#include <P3P/Level.hpp>


//Constructor
Player::Player (int pX, int pZ, ProgressTracker* pProgressTracker) : GameObject ()
{
        _progressTracker = pProgressTracker;
	registerForEvent (JCPPEngine::Event::EventType::KeyDown);

	//set up model
	_model = new GameObject ("cube_flat.obj");
	_model->setMaterial (new LitMaterial (glm::vec3 (1, 1, 1)));
	_model->translate (glm::vec3 (0, 0.5f, 0));
	_model->setParent (this);//By making the model a child object, we can rotate it without it affecting translation directions.

	translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
	_currentTile [0] = pX;
	_currentTile [1] = pZ;
	_oldTile [0] = _currentTile [0];
	_oldTile [1] = _currentTile [1];
}
//Destructor
Player::~Player ()
{
	unregisterForEvent (JCPPEngine::Event::EventType::KeyDown);
	delete _progressTracker;
	GameObject::~GameObject ();
}


//Move the player by a given amount of tiles
void Player::movePlayer (int pX, int pZ)
{
    //update position
    _oldTile [0] = _currentTile [0];
    _oldTile [1] = _currentTile [1];
    _currentTile [0] += pX;
    _currentTile [1] += pZ;

	//Check if old position contains a breaking block
	BreakingBlock * breakingBlock = dynamic_cast <BreakingBlock*> ((GameObject*)Level::map->baseTiles[_oldTile[0]][_oldTile[1]]);
	if (breakingBlock != nullptr)//old position contains a breaking block
	{
		std::cout << "found" << std::endl;
		breakingBlock->breakBlock(_oldTile[0], _oldTile[1]);
	}

    //Check if the new position contains a box or a door
    Box* box = dynamic_cast <Box*> ((GameObject*)Level::map->objectTiles [_currentTile [0]] [_currentTile [1]]);
    Door* door = dynamic_cast <Door*> ((GameObject*)Level::map->objectTiles [_currentTile [0]] [_currentTile [1]]);
    if (door != nullptr)//The new position contains a door
    {
		if (!door->enter ())
		{
			//we cannot enter the door
			_currentTile [0] = _oldTile [0];
			_currentTile [1] = _oldTile [1];
			return;
		}
	return;
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
			return;
		}
    }

    //update the object array
    Level::map->objectTiles [_currentTile [0]] [_currentTile [1]] = (int)this;
    Level::map->objectTiles [_oldTile [0]] [_oldTile [1]] = (int)nullptr;

    //move
    translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

    //check for win
    if (_progressTracker->checkWin () && !Level::singletonInstance->levelCompleted)
    {
        Level::singletonInstance->levelCompleted = true;
        if (_progressTracker->boxSpots.size () > 0)
        {
            Level::singletonInstance->increaseLevelKey ();
        }
    }
}


//Update player animation//[TODO]
void Player::update (float pStep, bool pUpdateWorldTransform)
{
	GameObject::update (pStep, pUpdateWorldTransform);
}


//Process input events
void Player::ProcessEvent (JCPPEngine::Event* pEvent)
{
	JCPPEngine::KeyEvent* keyDownEvent = (JCPPEngine::KeyEvent*)pEvent;
	if (keyDownEvent == nullptr || keyDownEvent->keyState () != JCPPEngine::InputManager::KEY_DOWN)
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
		movePlayer (movement [0], movement [1]);
	}
}