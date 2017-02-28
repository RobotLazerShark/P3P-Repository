#include <P3P/objects/Plug.hpp>
#include <P3P/Level.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

Plug::Plug(int pX, int pZ, int pOrientation) : GameObject()
{
	//Set up model
	_model = new GameObject();
	_model->setParent(this);
	_animator = new AnimationBehaviour({ "BoxLeft.txt", "BoxUp.txt", "BoxRight.txt", "BoxDown.txt","Plug.txt" });
	_model->setBehaviour(_animator);

	GameObject * subModel = new GameObject("cube_flat.obj");
	subModel->setMaterial(new LitMaterial("Plug.jpg"));
	subModel->setParent(_model);
	subModel->translate(glm::vec3(0, 0.5f, 0));

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//save orientation
	_orientation = pOrientation;
	switch (_orientation)
	{
	case 2:
		subModel->rotate(M_PI, glm::vec3(0, 1, 0));
		break;
	case 3:
		subModel->rotate(-M_PI/2, glm::vec3(0, 1, 0));
		break;
	case 4:
		subModel->rotate(M_PI/2, glm::vec3(0, 1, 0));
		break;
	}

	//save array position
	_currentTile[0] = pX;
	_currentTile[1] = pZ;
	_oldTile[0] = _currentTile[0];
	_oldTile[1] = _currentTile[1];
}

void stopFunctionPlug(int pAnimIndex, GameObject* pOwner)
{
	Plug* plug = (Plug*)pOwner;
	plug->setWorldPosition(glm::vec3(plug->_currentTile[0] * Level::TILESIZE, 0, plug->_currentTile[1] * Level::TILESIZE));

	if (pAnimIndex != 4)
	{
		plug->_model->setLocalPosition(glm::vec3(0, 0, 0));
		if (plug->_funcOwner != nullptr)
		{
			GameObject* tempOwner = plug->_funcOwner;
			void (*tempFunc) (int, GameObject*) = plug->_stopFunc;
			plug->_funcOwner = nullptr;
			plug->_stopFunc = nullptr;
			tempFunc (pAnimIndex, tempOwner);
		}
	}
}

bool Plug::move(int pX, int pZ, bool pAnimate) //return false if can't move(when plugged)
{
	if (!plugged)
	{
		//update position
		_oldTile[0] = _currentTile[0];
		_oldTile[1] = _currentTile[1];
		_currentTile[0] += pX;
		_currentTile[1] += pZ;

		//update object array
		Level::map->objectTiles[_currentTile[0]][_currentTile[1]] = (int)this;
		Level::map->objectTiles[_oldTile[0]][_oldTile[1]] = (int)nullptr;

		if (pAnimate)
		{		
			int animation = 0;
			if (pX == 0 && pZ != 0)
			{
				animation = 2 + pZ;
			}
			else if (pZ == 0 && pX != 0)
			{
				animation = 1 + pX;
			}
			_animator->playAnimation(animation, false, true, &stopFunctionPlug, this);
		}
		else
		{
			translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
		}
		return true;
	}
	else
		return false;
}
bool Plug::move(int pX, int pZ, bool pAnimate, void (*pFuncPtr) (int, GameObject*), GameObject* pFuncOwner) //return false if can't move(when plugged)
{
	if (!plugged)
	{
		//update position
		_oldTile[0] = _currentTile[0];
		_oldTile[1] = _currentTile[1];
		_currentTile[0] += pX;
		_currentTile[1] += pZ;

		//update object array
		Level::map->objectTiles[_currentTile[0]][_currentTile[1]] = (int)this;
		Level::map->objectTiles[_oldTile[0]][_oldTile[1]] = (int)nullptr;

		if (pAnimate)
		{
			_funcOwner = pFuncOwner;
			_stopFunc = pFuncPtr;
			int animation = 0;
			if (pX == 0 && pZ != 0)
			{
				animation = 2 + pZ;
			}
			else if (pZ == 0 && pX != 0)
			{
				animation = 1 + pX;
			}
			_animator->playAnimation(animation, false, true, &stopFunctionPlug, this);
		}
		else
		{
			translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
		}
		return true;
	}
	else
		return false;
}

void Plug::update(float pStep, bool pUpdateWorldTransform)
{
	GameObject::update(pStep, pUpdateWorldTransform);
	if (plugged && !_animator->isPlaying() && !playedPluggingAnimation)
	{
		_animator-> playAnimation(4, false, false);
		playedPluggingAnimation = true;
	}
}

void Plug::stopAnimation ()
{
	_animator->stopAnimation ();
}