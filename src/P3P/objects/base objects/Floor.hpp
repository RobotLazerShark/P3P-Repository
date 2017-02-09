<<<<<<< HEAD
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

=======
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

>>>>>>> 05a65edaed0d726361052543950587d121f9c233
#endif