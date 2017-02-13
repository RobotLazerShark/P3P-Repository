#include "P3P/objects/base objects/BreakingBlock.hpp"
#include <P3P/Level.hpp>


//Constructor
BreakingBlock::BreakingBlock(int pX, int pZ) : GameObject()
{
	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->translate(glm::vec3(0, -0.5f, 0));

	_model->setMaterial(new LitMaterial(glm::vec3(0.3f,0.3f,0.3f)));
	_model->setParent(this);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
	_position [0] = pX;
	_position [1] = pZ;
}

void BreakingBlock::breakBlock()
{
	//remove from array
	Level::map->baseTiles [_position [0]] [_position [1]] = (int)nullptr;

	//delete
	setParent(nullptr);
	delete this;
}