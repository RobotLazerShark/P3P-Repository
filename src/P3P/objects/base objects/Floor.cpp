#include "P3P/objects/base objects/Floor.hpp"
#include <P3P/Level.hpp>
#include <JCPPEngine/Random.hpp>


//Constructor
Floor::Floor (int pX, int pZ, int pType) : GameObject ()
{
	//Set up model
	LitMaterial* mat;
	switch (pType)
	{
		case 1:
			_model = new GameObject ("Floor1.obj");
			_model->setMaterial (new LitMaterial ("Floor1.png"));
			_model->translate (glm::vec3 (0, -0.05, 0));
			break;
		default:
			int tile = JCPPEngine::Random::Range (1, 10);
			tile = (tile <= 7) ? 1 : JCPPEngine::Random::Range (2, 3);
			_model = new GameObject ("Floor"+to_string (tile)+".obj");
			_model->setMaterial (new LitMaterial ("Floor"+to_string (tile)+".png"));
			_model->translate (glm::vec3 (0, -0.05f, 0));
			break;
	}
	_model->setParent (this);
	_model->rotate (glm::radians (JCPPEngine::Random::Range (0, 3) * 90.0f), glm::vec3 (0, 1, 0));

	translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
}