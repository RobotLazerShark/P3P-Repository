#include <P3P/objects/base objects/Button.hpp>
#include <P3P/Level.hpp>
#include <JCPPEngine/SoundManager.hpp>

Button::Button (int pX, int pZ, ButtonTarget* pTarget) : GameObject()
{
	_target = pTarget;

	//Set up model
	_model = new GameObject("Button.obj");
	_model->translate(glm::vec3(0, -0.05f, 0));
	_model->rotate (glm::radians (180.0f), glm::vec3 (0,1,0));
	_model->setMaterial(new LitMaterial("Button.png"));
	_model->setParent(this);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//save position
	_position [0] = pX;
	_position [1] = pZ;
}

void Button::update(float pStep, bool pUpdateWorldTransform)
{
	if (_target != nullptr)
	{
		//activate buttontarget if it's not activated and something is standing on the platform 
		if (!activated && Level::map->objectTiles[_position [0]][_position [1]] != (int)nullptr && _target->setActive (true))
		{
			activated = true;//Only switch state if switch succeeded
			JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/Button.wav")));
		}
		//deactivate buttontarget if it's activated and nothing is standing on the platform 
		else if (activated && Level::map->objectTiles[_position [0]][_position [1]] == (int)nullptr && _target->setActive (false))
		{
			activated = false;//Only switch state if switch succeeded
			JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/Button.wav")));
		}
	}

	GameObject::update(pStep, pUpdateWorldTransform);
}
