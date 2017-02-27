#include <P3P/objects/base objects/Mirror.hpp>
#include <P3P/Level.hpp>

Mirror::Mirror(int pX, int pZ) : ButtonTarget()
{
	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->setMaterial(new LitMaterial("Mirror.jpg"));
	_model->setParent(this);
	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
}

bool Mirror::setActive(bool pActive)
{
	up = pActive;
	return true;
}