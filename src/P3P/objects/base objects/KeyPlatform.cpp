#include <P3P/objects/base objects/KeyPlatform.hpp>
#include <P3P/Level.hpp>

KeyPlatform::KeyPlatform(int pX, int pZ) : GameObject()
{
	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->translate(glm::vec3(0, -0.5f, 0));
	_model->setMaterial(new LitMaterial("Platform.jpg"));
	_model->setParent(this);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//save position
	arrayPosition.x = pX;
	arrayPosition.y = pZ;
}

void KeyPlatform::setGate(Gate * pGate)
{
	_gate = pGate;
}

void KeyPlatform::update(float pStep, bool pUpdateWorldTransform)
{
	GameObject::update(pStep, pUpdateWorldTransform);

	if (_gate != nullptr)
	{
		//activate gate if it's not activated and something is standing on the platform 
		if (!activated && Level::map->objectTiles[arrayPosition.x][arrayPosition.y] != (int)nullptr)
		{
			activate();
		}
		//deactivate gate if it's activated and nothing is standing on the platform 
		else if (activated && Level::map->objectTiles[arrayPosition.x][arrayPosition.y] == (int)nullptr)
		{
			deActivate();
		}
	}
}

void KeyPlatform::activate()
{
	std::cout << "activate" << std::endl;
	activated = true;
	_gate->open();
}

void KeyPlatform::deActivate()
{
	std::cout << "deactivate" << std::endl;
	if (_gate->close())
	{
		activated = false;
	}
}