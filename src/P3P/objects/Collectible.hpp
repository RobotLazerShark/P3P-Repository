#ifndef COLLECTIBLE_Def
#define COLLECTIBLE_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/ColorMaterial.hpp>

#include <P3P/objects/Player.hpp>

class Collectible : public GameObject
{
public:
	Collectible(int pX, int pZ, std::string pName);
	void collect(int pX, int pZ);
private:
	GameObject* _model;
	std::string _name;
};

#endif