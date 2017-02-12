#ifndef KEYPLATFORM_Def
#define KEYPLATFORM_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>

class Gate;

class KeyPlatform : public GameObject
{
	public:
		KeyPlatform(int pX, int pZ);
		void update(float pStep, bool pUpdateWorldTransform = false) override;
		void setGate(Gate * pGate);
	private:
		GameObject* _model;
		Gate* _gate;
		glm::vec2 arrayPosition;
		bool activated = false;

		void activate();
		void deActivate();
};

#endif