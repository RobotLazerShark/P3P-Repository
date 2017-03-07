#include <P3P/objects/Gate.hpp>
#include <P3P/Level.hpp>
#include <P3P/objects/Player.hpp>
#include <JCPPEngine/SoundManager.hpp>
#include <JCPPEngine/Random.hpp>


//Constructor
Gate::Gate(int pX, int pZ) : ButtonTarget ()
{
	//Set up model
	_model = new GameObject("GateFrame.obj");
	_model->setMaterial (new LitMaterial ("GateFrame.png"));
	_model->setParent(this);
	GameObject* subModel = new GameObject ("Gate.obj");
	subModel->setMaterial(new LitMaterial("Gate.png"));
	subModel->setParent (_model);
	_animator = new AnimationBehaviour ({ "GateUp.txt", "GateDown.txt" }, false);
	subModel->setBehaviour (_animator);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//save position
	_position [0] = pX;
	_position [1] = pZ;
}


//Get x position
int Gate::x ()
{
	return _position [0];
}

//Get z position
int Gate::z ()
{
	return _position [1];
}


//Based on whether or not the connect button is pressed, open and close the gate
bool Gate::setActive (bool pActive)
{
	if (pActive)
	{
		//make gate position in array empty
		Level::map->objectTiles[_position [0]][_position [1]] = (int)nullptr;
		_animator->playAnimation (0);
		sf::Sound* sound = new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/GateUp.wav"));
		sound->setPitch (1 + (JCPPEngine::Random::Value () - 0.5f) * 0.5f);
		JCPPEngine::SoundManager::PlaySound (sound);
	}
	else
	{
		//if gate tile is empty
		if (Level::map->objectTiles[_position [0]][_position [1]] == (int)nullptr)
		{
			//put gate back into array
			Level::map->objectTiles[_position [0]][_position [1]] = (int)this;
			_animator->playAnimation (1);
			sf::Sound* sound = new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/GateDown.wav"));
			sound->setPitch (1 + (JCPPEngine::Random::Value () - 0.5f) * 0.5f);
			JCPPEngine::SoundManager::PlaySound (sound);
		}
		//if gate tile is taken by the player
		else if(Player::singletonInstance->_currentTile [0] == _position [0] && Player::singletonInstance->_currentTile [1] == _position [1])
		{
			_animator->playAnimation (1);
			sf::Sound* sound = new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/GateDown.wav"));
			sound->setPitch (1 + (JCPPEngine::Random::Value () - 0.5f) * 0.5f);
			JCPPEngine::SoundManager::PlaySound (sound);
			//Kill the player
			Player::singletonInstance->die ();
		}
		else
		{
			return false; //return false so the button will keep calling this function until the gate can close, or the box/player goes off the button
		}
	}
	return true;//return true to let the button know the switch succeeded
}