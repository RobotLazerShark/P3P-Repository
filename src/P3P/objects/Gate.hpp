#ifndef GATE_Def
#define GATE_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <P3P/ButtonTarget.hpp>

class Gate : public ButtonTarget
{
	public:
		Gate(int pX, int pZ);
		bool setActive (bool pActive) override;
		int x ();
		int z ();

	private:
		GameObject* _model = nullptr;
		int _position [2] = { 0, 0 };
};

#endif