#include "P3P/objects/base objects/BreakingBlock.hpp"
#include <P3P/Level.hpp>


//Constructor
BreakingBlock::BreakingBlock(int pX, int pZ) : GameObject()
{
	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->translate(glm::vec3(0, -0.5f, 0));

	_model->setMaterial(new LitMaterial(glm::vec3(0.6f,0.3f,0.3f)));
	_model->setParent(this);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
	_position [0] = pX;
	_position [1] = pZ;
}

void BreakingBlock::update (float pStep, bool pUpdateWorldTransform)
{
	//If the player is on top of us, mark for destruction
	if (!_breaking && Player::singletonInstance->_currentTile [0] == _position [0] && Player::singletonInstance->_currentTile [1] == _position [1])
	{
		_breaking = true;
	}
	//If the player was on us before, but not anymore, break
	else if (_breaking && Player::singletonInstance->_currentTile[0] != _position[0] && Player::singletonInstance->_currentTile[1] != _position[1])
	{
		breakBlock ();
	}
	GameObject::update (pStep, pUpdateWorldTransform);
}

void BreakingBlock::breakBlock ()
{
	//remove from array
	Level::map->baseTiles [_position [0]] [_position [1]] = (int)nullptr;

	//delete
	setParent (nullptr);
	delete this;
}