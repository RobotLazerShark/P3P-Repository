#include "mge/behaviours/ThirdPersonCameraBehaviour.hpp"

ThirdPersonCameraBehaviour::ThirdPersonCameraBehaviour(float pDistance, float pHeight) : AbstractBehaviour()
{
	_distance = pDistance;
	_height = pHeight;
	if (_distance >= _height)
	{
		_distanceProjectionLength = sqrt(_distance*_distance - _height*_height);
	}
	else
	{
		cout << "invalid input parameters, height cant be greater than distance" << endl;
	}
}

void ThirdPersonCameraBehaviour::update(float pStep)
{
	if (_player == nullptr)
	{
		_player = Player::singletonInstance;
	}
	if (_start && !inTransition)
	{
		//set correct camera start position
		_owner->setWorldPosition(glm::vec3(_player->getActualWorldPosition().x, _height, _player->getActualWorldPosition().z + _distanceProjectionLength));
		//save distanceVector (player--->camera vector)
		_distanceVector = _owner->getLocalPosition() - _player->getActualWorldPosition();
		lookAtPlayer();
		_start = false;
	}

	if (!inTransition) //normal 3d person behaviour
	{
		glm::vec3 correctCameraPosition = _player->getActualWorldPosition() + _distanceVector;
		//if camera position is'nt correct
		if (_owner->getLocalPosition() != correctCameraPosition)
		{
			//adjust camera position towards right position with certain SPEED;
			glm::vec3 movingDirection = glm::normalize(correctCameraPosition - _owner->getLocalPosition());
			movingDirection = movingDirection * pStep * SPEED;

			if (glm::length(movingDirection) < glm::length(correctCameraPosition - _owner->getLocalPosition()))
			{
				_owner->setLocalPosition(_owner->getLocalPosition() + movingDirection);
			}
			else
			{
				_owner->setLocalPosition(correctCameraPosition);
			}

		}
	}
	else //smooth transition to new 3d person position
	{
		if ( glm::length(_transitionDistance * (pStep / _transitionTime)) < glm::distance(_targetTransitionPos, _owner->getWorldPosition()))
		{
			_owner->setWorldPosition(_owner->getWorldPosition() + _transitionDistance * (pStep / _transitionTime));
		}
		else
		{
			inTransition = false;
			_player->blockMovement = false;
		}
	}
}

void ThirdPersonCameraBehaviour::lookAtPlayer()
{
	glm::vec3 forward = glm::normalize(_owner->getLocalPosition() - _player->getWorldPosition());
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
	glm::vec3 up = glm::cross(forward, right);
	_owner->setTransform(glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getLocalPosition(), 1)));
}

void ThirdPersonCameraBehaviour::startTransition(glm::vec3 targetPos, float time)
{
	_player = Player::singletonInstance;
	_player->blockMovement = true;
	_distanceProjectionLength = sqrt(_distance*_distance - _height*_height);
	_distanceVector = glm::vec3(_player->getActualWorldPosition().x, _height, _player->getActualWorldPosition().z + _distanceProjectionLength) - _player->getActualWorldPosition();

	inTransition = true;
	_targetTransitionPos = targetPos + _distanceVector;
	_transitionTime = time;

	_transitionDistance = _targetTransitionPos - _owner->getWorldPosition();
}