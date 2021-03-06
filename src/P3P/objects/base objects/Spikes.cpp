#include "P3P/objects/base objects/Spikes.hpp"
#include <P3P/Level.hpp>
#include <P3P/objects/Player.hpp>
#include <JCPPEngine/Random.hpp>
#include <JCPPEngine/SoundManager.hpp>


//Constructor
Spikes::Spikes(int pX, int pZ) : ButtonTarget()
{
	//Set up model
	_model = new GameObject("SpikesFloor.obj");
	_model->translate(glm::vec3(0, -0.05f, 0));
	_model->setMaterial(new LitMaterial("SpikesFloor.png"));
	_model->setParent(this);
	_model->rotate (glm::radians (JCPPEngine::Random::Range (0, 3) * 90.0f), glm::vec3 (0, 1, 0));
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
		sf::Sound* sound = new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/Spikes.wav"));
		sound->setPitch (1 + (JCPPEngine::Random::Value () - 0.5f) * 0.5f);
		sound->setVolume (25 + JCPPEngine::Random::Range (-10, 10));
		JCPPEngine::SoundManager::PlaySound (sound);
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