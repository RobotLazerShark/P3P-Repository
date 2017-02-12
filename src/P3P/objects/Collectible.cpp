#include "P3P/objects/Collectible.hpp"
#include <P3P/Level.hpp>

Collectible::Collectible(int pX, int pZ, std::string pName) : GameObject()
{
	_name = pName;

	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->setMaterial(new ColorMaterial(glm::vec3(1,0.5f,0)));
	_model->translate(glm::vec3(0, 0.5f, 0));
	_model->setParent(this);
	_model->scale(0.5f);
	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
}

void Collectible::collect(int pX, int pZ)
{
	//collect item only if you have never collected it
	bool itemAlreadyCollected = false;
	for (int i = 0; i < Player::collectedItems.size(); i++)
	{
		if (Player::collectedItems[i] == _name)
		{
			itemAlreadyCollected = true;
		}
	}
	if (!itemAlreadyCollected)
	{
		std::cout << "adding item" << endl;
		Player::collectedItems.push_back(_name);
	}
	
	//remove from array
	Level::map->objectTiles[pX][pZ] = (int)nullptr;

	//delete
	setParent(nullptr);
	delete this;
}

