#ifndef PLUG_Def
#define PLUG_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <mge/behaviours/AnimationBehaviour.hpp>
#include <P3P/Moveable.hpp>

class Plug : public GameObject, public Moveable
{
	public:
		Plug(int pX, int pZ, int pOrientation);
		void update(float pStep, bool pUpdateWorldTransform = false) override;
		bool move(int pX, int pZ, bool pAnimate = true) override;
	          bool move (int pX, int pZ, bool pAnimate, void (*pFuncPtr) (int, GameObject*), GameObject* pFuncOwner) override;
		void stopAnimation () override;

		GameObject* _model = nullptr;
		AnimationBehaviour* _animator = nullptr;
		AnimationBehaviour* _subAnimator = nullptr;
		int _orientation = 0;
		int _currentTile[2] = { 0, 0 };
		int _oldTile[2] = { 0, 0 };
		bool plugged = false;
	private:
		bool playedPluggingAnimation = false;
};

#endif