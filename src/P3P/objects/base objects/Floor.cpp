#include "P3P/objects/base objects/Floor.hpp"
#include <P3P/Level.hpp>


//Constructor
Floor::Floor (int pX, int pZ) : GameObject ()
{
	//Set up model
	_model = new GameObject ("cube_flat.obj");
	_model->setMaterial (new LitMaterial (glm::vec3 (1, 0, 0)));
	_model->translate (glm::vec3 (0, -0.5f, 0));
	_model->setParent (this);

	translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
}