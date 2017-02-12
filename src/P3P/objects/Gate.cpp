#include <P3P/objects/Gate.hpp>
#include <P3P/Level.hpp>

Gate::Gate(int pX, int pZ) : GameObject()
{
	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->translate(glm::vec3(0, -0.5f, 0));
	_model->setMaterial(new LitMaterial("Gate.jpg"));
	_model->setParent(this);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//save position
	arrayPosition.x = pX;
	arrayPosition.y = pZ;
}

void Gate::open()
{
	//make gate position in array empty
	Level::map->objectTiles[arrayPosition.x][arrayPosition.y] = (int)nullptr;
}

bool Gate::close()
{
	//if gate tile is taken by door or its empty
	if (Level::map->objectTiles[arrayPosition.x][arrayPosition.y] == (int)this || Level::map->objectTiles[arrayPosition.x][arrayPosition.y] == (int)nullptr)
	{
		//put gat back to array
		Level::map->objectTiles[arrayPosition.x][arrayPosition.y] = (int)this;
		
		return true; //return true to let platofrm know that door succesfully closed
	}
	//if gate tile is take by a box
	else if(dynamic_cast <Player*> ((GameObject*)Level::map->objectTiles[arrayPosition.x][arrayPosition.y]) != nullptr)
	{
		std::cout << "player was killed by a closing gate" << std::endl;
		//reload level
		Level::singletonInstance->reloadLevel(); 
	}
	
	return false; //return false to let platofrm know that door wasnt closed
}