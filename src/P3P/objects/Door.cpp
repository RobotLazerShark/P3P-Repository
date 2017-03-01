#include "P3P/objects/Door.hpp"
#include <mge/core/World.hpp>
#include <P3P/Level.hpp>


//Constructor
Door::Door (int pX, int pZ, int pNextLevelNumber, int pOrientation) : GameObject ()
{
	_nextLevelNumber = pNextLevelNumber;

	//Set up model
	_model = new GameObject ();
	GameObject* shadow = new GameObject ("ShadowPlane.obj");
	shadow->setMaterial (new TextureMaterial ("DoorShadow.png"));
	shadow->badScale (glm::vec3 (1, 1, 2));
	shadow->translate (glm::vec3 (0, 0, 0.25f));
	shadow->setParent (_model);
	switch (pOrientation)
	{
		case 1:
			//Door facing right
			Level::map->objectTiles [pX] [pZ+1] = (int)this;
			break;
		case 2:
			//Door facing down
			_model->rotate (glm::radians (-90.0f), glm::vec3 (0, 1, 0));
			Level::map->objectTiles [pX-1] [pZ] = (int)this;
			break;
		case 3:
			//Door facing left
			_model->rotate (glm::radians (-180.0f), glm::vec3 (0, 1, 0));
			Level::map->objectTiles [pX] [pZ-1] = (int)this;
			break;
		default:
			break;
	}
	_orientation = pOrientation;
	_model->setParent (this);
	_model->translate(glm::vec3 (0,0.06f,0));
	GameObject* backside = new GameObject ("plane.obj");
	_portal = new TextureMaterial ("Portal.png", 1, true);
	backside->setMaterial (_portal);
	backside->rotate (glm::radians (90.0f), glm::vec3 (0, 1, 0));
	backside->rotate (glm::radians (90.0f), glm::vec3 (1, 0, 0));
	backside->translate (glm::vec3 (-0.5, -0.265f, -1));
	backside->badScale (glm::vec3 (0.09f, 1, 0.09f));
	backside->setParent (_model);
	GameObject* supports = new GameObject ("DoorSupport.obj");
	supports->setMaterial (new LitMaterial ("Door.png"));
	supports->setParent (_model);
	GameObject* doorLeft = new GameObject ("DoorLeft.obj");
	doorLeft->setMaterial (new LitMaterial ("Door.png"));
	doorLeft->setParent (_model);
	_doorLeftAnimator = new AnimationBehaviour ({ "DoorLeft.txt" }, false);
	doorLeft->setBehaviour (_doorLeftAnimator);
	GameObject* doorRight = new GameObject ("DoorRight.obj");
	doorRight->setMaterial (new LitMaterial ("Door.png"));
	doorRight->setParent (_model);
	_doorRightAnimator = new AnimationBehaviour ({ "DoorRight.txt" }, false);
	doorRight->setBehaviour (_doorRightAnimator);
	GameObject* doorBarsLeft = new GameObject ("DoorBarsLeft.obj");
	doorBarsLeft->setMaterial (new LitMaterial ("Door.png"));
	doorBarsLeft->setParent (_model);
	_barsLeftAnimator = new AnimationBehaviour ({ "DoorBarsLeft.txt" }, false);
	doorBarsLeft->setBehaviour (_barsLeftAnimator);
	GameObject* doorBarsRight = new GameObject ("DoorBarsRight.obj");
	doorBarsRight->setMaterial (new LitMaterial ("Door.png"));
	doorBarsRight->setParent (_model);
	_barsRightAnimator = new AnimationBehaviour ({ "DoorBarsRight.txt" }, false);
	doorBarsRight->setBehaviour (_barsRightAnimator);

	_position [0] = pX;
	_position [1] = pZ;
	translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
}
Door::~Door ()
{
	Level::map->objectTiles [_position [0]] [_position [1]] = (int)nullptr;
	switch (_orientation)
	{
		case 1:
			Level::map->objectTiles [_position [0]] [_position [1]+1] = (int)nullptr;
			break;
		case 2:
			Level::map->objectTiles [_position [0]-1] [_position [1]] = (int)nullptr;
			break;
		case 3:
			Level::map->objectTiles [_position [0]] [_position [1]-1] = (int)nullptr;
			break;
		default:
			break;
	}
}


void Door::update (float pStep, bool pUpdateWorldTransform)
{
	GameObject::update (pStep, pUpdateWorldTransform);
}


//Stop function for animation
void stopFunctionDoor (int pAnimIndex, GameObject* pOwner)
{
	Door* door = (Door*)pOwner;
	Level::singletonInstance->hud->disable();
}

//Load the level this door leads to
bool Door:: enter ()
{
	if (_open)
	{
		Level::singletonInstance->loadLevel (_nextLevelNumber);
	}
	return _open;
}

//Open the door
void Door::open ()
{
    if (_open)
    {
	return;
    }
    if (Level::singletonInstance->levelKey () >= _nextLevelNumber && Level::singletonInstance->levelCompleted)
    {
	_barsLeftAnimator->playAnimation (0);
	_barsRightAnimator->playAnimation (0);
	_doorLeftAnimator->playAnimation (0);
	_doorRightAnimator->playAnimation (0);
	_open = true;
    }
}


//Get the level this door is supposed to load
int Door::levelNumber ()
{
	return _nextLevelNumber;
}