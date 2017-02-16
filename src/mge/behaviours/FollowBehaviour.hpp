#ifndef FOLLOWBEHAVIOUR_H
#define FOLLOWBEHAVIOUR_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include <glm.hpp>

class Player;

class PercentageFollowBehaviour : public AbstractBehaviour
{
	public:
		PercentageFollowBehaviour(float pIntensity, float pX, float pY, float pZ);
		void update(float pStep) override;
		
	private:
		float _intensity;//1 max, 0 min
		glm::vec3 _initialPosition;
		bool _start = true;
		Player* _player = nullptr;
		float _curTargetPos [2] = { 0, 0 };
		int _newTargetPos [2] = { 0, 0 };
		float _smoothing = 0.9f;
};

#endif
