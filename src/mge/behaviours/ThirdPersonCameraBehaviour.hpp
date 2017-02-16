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
		bool _start = true;
		glm::vec3 _distanceVector;
		void lookAtPlayer();
		const float SPEED = 1.25f; //unit per sec
};

#endif