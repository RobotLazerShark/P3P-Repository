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
	_currentTile = glm::vec2 (pX, pZ);
	_oldTile = glm::vec2 (_currentTile);
}


//Move the box by a given amount of tiles
void Box::moveBox (int pX, int pZ)
{
    //update position
    _oldTile.x = _currentTile.x;
    _oldTile.y = _currentTile.y;
    _currentTile.x += pX;
    _currentTile.y += pZ;

    //update object array
    Level::map->objectTiles [_currentTile.x] [_currentTile.y] = (int)this;
    Level::map->objectTiles [_oldTile.x] [_oldTile.y] = (int)nullptr;

    //move gameoject
    glm::mat4 currentTrans = getTransform ();
    glm::vec4 currentTransPos = currentTrans[3];
    currentTransPos[0] = currentTransPos[0] + pX * Level::TILESIZE;
    currentTransPos[2] = currentTransPos[2] + pZ * Level::TILESIZE;
    currentTrans[3] = currentTransPos;
    setTransform(currentTrans);

    //update BoxSpot if needed
    BoxSpot* boxSpot = (BoxSpot*)(Level::map->baseTiles [_currentTile.x] [_currentTile.y]);
    if (boxSpot != nullptr)
    {
	    boxSpot->taken = false;
    }
    boxSpot = (BoxSpot*)(Level::map->baseTiles [_oldTile.x] [_oldTile.y]);
    if (boxSpot != nullptr)
    {
	    boxSpot->taken = true;
    }
}