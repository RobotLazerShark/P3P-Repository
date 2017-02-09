#include <P3P/objects/Box.hpp>
#include <P3P/Level.hpp>


//Constructor with a tile position
Box::Box (int pX, int pZ) : GameObject ()
{
	//Set up model
	_model = new GameObject ("cube_flat.obj");
	_model->setMaterial (new LitMaterial (glm::vec3 (0, 0, 1)));
	_model->translate (glm::vec3 (0, 0.5f, 0));
	_model->setParent (this);

	translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
	_currentTile [0] = pX;
	_currentTile [1] = pZ;
	_oldTile [0] = _currentTile [0];
	_oldTile [1] = _currentTile [1];
}


//Move the box by a given amount of tiles
void Box::moveBox (int pX, int pZ)
{
    //update position
    _oldTile [0] = _currentTile [0];
    _oldTile [1] = _currentTile [1];
    _currentTile [0] += pX;
    _currentTile [1] += pZ;

    //update object array
    Level::map->objectTiles [_currentTile [0]] [_currentTile [1]] = (int)this;
    Level::map->objectTiles [_oldTile [0]] [_oldTile [1]] = (int)nullptr;

    //move gameoject
    translate (glm::vec3 (pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

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