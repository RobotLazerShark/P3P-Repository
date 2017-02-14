#include <P3P/objects/Fan.hpp>
#include <P3P/Level.hpp>

Fan::Fan(int pX, int pZ, int pDirection[2], bool pReversed) : GameObject()
{
	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->translate(glm::vec3(0, 0.5f, 0));
	if (!pReversed)
		_model->setMaterial(new LitMaterial("Fan.jpg"));
	else
		_model->setMaterial(new LitMaterial("FanR.jpg"));
	_model->setParent(this);
	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//save position
	_position[0] = pX;
	_position[1] = pZ;

	//save direction
	_direction[0] = pDirection[0];
	_direction[1] = pDirection[1];

	//set mode
	_reversed = pReversed;
	//save visible area
	visibleAreaSize =  sizeof(_visibleArea) / sizeof(*_visibleArea);
	saveVisibleArea();
	//push/pull
	if (!_reversed)
		push();
	else
		pull();
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
			push();
		else
			pull();
	}
}

bool Fan::checkForChanges() //return true if any changes found
{
	for (int i = 0; i < visibleAreaSize; i++)
	{
		//if position of element in visible area isn't out of bounds
		if (_position[0] + _direction[0] * (i + 1) >= 0 && _position[0] + _direction[0] * (i + 1) < Level::map->width
			&& _position[1] + _direction[1] * (i + 1) >= 0 && _position[1] + _direction[1] * (i + 1) < Level::map->height)
		{
			//check if element in visible area is equal to saved one
			if (_visibleArea[i] != Level::map->objectTiles[_position[0] + _direction[0] * (i + 1)][_position[1] + _direction[1] * (i + 1)])
			{
				saveVisibleArea();
				return true;
			}
		}
		else 
		{
			if (_visibleArea[i] != 0)
			{
				saveVisibleArea();
				return true;
			}
		}
	}
	return false;
}

void Fan::saveVisibleArea()
{
	for (int i = 0; i < visibleAreaSize; i++)
	{
		//if position of element in visible area isn't out of bounds
		if (_position[0] + _direction[0] * (i + 1) >= 0 && _position[0] + _direction[0] * (i + 1) < Level::map->width
			&& _position[1] + _direction[1] * (i + 1) >= 0 && _position[1] + _direction[1] * (i + 1) < Level::map->height)
		{
			//save it
			_visibleArea[i] = Level::map->objectTiles[_position[0] + _direction[0] * (i + 1)][_position[1] + _direction[1] * (i + 1)];

			distanceToArrayBound = i + 1;
		}
		else
		{
			//make it 0
			_visibleArea[i] = 0;
		}
	}
}

void Fan::push() //moves every box in vision by 1 tile if possible
{
	for (int i = visibleAreaSize - 2; i >= 0; i--)
	{
		Box* box = dynamic_cast <Box*> ((GameObject*)_visibleArea[i]);
		Player* player = dynamic_cast <Player*> ((GameObject*)_visibleArea[i]);

		//check if future box position is valid
		if (box != nullptr && distanceToArrayBound > i+1 &&_visibleArea[i+1] == (int)nullptr)
		{
			box->moveBox(_direction[0], _direction[1]);
		}
		if (player != nullptr && distanceToArrayBound > i + 1 && _visibleArea[i + 1] == (int)nullptr)
		{
			player->movePlayer(_direction[0], _direction[1], true);
		}
	}
	//push again if needed
	if (checkForChanges())
	{
		push();
	}
}

void Fan::pull()
{
	for (int i = 1; i < visibleAreaSize-1; i++)
	{
		Box* box = dynamic_cast <Box*> ((GameObject*)_visibleArea[i]);
		Player* player = dynamic_cast <Player*> ((GameObject*)_visibleArea[i]);

		//check if future position is valid
		if (box != nullptr && _visibleArea[i - 1] == (int)nullptr)
		{
			box->moveBox(-_direction[0], -_direction[1]);
		}
		if (player != nullptr && _visibleArea[i - 1] == (int)nullptr)
		{
			player->movePlayer(-_direction[0], -_direction[1], true);
		}
	}

	//pull again if needed
	if (checkForChanges())
	{
		pull();
	}
}

