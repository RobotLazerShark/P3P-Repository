#ifndef GATE_Def
#define GATE_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>

class Gate : public GameObject
{
	public:
		Gate(int pX, int pZ);
		void open();
		bool close();

	private:
		GameObject* _model;
		glm::vec2 arrayPosition;
};

#endif