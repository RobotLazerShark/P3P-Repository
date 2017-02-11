#include "P3P/objects/base objects/Floor.hpp"
#include <P3P/Level.hpp>


//Constructor
BreakingBlock::BreakingBlock(int pX, int pZ) : GameObject()
{
	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->translate(glm::vec3(0, -0.5f, 0));

	_model->setMaterial(new ColorMaterial(glm::vec3(0,0,1)));
	_model->setParent(this);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
}

void BreakingBlock::breakBlock(int pX, int pZ)
{
	//remove from array
	Level::map->baseTiles[pX][pZ] = (int)nullptr;

	//delete
	setParent(nullptr);
	delete this;
}