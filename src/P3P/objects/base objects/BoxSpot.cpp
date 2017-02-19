#include "P3P/objects/base objects/BoxSpot.hpp"
#include <P3P/Level.hpp>


//Constructor
BoxSpot::BoxSpot (int pX, int pZ) : GameObject ()
{
	//Set up model
	_model = new GameObject ("BoxSpot.obj");
	_model->setMaterial (new LitMaterial ("BoxSpot.png"));
	_model->translate (glm::vec3 (0, -0.05f, 0));
	_model->setParent (this);

	translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
}