#include "mge/behaviours/BossCameraBehaviour.hpp"
#include <P3P/Level.hpp>

BossCameraBehaviour::BossCameraBehaviour()
{

}

void BossCameraBehaviour::update(float pStep)
{
	if (_player == nullptr)
	{
		_player = Player::singletonInstance;
	}
	if (_start)
	{
		_initialPosition = _owner->getWorldPosition();

		//Set initial values
		glm::vec3 playerPos = glm::vec3(_player->_currentTile[0] * Level::TILESIZE, 0, _player->_currentTile[1] * Level::TILESIZE);
		_curTargetPos[0] = playerPos.x;
		_curTargetPos[1] = playerPos.z;
		_newTargetPos[0] = _player->_currentTile[0];
		_newTargetPos[1] = _player->_currentTile[1];

		//look at player
		lookAt(playerPos);

		_start = false;
		return;
	}

	//change anchor if entered new area
	glm::vec3 playerPos = _player->getWorldPosition();
	cout << anchors.size()<<endl;
	for (BossCameraAnchor * anchor : anchors)
	{
		if (currentAnchor != anchor)
		{
			if (playerPos.x >= anchor->rangeX[0] && playerPos.x <= anchor->rangeX[1]
				&& playerPos.z >= anchor->rangeY[0] && playerPos.z <= anchor->rangeY[1])
			{
				startTransition(anchor->position);
				_player->controlsMode = anchor->controlsMode;
				currentAnchor = anchor;

			}
		}
	}
	
	//update transition
	if(_inTransition)
	{
		glm::vec3 stepVector = pStep * SPEED * glm::normalize(_transitionDirection);

		//if step is smaller than distance to target
		if (glm::length(stepVector) < glm::distance(_targetTransitionPos, _owner->getWorldPosition()))
		{
			//move by 1 step
			_owner->setWorldPosition(_owner->getWorldPosition() + stepVector);
		}
		else
		{
			//move to target position
			_owner->setWorldPosition(_targetTransitionPos);
			_player->_noMove = false;
			_inTransition = false;
			
		}
	}

	if (_inLookTransition)
	{
		glm::vec3 distanceVec = _lookTargetPos - _currentLookAtPos;
		glm::vec3 stepVector = glm::normalize(distanceVec)*FOLLOWING_SPEED * pStep;
		if (glm::length(stepVector) < glm::length(distanceVec))
		{
			lookAt(_currentLookAtPos + stepVector);
		}
		else
		{
			lookAt(_lookTargetPos);
			_player->_noMove = false;
			cout << "unlocked movement" << endl;
			_inLookTransition = false;		
		}
	}
	else
	{
		if (_lookTargetIsPlayer)
		{
			_newTargetPos[0] = _player->_currentTile[0];
			_newTargetPos[1] = _player->_currentTile[1];
			if (_newTargetPos[0] != _curTargetPos[0] || _newTargetPos[1] != _curTargetPos[1])
			{
				glm::vec3 playerPos = glm::vec3(_curTargetPos[0] + (_newTargetPos[0] - _curTargetPos[0]) * (1 - _smoothing), 0, _curTargetPos[1] + (_newTargetPos[1] - _curTargetPos[1]) * (1 - _smoothing)) * Level::TILESIZE;
				_curTargetPos[0] = playerPos.x;
				_curTargetPos[1] = playerPos.z;

				//look at player
				lookAt(playerPos);
			}
		}
		else
		{
			//look at target
			lookAt(_lookTargetPos);
		}
	}
}

void BossCameraBehaviour::startTransition(glm::vec3 targetPos)
{
	_initialPosition = targetPos;

	_transitionDirection = targetPos - _owner->getWorldPosition();
	_targetTransitionPos = targetPos;

	_inTransition = true;

	Player::singletonInstance->_noMove = true;
}

void BossCameraBehaviour::startLookTransition(bool lookAtPlayer, glm::vec3 newTarget)
{
	if (lookAtPlayer)
	{
		_lookTargetIsPlayer = true;
		_lookTargetPos = glm::vec3(_player->_currentTile[0], 0, _player->_currentTile[1]) * Level::TILESIZE; // not sure if this line gets corrent palyer position
	}
	else
	{
		_lookTargetIsPlayer = false;
		_lookTargetPos = newTarget;
	}

	_inLookTransition = true;

	Player::singletonInstance->_noMove = true;
	cout << "blocked movement" << endl;
}

void BossCameraBehaviour::lookAt(glm::vec3 target)
{
	_currentLookAtPos = target;

	glm::vec3 forward = glm::normalize(_owner->getLocalPosition() - target);
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
	glm::vec3 up = glm::cross(forward, right);
	_owner->setTransform(glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getLocalPosition(), 1)));
}