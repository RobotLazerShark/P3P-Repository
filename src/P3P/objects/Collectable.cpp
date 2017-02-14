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


//Add item to player inventory, and make it invisible
void Collectable::collect ()
{
	_collected = true;
	//Add this object to the player's inventory
	Player::singletonInstance->inventory.push_back (this);
	setParent (nullptr);
}