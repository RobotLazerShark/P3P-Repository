#include <P3P/objects/Box.hpp>
#include <P3P/Level.hpp>
#include <JCPPEngine/Random.hpp>


//Constructor with a tile position
Box::Box (int pX, int pZ, bool pPowered) : GameObject ()
{
	_powered = pPowered;
	//Set up model
	_model = new GameObject ();
	_model->setParent (this);
	_animator = new AnimationBehaviour ({ "BoxLeft.txt", "BoxUp.txt", "BoxRight.txt", "BoxDown.txt" });
	_model->setBehaviour (_animator);
	if (_powered)
	{
		GameObject* subModel = new GameObject ("PowerBox.obj");
		subModel->setMaterial (new LitMaterial ("PowerBox.png"));
		subModel->setParent (_model);
		subModel->rotate (glm::radians (JCPPEngine::Random::Range (1, 3) * 90.0f), glm::vec3 (0, 1, 0));
	}
	else
	{
		GameObject* subModel = new GameObject ("Box.obj");
		subModel->setMaterial (new LitMaterial ("Box.png"));
		subModel->setParent (_model);
		subModel->rotate (glm::radians (JCPPEngine::Random::Range (1, 3) * 90.0f), glm::vec3 (0, 1, 0));
	}

	translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
	_currentTile [0] = pX;
	_currentTile [1] = pZ;
	_oldTile [0] = _currentTile [0];
	_oldTile [1] = _currentTile [1];
}


//////////////////////////////|	ANIMATION
//Function to be called when animations are stopped. It has to be in global space, or we have to pass our object type as well.
void stopFunctionBox (int pAnimIndex, GameObject* pOwner)
{
	Box* box = (Box*)pOwner;
	box->setWorldPosition (glm::vec3 (box->_currentTile [0] * Level::TILESIZE, 0, box->_currentTile [1] * Level::TILESIZE));
}

//Stop any playing animation
void Box::stopAnimation ()
{
	_animator->stopAnimation ();
}


//Move the box by a given amount of tiles
void Box::moveBox (int pX, int pZ, bool pAnimate)
{
    //update position
    _oldTile [0] = _currentTile [0];
    _oldTile [1] = _currentTile [1];
    _currentTile [0] += pX;
    _currentTile [1] += pZ;

    //update object array
    Level::map->objectTiles [_currentTile [0]] [_currentTile [1]] = (int)this;
    Level::map->objectTiles [_oldTile [0]] [_oldTile [1]] = (int)nullptr;

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
	    _animator->playAnimation (animation, false, &stopFunctionBox, this);
    }
    else
    {
	    translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
    }

    if (_powered)
    {
	    //update BoxSpot if needed
	    BoxSpot* boxSpot = dynamic_cast <BoxSpot*> ((GameObject*)Level::map->baseTiles [_currentTile [0]] [_currentTile [1]]);
	    if (boxSpot != nullptr)
	    {
		    boxSpot->taken = true;
	    }
	    boxSpot = dynamic_cast <BoxSpot*> ((GameObject*)Level::map->baseTiles [_oldTile [0]] [_oldTile [1]]);
	    if (boxSpot != nullptr)
	    {
		    boxSpot->taken = false;
	    }
    }
}