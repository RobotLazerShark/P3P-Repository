<<<<<<< HEAD
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
	_currentTile = glm::vec2 (pX, pZ);
	_oldTile = glm::vec2 (_currentTile);
}


//Move the player by a given amount of tiles
void Player::movePlayer (int pX, int pZ)
{
    //update position
    _oldTile.x = _currentTile.x;
    _oldTile.y = _currentTile.y;
    _currentTile.x += pX;
    _currentTile.y += pZ;

    //Check if the new position contains a box or a door
    Box* box = (Box*)Level::map->objectTiles [_currentTile.x] [_currentTile.y];
    Door* door = (Door*)Level::map->objectTiles [_currentTile.x] [_currentTile.y];
    if (box != nullptr)//The new position contains a box
    {
	glm::vec2 newBoxTile = glm::vec2 (_currentTile);
	newBoxTile.x += pX;
	newBoxTile.y += pZ;
	//Check if we can move the box
	if
	(
		newBoxTile.x >= 0 && newBoxTile.x < Level::map->width &&
		newBoxTile.y >= 0 && newBoxTile.y < Level::map->height &&
		Level::map->baseTiles [newBoxTile.x] [newBoxTile.y] != (int)nullptr &&
		Level::map->objectTiles [newBoxTile.x] [newBoxTile.y] == (int)nullptr
	)
	{
		//we can move the box
		box->moveBox (pX, pZ);
	}
	else
	{
		//we cannot move the box
		_currentTile.x = _oldTile.x;
		_currentTile.y = _oldTile.y;
		return;
	}
    }
    else if (door != nullptr)//The new position contains a door
    {
	door->enter ();
    }

    //update the object array
    Level::map->objectTiles [_currentTile.x] [_currentTile.y] = (int)this;
    Level::map->objectTiles [_oldTile.x] [_oldTile.y] = (int)nullptr;

    //move
    translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

    //check for win
    _progressTracker->checkWin ();
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

	glm::vec2 movement = glm::vec2 (0, 0);
	switch (keyDownEvent->key ())
	{
		case sf::Keyboard::Key::Up:
			movement.y --;
			break;
		case sf::Keyboard::Key::Down:
			movement.y ++;
			break;
		case sf::Keyboard::Key::Left:
			movement.x --;
			break;
		case sf::Keyboard::Key::Right:
			movement.x ++;
			break;
		default:
			break;
	}
	glm::vec2 newTile = glm::vec2 (_currentTile);
	newTile += movement;
	//Check if new position is valid
	if
	(
		glm::length (movement) > 0 &&
		newTile.x >= 0 && newTile.x < Level::map->width &&
		newTile.y >= 0 && newTile.y < Level::map->height &&
		Level::map->baseTiles [newTile.x] [newTile.y] != (int)nullptr
	)
	{
		movePlayer (movement.x, movement.y);
	}
	//Process key/mouse events
=======
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
	_currentTile = glm::vec2 (pX, pZ);
	_oldTile = glm::vec2 (_currentTile);
}


//Move the player by a given amount of tiles
void Player::movePlayer (int pX, int pZ)
{
    //update position
    _oldTile.x = _currentTile.x;
    _oldTile.y = _currentTile.y;
    _currentTile.x += pX;
    _currentTile.y += pZ;

    //Check if the new position contains a box or a door
    Box* box = (Box*)Level::map->objectTiles [_currentTile.x] [_currentTile.y];
    Door* door = (Door*)Level::map->objectTiles [_currentTile.x] [_currentTile.y];
    if (box != nullptr)//The new position contains a box
    {
	glm::vec2 newBoxTile = glm::vec2 (_currentTile);
	newBoxTile.x += pX;
	newBoxTile.y += pZ;
	//Check if we can move the box
	if
	(
		newBoxTile.x >= 0 && newBoxTile.x < Level::map->width &&
		newBoxTile.y >= 0 && newBoxTile.y < Level::map->height &&
		Level::map->baseTiles [newBoxTile.x] [newBoxTile.y] != (int)nullptr &&
		Level::map->objectTiles [newBoxTile.x] [newBoxTile.y] == (int)nullptr
	)
	{
		//we can move the box
		box->moveBox (pX, pZ);
	}
	else
	{
		//we cannot move the box
		_currentTile.x = _oldTile.x;
		_currentTile.y = _oldTile.y;
		return;
	}
    }
    else if (door != nullptr)//The new position contains a door
    {
	door->enter ();
    }

    //update the object array
    Level::map->objectTiles [_currentTile.x] [_currentTile.y] = (int)this;
    Level::map->objectTiles [_oldTile.x] [_oldTile.y] = (int)nullptr;

    //move
    translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

    //check for win
    _progressTracker->checkWin ();
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

	glm::vec2 movement = glm::vec2 (0, 0);
	switch (keyDownEvent->key ())
	{
		case sf::Keyboard::Key::Up:
			movement.y --;
			break;
		case sf::Keyboard::Key::Down:
			movement.y ++;
			break;
		case sf::Keyboard::Key::Left:
			movement.x --;
			break;
		case sf::Keyboard::Key::Right:
			movement.x ++;
			break;
		default:
			break;
	}
	glm::vec2 newTile = glm::vec2 (_currentTile);
	newTile += movement;
	//Check if new position is valid
	if
	(
		glm::length (movement) > 0 &&
		newTile.x >= 0 && newTile.x < Level::map->width &&
		newTile.y >= 0 && newTile.y < Level::map->height &&
		Level::map->baseTiles [newTile.x] [newTile.y] != (int)nullptr
	)
	{
		movePlayer (movement.x, movement.y);
	}
	//Process key/mouse events
>>>>>>> 05a65edaed0d726361052543950587d121f9c233
}