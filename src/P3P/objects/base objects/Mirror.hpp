#ifndef MIRROR_Def
#define MIRROR_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <P3P/ButtonTarget.hpp>
#include <mge/behaviours/AnimationBehaviour.hpp>

//Forward declaration
class BossCameraBehaviour;

class Mirror : public ButtonTarget
{
	public:
		Mirror(int pX, int pZ);
		bool setActive(bool pActive);
		void update(float pStep, bool pUpdateWorldTransform = false) override;
		bool up = false;
		bool broken = false;
	private:
		GameObject* _model = nullptr;
		bool _active = false;
		AnimationBehaviour* _animator = nullptr;
		BossCameraBehaviour* behaviour = nullptr;
		bool _facingBoss = false;

		bool _cameraIsMovingTowardsMirror = false;
		bool _cameraLookingAtMirror = false;
		float _lookedAtFor = 0;
		float _lookingDuration = 1.5f;
		glm::vec3 _cameraPositionBeforeTransition;
};

#endif