#include <P3P/objects/Fan.hpp>
#include <P3P/Level.hpp>

Fan::Fan(int pX, int pZ, int pXDirection, int pYDirection, bool pReversed) : GameObject()
{
	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->translate(glm::vec3(0, 0.5f, 0));
	if (!pReversed)
	{
		_model->setMaterial(new LitMaterial("Fan.jpg"));
	}
	else
	{
		_model->setMaterial(new LitMaterial("FanR.jpg"));
	}
	_model->setParent(this);
	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//save position
	_position[0] = pX;
	_position[1] = pZ;

	//save direction
	_direction[0] = pXDirection;
	_direction[1] = pYDirection;

	//set mode
	_reversed = pReversed;
	//save items in visible area
	for (int i = 0; i < _visibleAreaSize; i++)
	{
		int tempX = _position[0] + _direction[0] * (i + 1);
		int tempY = _position[1] + _direction[1] * (i + 1);
		//if position of element in visible area isn't out of bounds
		if
		(
			tempX >= 0 && tempX < Level::map->width &&
			tempY >= 0 && tempY < Level::map->height
		)
		{
			_visibleArea [i] = Level::map->objectTiles [tempX] [tempY];
		}
		else 
		{
			_visibleArea [i] = 0;
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

void Fan::update(float pStep, bool pUpdateWorldTransform)
{
	GameObject::update(pStep, pUpdateWorldTransform);

	//push if any changes registered
	if (checkForChanges())
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
	for (int i = 0; i < _visibleAreaSize; i++)
	{
		int tempX = _position[0] + _direction[0] * (i + 1);
		int tempY = _position[1] + _direction[1] * (i + 1);
		//if position of element in visible area isn't out of bounds
		if
		(
			tempX >= 0 && tempX < Level::map->width &&
			tempY >= 0 && tempY < Level::map->height
		)
		{
			//check if element in visible area is equal to saved one
			if (_visibleArea[i] != Level::map->objectTiles[tempX][tempY])
			{
				_visibleArea [i] = Level::map->objectTiles [tempX] [tempY];
				_changeIndex = i;//Store the index of the changed item that is the furthest away
				changes = true;
			}
		}
		else 
		{
			if (_visibleArea [i] != 0)
			{
				_visibleArea [i] = 0;
				changes = true;
			}
		}
	}
	return changes;
}

void Fan::push() //moves every box in vision by 1 tile if possible
{
	Box* box;
	Player* player;
	if (_changeIndex == _visibleAreaSize)
	{
		_changeIndex --;
	}
	for (int i = _changeIndex; i >= 0; i --)//Go through all positions that are affected by the change
	{
		if (_visibleArea [i+1] == (int)nullptr && _visibleArea [i] != (int)nullptr)
		{
			box = dynamic_cast <Box*> ((GameObject*)_visibleArea [i]);
			player = dynamic_cast <Player*> ((GameObject*)_visibleArea [i]);
			if (box != nullptr)
			{
				box->moveBox(_direction[0], _direction[1]);
			}
			else if (player != nullptr)
			{
				player->movePlayer(_direction[0], _direction[1], true);
			}
		}
	}
}

void Fan::pull()
{
	Box* box;
	Player* player;
	for (int i = 1; i <= _changeIndex; i ++)//Go through all positions that are affected by the change
	{
		if (_visibleArea[i - 1] == (int)nullptr)
		{
			box = dynamic_cast <Box*> ((GameObject*)_visibleArea [i]);
			player = dynamic_cast <Player*> ((GameObject*)_visibleArea [i]);
			if (box != nullptr)
			{
				box->moveBox(-_direction[0], -_direction[1]);
			}
			else if (player != nullptr)
			{
				player->movePlayer(-_direction[0], -_direction[1], true);
			}
		}
	}
}