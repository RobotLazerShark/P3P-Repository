#ifndef COLLECTIBLE_Def
#define COLLECTIBLE_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>

//We can extend this class for different types of collectables if neccesary
class Collectable : public GameObject
{
public:
	Collectable(int pX, int pZ, std::string pName);
	std::string getName ();
	virtual void collect ();
protected:
	GameObject* _model;
	std::string _name;
	int _position [2] = { 0, 0 };
	bool _collected = false;
};

#endif