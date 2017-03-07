#ifndef THIRDPERSONCAMERABEHAVIOUR_H
#define THIRDPERSONCAMERABEHAVIOUR_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "P3P/objects/Player.hpp"
#include <Math.h>

class ThirdPersonCameraBehaviour : public AbstractBehaviour
{
	public:
		ThirdPersonCameraBehaviour(float pDistance, float pHeight);
		virtual void update(float pStep);
		void startTransition(glm::vec3 targetPos, float time);

	private:
		Player * _player = nullptr;
		float _distance = 6;
		float _height = 5;
		float _distanceProjectionLength = 0;
		bool _start = true;
		glm::vec3 _distanceVector; 
		const float SPEED = 2; //unit per sec

		bool inTransition = false;
		glm::vec3 _targetTransitionPos = glm::vec3(0, 0, 0);
		glm::vec3 _transitionDistance = glm::vec3(0, 0, 0);
		float _transitionTime = 0;


		void lookAtPlayer();
};

#endif