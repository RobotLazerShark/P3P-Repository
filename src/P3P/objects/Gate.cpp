#include <P3P/objects/Gate.hpp>
#include <P3P/Level.hpp>
#include <P3P/objects/Player.hpp>


//Constructor
Gate::Gate(int pX, int pZ) : ButtonTarget ()
{
	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->translate(glm::vec3(0, 0.5f, 0));
	_model->setParent(this);
	GameObject* subModel = new GameObject ("cube_flat.obj");
	subModel->setMaterial(new LitMaterial("Gate.jpg"));
	subModel->setParent (_model);
	_animator = new AnimationBehaviour ({ "GateUp.txt", "GateDown.txt" }, false);
	subModel->setBehaviour (_animator);

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
		_animator->playAnimation (0);
	}
	else
	{
		//if gate tile is empty
		if (Level::map->objectTiles[_position [0]][_position [1]] == (int)nullptr)
		{
			//put gate back into array
			Level::map->objectTiles[_position [0]][_position [1]] = (int)this;
			_animator->playAnimation (1);
		}
		//if gate tile is taken by the player
		else if(Player::singletonInstance->_currentTile [0] == _position [0] && Player::singletonInstance->_currentTile [1] == _position [1])
		{
			_animator->playAnimation (1);
			//Kill the player
			Player::singletonInstance->die ();
		}
		else
		{
			return false; //return false so the button will keep calling this function until the gate can close, or the box/player goes off the button
		}
	}
	return true;//return true to let the button know the switch succeeded
}