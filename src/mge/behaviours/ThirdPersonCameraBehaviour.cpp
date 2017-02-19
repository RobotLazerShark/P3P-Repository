#include "mge/behaviours/ThirdPersonCameraBehaviour.hpp"
#include <P3P/Level.hpp>

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
	if (_start)
	{
		//set correct camera start position
		_owner->setWorldPosition(glm::vec3(_player->getWorldPosition().x, _height, _player->getWorldPosition().z + _distanceProjectionLength));
		//Set initial values
		glm::vec3 playerPos = glm::vec3 (_player->_currentTile [0] * Level::TILESIZE, 0, _player->_currentTile [1] * Level::TILESIZE);
		_curTargetPos [0] = playerPos.x;
		_curTargetPos [1] = playerPos.z;
		_newTargetPos [0] = _player->_currentTile [0];
		_newTargetPos [1] = _player->_currentTile [1];
		//save distanceVector (player--->camera vector)
		_distanceVector = _owner->getLocalPosition() - _player->getWorldPosition();
		lookAtPlayer();
		_start = false;
	}

	glm::vec3 correctCameraPosition = _player->getWorldPosition() + _distanceVector;
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

	_newTargetPos [0] = _player->_currentTile [0];
	_newTargetPos [1] = _player->_currentTile [1];
	if (_newTargetPos [0] != _curTargetPos [0] || _newTargetPos [1] != _curTargetPos [1]) 
	{
		glm::vec3 playerPos = glm::vec3 (_curTargetPos [0] + (_newTargetPos [0] - _curTargetPos [0]) * (1 - _smoothing), 0, _curTargetPos [1] + (_newTargetPos [1] - _curTargetPos [1]) * (1 - _smoothing)) * Level::TILESIZE;
		_curTargetPos [0] = playerPos.x;
		_curTargetPos [1] = playerPos.z;

		//look at player
		glm::vec3 forward = glm::normalize (_owner->getLocalPosition () - playerPos);
		glm::vec3 right = glm::normalize (glm::cross (glm::vec3 (0, 1, 0), forward));
		glm::vec3 up = glm::cross (forward, right);
		_owner->setTransform (glm::mat4 (glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getLocalPosition(), 1)));
	}
}

void ThirdPersonCameraBehaviour::lookAtPlayer()
{
	glm::vec3 forward = glm::normalize(_owner->getLocalPosition() - _player->getWorldPosition());
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
	glm::vec3 up = glm::cross(forward, right);
	_owner->setTransform(glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getLocalPosition(), 1)));
}