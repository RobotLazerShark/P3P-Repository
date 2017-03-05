#include "mge/behaviours/FollowBehaviour.hpp"
#include <P3P/Level.hpp>
#include "P3P/objects/Player.hpp"

PercentageFollowBehaviour::PercentageFollowBehaviour(float pIntensity, float pX, float pY, float pZ) : AbstractBehaviour()
{
	//set intensity
	_intensity = pIntensity / 100.0f;
	_initialPosition = glm::vec3 (pX, pY, pZ);
}

void PercentageFollowBehaviour::update(float pStep)
{
	if (_start)
	{
		_owner->setLocalPosition (_initialPosition);
		_player = Player::singletonInstance;
		//Set initial values
		glm::vec3 playerPos = glm::vec3 (_player->_currentTile [0], 0, _player->_currentTile [1]);
		_curTargetPos [0] = playerPos.x;
		_curTargetPos [1] = playerPos.z;
		_newTargetPos [0] = _player->_currentTile [0];
		_newTargetPos [1] = _player->_currentTile [1];
		//move
		glm::vec3 newPosition = _initialPosition;
		newPosition.x = _initialPosition.x * (1 - _intensity) + playerPos.x * _intensity;
		newPosition.z = _initialPosition.z * (1 - _intensity) + playerPos.z * _intensity;
		if (newPosition.z < playerPos.z + 3)
		{
			newPosition.z = playerPos.z + 3;
		}
		_owner->setLocalPosition (newPosition);
		//look at player
		glm::vec3 forward = glm::normalize (_owner->getLocalPosition () - playerPos);
		glm::vec3 right = glm::normalize (glm::cross (glm::vec3 (0, 1, 0), forward));
		glm::vec3 up = glm::cross (forward, right);
		_owner->setTransform (glm::mat4 (glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getLocalPosition(), 1)));
		_start = false;
		return;
	}

	_newTargetPos [0] = _player->_currentTile [0];
	_newTargetPos [1] = _player->_currentTile [1];
	if (_newTargetPos [0] != _curTargetPos [0] || _newTargetPos [1] != _curTargetPos [1]) 
	{
		glm::vec3 playerPos = glm::vec3 (_curTargetPos [0] + (_newTargetPos [0] - _curTargetPos [0]) * (1 - _smoothing), 0, _curTargetPos [1] + (_newTargetPos [1] - _curTargetPos [1]) * (1 - _smoothing));
		_curTargetPos [0] = playerPos.x;
		_curTargetPos [1] = playerPos.z;

		//move
		glm::vec3 newPosition = _initialPosition;
		newPosition.x = _initialPosition.x * (1 - _intensity) + playerPos.x * _intensity;
		newPosition.z = _initialPosition.z * (1 - _intensity) + playerPos.z * _intensity;
		if (newPosition.z < playerPos.z + 3)
		{
			newPosition.z = playerPos.z + 3;
		}
		_owner->setLocalPosition (newPosition);

		//look at player
		glm::vec3 forward = glm::normalize (_owner->getLocalPosition () - playerPos);
		glm::vec3 right = glm::normalize (glm::cross (glm::vec3 (0, 1, 0), forward));
		glm::vec3 up = glm::cross (forward, right);
		_owner->setTransform (glm::mat4 (glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getLocalPosition(), 1)));
	}
}