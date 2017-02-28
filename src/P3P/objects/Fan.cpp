#include <P3P/objects/Fan.hpp>
#include <P3P/Level.hpp>
#include <P3P/Moveable.hpp>
#include <P3P/objects/Player.hpp>


Fan::Fan(int pX, int pZ, int pXDirection, int pYDirection, bool pReversed) : ButtonTarget()
{
	//set mode
	_reversed = pReversed;

	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->translate(glm::vec3(0, 0.5f, 0));
	//Rotate fan to face push/pull direction
	if (pXDirection == 0)
	{
		_model->rotate (glm::radians (90.0f * pYDirection), glm::vec3 (0, 1, 0));
	}
	else if (pYDirection == 0)
	{
		_model->rotate(glm::radians((pXDirection < 0) ? 180.0f : 0.0f), glm::vec3(0, 1, 0));
	}
	_model->setParent(this);
	GameObject* subModel = new GameObject ("cube_flat.obj");
	subModel->setMaterial (new LitMaterial ("Black.png"));
	_animator = new AnimationBehaviour ({ "Fan.txt", "FanReverse.txt" }, false);
	subModel->setBehaviour (_animator);
	subModel->setParent (_model);
	_model->setMaterial (new LitMaterial (_reversed ? "FanR.png" : "Fan.png"));
	_animator->playAnimation (_reversed ? 1 : 0, true);
	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//save position
	_position[0] = pX;
	_position[1] = pZ;

	//save direction
	_direction[0] = pXDirection;
	_direction[1] = pYDirection;

	//save items in visible area
	bool pathBlocked = false;
	for (int i = 0; i < _visibleAreaSize; i++)
	{
		int tempX = _position[0] + _direction[0] * (i + 1);
		int tempY = _position[1] + _direction[1] * (i + 1);
		//if position of element in visible area isn't out of bounds
		if
		(
			!pathBlocked &&
			tempX >= 0 && tempX < Level::map->width &&
			tempY >= 0 && tempY < Level::map->height &&
			Level::map->baseTiles [tempX] [tempY] != (int)nullptr
		)
		{
			_visibleArea [i] = Level::map->objectTiles [tempX] [tempY];
		}
		else 
		{
			//Make sure fan doesn't push anything into this spot
			_visibleArea [i] = -1;
			pathBlocked = true;
		}
	}
}

void Fan::move(int pX, int pZ)
{
	//update position
	int oldTile[2] = { 0, 0 };
	oldTile[0] = _position[0];
	oldTile[1] = _position[1];
	_position[0] += pX;
	_position[1] += pZ;

	//update object array
	Level::map->objectTiles[_position[0]][_position[1]] = (int)this;
	Level::map->objectTiles[oldTile[0]][oldTile[1]] = (int)nullptr;

	//move gameoject
	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
}

bool Fan::setActive (bool pActive)
{
	//Switch fan's direction
	if (pActive && !_active)
	{
		_active = true;
		_reversed = !_reversed;
		_model->setMaterial (new LitMaterial (_reversed ? "FanR.png" : "Fan.png"));
		_animator->playAnimation (_reversed ? 1 : 0, true);
	}
	else if (!pActive && _active)
	{
		_active = true;
		_reversed = !_reversed;
		_model->setMaterial (new LitMaterial (_reversed ? "FanR.png" : "Fan.png"));
		_animator->playAnimation (_reversed ? 1 : 0, true);
	}
	return true;
}

void Fan::update(float pStep, bool pUpdateWorldTransform)
{
	GameObject::update(pStep, pUpdateWorldTransform);

	//push if any changes registered
	if (!_animating && checkForChanges())
	{
		if (!_reversed)
		{
			push();
		}
		else
		{
			pull();
		}
	}
}

bool Fan::checkForChanges() //return true if any changes found
{
	bool changes = false;
	bool pathBlocked = false;
	_changeIndex = -1;
	for (int i = 0; i < _visibleAreaSize; i++)
	{
		int tempX = _position[0] + _direction[0] * (i + 1);
		int tempY = _position[1] + _direction[1] * (i + 1);
		//if position of element in visible area isn't out of bounds
		if
		(
			!pathBlocked &&
			tempX >= 0 && tempX < Level::map->width &&
			tempY >= 0 && tempY < Level::map->height &&
			Level::map->baseTiles [tempX] [tempY] != (int)nullptr
		)
		{
			//check if element in visible area is equal to saved one
			if (_visibleArea[i] != Level::map->objectTiles[tempX][tempY])
			{
				_visibleArea [i] = Level::map->objectTiles [tempX] [tempY];
				if (!_reversed || _changeIndex == -1)
				{
					_changeIndex = i;//Store the index of the changed item that is closest when pulling and farthest when pushing
				}
				changes = true;
			}
		}
		else 
		{
			//Make sure fan doesn't push/pull anything in(to) this spot and all further spots
			_visibleArea [i] = -1;
			pathBlocked = true;
		}
	}
	return changes;
}

//Function for correctly animating objects being pushed/pulled
void stopFunctionFan (int pAnimIndex, GameObject* pOwner)
{
	Fan* fan = (Fan*)pOwner;
	fan->_animating = false;
	if (fan->_movingPlayer)
	{
		fan->_movingPlayer = false;
		Player::singletonInstance->blockMovement = false;
	}
	if (fan->checkForChanges())
	{
		if (!fan->_reversed)
		{
			fan->push();
		}
		else
		{
			fan->pull();
		}
	}
}

void Fan::push() //moves every box in vision by 1 tile if possible
{
	Moveable* box;
	Player* player;
	bool changes = false;
	if (_changeIndex == _visibleAreaSize)
	{
		_changeIndex --;
	}
	for (int i = _changeIndex; i >= 0; i --)//Go through all positions that are affected by the change
	{
		if (_visibleArea[i] == -1)
		{
			continue;
		}
		if (_visibleArea [i+1] == (int)nullptr && _visibleArea [i] != (int)nullptr)
		{
			box = dynamic_cast <Moveable*> ((GameObject*)_visibleArea [i]);
			player = dynamic_cast <Player*> ((GameObject*)_visibleArea [i]);
			if (box != nullptr)
			{
				box->stopAnimation ();
				if (!changes)
				{
					changes = true;
					_animating = true;
					box->move (_direction[0], _direction[1], true, &stopFunctionFan, this);
				}
				else
				{
					box->move (_direction[0], _direction[1], true);
				}
			}
			else if (player != nullptr)
			{
				player->stopAnimation ();
				if (!changes)
				{
					changes = true;
					_animating = true;
					player->movePlayer(_direction[0], _direction[1], true, &stopFunctionFan, this);
				}
				else
				{
					player->movePlayer(_direction[0], _direction[1], true);
				}
				if (!_movingPlayer)
				{
					_movingPlayer = true;
					Player::singletonInstance->blockMovement = true;
					//Rotate model to the correct orientation
					Player::singletonInstance->setDirection (_direction);
				}
			}
		}
	}
}

void Fan::pull()
{
	Moveable* box;
	Player* player;
	bool changes = false;
	if (_changeIndex == 0)
	{
		_changeIndex ++;
	}
	for (int i = _changeIndex - 1; i < _visibleAreaSize; i ++)//Go through all positions that are affected by the change
	{
		if (_visibleArea[i] == -1)
		{
			return;
		}
		if (_visibleArea[i - 1] == (int)nullptr && _visibleArea [i] != (int)nullptr)
		{
			box = dynamic_cast <Moveable*> ((GameObject*)_visibleArea [i]);
			player = dynamic_cast <Player*> ((GameObject*)_visibleArea [i]);
			if (box != nullptr)
			{
				box->stopAnimation ();
				if (!changes)
				{
					changes = true;
					_animating = true;
					box->move (-_direction[0], -_direction[1], true, &stopFunctionFan, this);
				}
				else
				{
					box->move (-_direction[0], -_direction[1], true);
				}
			}
			else if (player != nullptr)
			{
				player->stopAnimation ();
				if (!changes)
				{
					changes = true;
					_animating = true;
					player->movePlayer(-_direction[0], -_direction[1], true, &stopFunctionFan, this);
				}
				else
				{
					player->movePlayer(-_direction[0], -_direction[1], true);
				}
				if (!_movingPlayer)
				{
					_movingPlayer = true;
					Player::singletonInstance->blockMovement = true;
					//Rotate model to the correct orientation
					int revDirection [2] = { -_direction[0], -_direction[1] };
					Player::singletonInstance->setDirection (revDirection);
				}
			}
		}
	}
}