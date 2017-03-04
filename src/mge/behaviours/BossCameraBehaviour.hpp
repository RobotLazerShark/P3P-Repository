#ifndef BOSSCAMERABEHAVIOUR_H
#define BOSSCAMERABEHAVIOUR_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "P3P/objects/Player.hpp"
#include "P3P/BossCameraAnchor.hpp"

class BossCameraBehaviour : public AbstractBehaviour
{
	public:
		BossCameraBehaviour();
		virtual void update(float pStep);
		void startTransition(glm::vec3 targetPos);
		void startLookTransition(bool lookAtPlayer, glm::vec3 newTarget = glm::vec3(0, 0, 0));

		std::vector<BossCameraAnchor*> anchors;
		BossCameraAnchor * currentAnchor = nullptr;

		glm::vec3 _currentLookAtPos; //needed to be public for mirror

	private:
		Player * _player = nullptr;

		//looking at player variables
		glm::vec3 _lookingTarget;
		bool _start = true;
		float _curTargetPos[2] = { 0, 0 };
		int _newTargetPos[2] = { 0, 0 };
		glm::vec3 _initialPosition;
		float _smoothing = 0.9f;

		//looking at object variables
		bool _lookTargetIsPlayer = true;
		glm::vec3 _lookTargetPos;
		float FOLLOWING_SPEED = 6;
		bool _inLookTransition = false;

		//transition variables
		const float SPEED = 15; //units per sec
		glm::vec3  _transitionDirection;
		glm::vec3  _targetTransitionPos;
		bool _inTransition = false;

		void lookAt(glm::vec3 target);
		
};

#endif
