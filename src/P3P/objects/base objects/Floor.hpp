#ifndef Floor_Def
#define Floor_Def


//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>


class Floor : public GameObject
{
	public:
		Floor (int pX, int pZ);
	private:
		GameObject* _model;
};

#endif