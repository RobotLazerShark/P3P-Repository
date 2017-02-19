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
	private:
		float _distance = 6;
		float _height = 5;
		float _distanceProjectionLength = 0;
		Player * _player = nullptr;
		float _curTargetPos [2] = { 0, 0 };
		int _newTargetPos [2] = { 0, 0 };
		float _smoothing = 0.98f;
		bool _start = true;
		glm::vec3 _distanceVector;
		void lookAtPlayer();
		const float SPEED = 2.0f; //unit per sec
};

#endif