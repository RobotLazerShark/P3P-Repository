#include "P3P/objects/Collectable.hpp"
#include <P3P/Level.hpp>
#include <P3P/objects/Player.hpp>

//Constructor
Collectable::Collectable(int pX, int pZ, std::string pName) : GameObject()
{
	_name = pName;

	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->setMaterial(new ColorMaterial(glm::vec3(0.6f,0.6f,0)));
	_model->translate(glm::vec3(0, 0.5f, 0));
	_model->setParent(this);
	_model->scale(0.5f);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
	_position [0] = pX;
	_position [1] = pZ;
}

//Get this collectable's name
std::string Collectable::getName ()
{
	return _name;
}


//Update
void Collectable::update (float pStep, bool pUpdateWorldTransform)
{
	//Check if player has picked us up
	if (!_collected && Player::singletonInstance->_currentTile [0] == _position [0] && Player::singletonInstance->_currentTile [1] == _position [1])
	{
		_collected = true;
		//Add this object to the player's inventory
		Player::singletonInstance->inventory.push_back (this);
		collect ();
		//Remove us from level and make us invisible.
		setParent (nullptr);
		Level::map->objectTiles[_position[0]] [_position[1]] = (int)nullptr;
	}
}


//Abstract function to override, for extra functionality
void Collectable::collect ()
{
}