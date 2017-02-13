#include "P3P/objects/base objects/Spikes.hpp"
#include <P3P/Level.hpp>


//Constructor
Spikes::Spikes(int pX, int pZ) : GameObject()
{
	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->translate(glm::vec3(0, -0.5f, 0));
	_model->setMaterial(new LitMaterial(glm::vec3(0.75f, 0.25f, 0)));
	_model->setParent(this);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//save position
	_position [0] = pX;
	_position [1] = pZ;
}

void Spikes::update(float pStep, bool pUpdateWorldTransform)
{
	//update time
	_timer += pStep;

	//switch on/off spikes if its time to
	if (_timer >= SWITCH_TIME)
	{
		_timer = 0;
		_spikesUp = !_spikesUp;

		if (_spikesUp) //set color to red(danger)
		{
			((LitMaterial*)_model->getMaterial ())->SetColor (glm::vec3 (1, 0, 0));
		}
		else //set color to green(safe)
		{
			((LitMaterial*)_model->getMaterial ())->SetColor (glm::vec3 (0.75f, 0.25f, 0));
		}
	}

	if (_spikesUp)//check if player standing on spikes
	{
		if (Player::singletonInstance->_currentTile [0] == _position [0] && Player::singletonInstance->_currentTile [1] == _position [1])//If the player is on top of us, the player dies
		{
			//Kill the player
			Player::singletonInstance->die ();
		}
	}

	GameObject::update(pStep, pUpdateWorldTransform);
}