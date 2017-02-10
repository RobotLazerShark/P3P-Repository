#include "P3P/objects/base objects/Floor.hpp"
#include <P3P/Level.hpp>


//Constructor
Floor::Floor (int pX, int pZ, int pType) : GameObject ()
{
	//Set up model
	_model = new GameObject ("cube_flat.obj");
	LitMaterial* mat;
	switch (pType)
	{
		case 1:
			mat = new LitMaterial ("Floor1.jpg");
			mat->SetTextureScale (0.75f);
			break;
		case 2:
			mat = new LitMaterial ("Floor2.jpg");
			mat->SetTextureScale (0.75f);
			break;
		default:
			mat = new LitMaterial (glm::vec3 (1, 0, 0));
			break;
	}
	_model->setMaterial (mat);
	_model->translate (glm::vec3 (0, -0.5f, 0));
	_model->setParent (this);

	translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
}