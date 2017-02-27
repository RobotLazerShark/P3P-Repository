#ifndef MIRROR_Def
#define MIRROR_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <P3P/ButtonTarget.hpp>

class Mirror : public ButtonTarget
{
	public:
		Mirror(int pX, int pZ);
		bool setActive(bool pActive);
		bool up = true;

	private:
		GameObject* _model = nullptr;
};

#endif