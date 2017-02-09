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


//Load the level this door leads to
bool Door:: enter ()
{
    if (Level::singletonInstance->levelKey () >= _nextLevelNumber && Level::singletonInstance->levelCompleted)
    {
        Level::singletonInstance->loadLevel (_nextLevelNumber);
        return true;
    }
    else
    {
        return false;
    }
}


//Get the level this door is supposed to load
int Door::levelNumber ()
{
	return _nextLevelNumber;
}