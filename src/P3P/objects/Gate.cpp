#include <P3P/objects/Gate.hpp>
#include <P3P/Level.hpp>


//Constructor
Gate::Gate(int pX, int pZ) : GameObject()
{
	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->translate(glm::vec3(0, 0.5f, 0));
	_model->setMaterial(new LitMaterial("Gate.jpg"));
	_model->setParent(this);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//save position
	_position [0] = pX;
	_position [1] = pZ;
}


//Get x position
int Gate::x ()
{
	return _position [0];
}

//Get z position
int Gate::z ()
{
	return _position [1];
}


//Based on whether or not the connect button is pressed, open and close the gate
bool Gate::setActive (bool pActive)
{
	if (pActive)
	{
		//make gate position in array empty
		Level::map->objectTiles[_position [0]][_position [1]] = (int)nullptr;
	}
	else
	{
		//if gate tile is empty
		if (Level::map->objectTiles[_position [0]][_position [1]] == (int)nullptr)
		{
			//put gate back into array
			Level::map->objectTiles[_position [0]][_position [1]] = (int)this;
			return false; //return false so the button will keep calling this function until the gate can close, or the box/player goes off the button
		}
		//if gate tile is taken by the player
		else if(dynamic_cast <Player*> ((GameObject*)Level::map->objectTiles[_position [0]][_position [1]]) != nullptr)
		{
			//Kill the player
			Player::singletonInstance->die ();
		}
	}
	return true;//return true to let the button know the switch succeeded
}