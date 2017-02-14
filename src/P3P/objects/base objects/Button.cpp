#include <P3P/objects/base objects/Button.hpp>
#include <P3P/Level.hpp>

Button::Button (int pX, int pZ, ButtonTarget* pTarget) : GameObject()
{
	_target = pTarget;

	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->translate(glm::vec3(0, -0.5f, 0));
	_model->setMaterial(new LitMaterial(glm::vec3 (0.5f,1,0.5f)));
	_model->setParent(this);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//save position
	_position [0] = pX;
	_position [1] = pZ;
}

void Button::update(float pStep, bool pUpdateWorldTransform)
{
	if (_target != nullptr)
	{
		//activate buttontarget if it's not activated and something is standing on the platform 
		if (!activated && Level::map->objectTiles[_position [0]][_position [1]] != (int)nullptr)
		{
			activated = _target->setActive (true);//Only switch state if switch succeeded
		}
		//deactivate buttontarget if it's activated and nothing is standing on the platform 
		else if (activated && Level::map->objectTiles[_position [0]][_position [1]] == (int)nullptr)
		{
			activated = !_target->setActive (false);//Only switch state if switch succeeded
		}
	}

	GameObject::update(pStep, pUpdateWorldTransform);
}
