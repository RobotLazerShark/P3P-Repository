#include "P3P/objects/base objects/Spikes.hpp"
#include <P3P/Level.hpp>


//Constructor
Spikes::Spikes(int pX, int pZ) : GameObject()
{
	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->translate(glm::vec3(0, -0.5f, 0));
	_model->setMaterial(new ColorMaterial(glm::vec3(0, 1, 0)));
	_model->setParent(this);
	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//set starting time
	time = clock.getElapsedTime();
	oldTime = time.asSeconds();

	//save position
	arrayPosition.x = pX;
	arrayPosition.y = pZ; 
}

void Spikes::update(float pStep, bool pUpdateWorldTransform)
{
	GameObject::update(pStep, pUpdateWorldTransform);

	//update time
	time = clock.getElapsedTime();
	timeSinceLastChange += time.asSeconds() - oldTime;
	oldTime = time.asSeconds();

	//switch on/off spikes if its time to
	if (timeSinceLastChange > SWITCH_TIME)
	{
		timeSinceLastChange -= SWITCH_TIME;

		isOn = !isOn;

		if (isOn) //set material to red(danger)
		{
			_model->setMaterial(new ColorMaterial(glm::vec3(1, 0, 0)));
		}
		else //set green to red(safe)
		{
			_model->setMaterial(new ColorMaterial(glm::vec3(0, 1, 0)));
		}
	}

	if (isOn)//check if player standing on spikes
	{
		Player * player = dynamic_cast <Player*> ((GameObject*)Level::map->objectTiles[arrayPosition.x][arrayPosition.y]);
		if (player != nullptr)
		{
			Level::singletonInstance->reloadLevel(); //reload level
		}
	}
}