#ifndef BUTTON_Def
#define BUTTON_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <P3P/ButtonTarget.hpp>

class Button : public GameObject
{
	public:
		Button(int pX, int pZ, ButtonTarget* pTarget);
		void update(float pStep, bool pUpdateWorldTransform = false) override;
	private:
		GameObject* _model = nullptr;
		ButtonTarget* _target = nullptr;
		int _position [2] = { 0, 0 };
		bool activated = false;
};

#endif