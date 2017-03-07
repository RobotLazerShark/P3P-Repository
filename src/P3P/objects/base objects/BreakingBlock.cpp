#include "P3P/objects/base objects/BreakingBlock.hpp"
#include <P3P/Level.hpp>
#include <P3P/objects/Player.hpp>
#include <JCPPEngine/Random.hpp>
#include <JCPPEngine/SoundManager.hpp>


//Constructor
BreakingBlock::BreakingBlock(int pX, int pZ) : GameObject()
{
	//Set up model
	_model = new GameObject();
	_model->translate(glm::vec3(0, -0.05f, 0));
	_model->rotate (glm::radians (JCPPEngine::Random::Range (0, 3) * 90.0f), glm::vec3 (0, 1, 0));
	_model->setParent(this);
	GameObject* submodel = new GameObject ("FloorBroken.obj");
	submodel->setMaterial(new LitMaterial("FloorBroken.png"));
	submodel->setParent (_model);
	_animator = new AnimationBehaviour ({ "UnstableBlock.txt", "FallingBlock.txt" });
	submodel->setBehaviour (_animator);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
	_position [0] = pX;
	_position [1] = pZ;
}
BreakingBlock::~BreakingBlock ()
{
	if (_soundIndex != -1)
	{
		JCPPEngine::SoundManager::StopSoundLoop (_soundIndex);
		_soundIndex = -1;
	}
}


//Breaks this block (post-animation function)
void stopFunctionBreakingBlock (int pAnimIndex, GameObject* pOwner)
{
	BreakingBlock* block = (BreakingBlock*)pOwner;
	switch (pAnimIndex)
	{
		case 1:
			//mark for deletion
			block->_delete = true;
			break;
		case 0:
			JCPPEngine::SoundManager::StopSoundLoop (block->_soundIndex);
			block->_soundIndex = -1;
			break;
		default:
			break;
	}
}

//Update loop
void BreakingBlock::update (float pStep, bool pUpdateWorldTransform)
{
	GameObject::update (pStep, pUpdateWorldTransform);

	if (_delete)
	{
		setParent (nullptr);
		delete this;
	}
	else
	{
		//If the player is on top of us, mark for destruction
		if ((_breakLevel == 0) && Player::singletonInstance->_currentTile [0] == _position [0] && Player::singletonInstance->_currentTile [1] == _position [1])
		{
			_breakLevel = 1;
			_soundIndex = JCPPEngine::SoundManager::PlaySoundLoop (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/Unstable.wav")));
			_animator->playAnimation (0, true, &stopFunctionBreakingBlock, this);
		}
		//If the player was on us before, but not anymore, break
		else if ((_breakLevel == 1) && (Player::singletonInstance->_currentTile[0] != _position[0] || Player::singletonInstance->_currentTile[1] != _position[1]))
		{
			_breakLevel = 2;
			//remove the block from array
			Level::map->baseTiles [_position [0]] [_position [1]] = (int)nullptr;
			JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/Breaking.wav")));
			_animator->playAnimation (1, false, &stopFunctionBreakingBlock, this);

			Stats::singletonInstance->data.platformsBroke++;
			Stats::singletonInstance->refreshText();
		}
	}
}