#include "P3P/objects/base objects/Spikes.hpp"
#include <P3P/Level.hpp>
#include <P3P/objects/Player.hpp>


//Constructor
Spikes::Spikes(int pX, int pZ) : GameObject()
{
	//Set up model
	_model = new GameObject("SpikesFloor.obj");
	_model->translate(glm::vec3(0, -0.05f, 0));
	_model->setMaterial(new LitMaterial("SpikesFloor.png"));
	_model->setParent(this);
	GameObject* submodel = new GameObject ("Spikes.obj");
	submodel->setMaterial (new LitMaterial ("Spikes.png"));
	submodel->setParent (_model);
	_animator = new AnimationBehaviour ({ "SpikesUp.txt", "SpikesDown.txt" }, false);
	submodel->setBehaviour (_animator);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//save position
	_position [0] = pX;
	_position [1] = pZ;
}

bool Spikes::setActive (bool pActive)
{
	if (pActive && !_stopped)
	{
		_stopped = true;
		_timer = 0;
		if (_spikesUp)
		{
			_spikesUp = false;
			_animator->playAnimation (1);
		}
	}
	else if (!pActive && _stopped)
	{
		_stopped = false;
	}
	return true;
}

void Spikes::update(float pStep, bool pUpdateWorldTransform)
{
	GameObject::update(pStep, pUpdateWorldTransform);

	if (_stopped)
	{
		return;
	}

	//update time
	_timer = _timer + pStep;

	//switch on/off spikes if its time to
	if (_timer >= SWITCH_TIME)
	{
		_timer = 0;
		_spikesUp = !_spikesUp;

		if (_spikesUp)
		{
			_animator->playAnimation (0);
		}
		else
		{
			_animator->playAnimation (1);
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
}