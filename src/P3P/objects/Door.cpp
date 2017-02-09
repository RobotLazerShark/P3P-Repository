#include "P3P/objects/Door.hpp"
#include <P3P/Level.hpp>


//Constructor
Door::Door (int pX, int pZ, int pNextLevelNumber) : GameObject ()
{
	_nextLevelNumber = pNextLevelNumber;

	//Set up model
	_model = new GameObject ("cube_flat.obj");
	_model->setMaterial (new LitMaterial (glm::vec3 (0.5f, 0.5f, 0)));
	_model->translate (glm::vec3 (0, 0.5f, 0));
	_model->setParent (this);

	translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
}

void Door:: enter ()
{
    Level::singletonInstance->clear ();
    Level::singletonInstance->setMap (_nextLevelNumber);
    Level::singletonInstance->loadMap ();
}
