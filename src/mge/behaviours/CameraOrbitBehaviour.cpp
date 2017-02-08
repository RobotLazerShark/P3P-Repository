using namespace std;
#include "mge/behaviours/CameraOrbitBehaviour.hpp"


//////////////////////////////|	INITIALIZATION
//Set the required values for this behaviour
CameraOrbitBehaviour::CameraOrbitBehaviour (float pDistanceFromTarget, float pMinTiltAngle, float pMaxTiltAngle, float pMoveSpeed, GameObject* pTargetObject) : AbstractBehaviour ()
{
	_distanceFromTarget = pDistanceFromTarget;
	_minTiltAngle = pMinTiltAngle;
	_maxTiltAngle = pMaxTiltAngle;
	_moveSpeed = pMoveSpeed;
	_targetObject = pTargetObject;
	_rotation = glm::vec3 (0.0f, 0.0f, 0.0f);
	registerForEvent (sf::Event::EventType::MouseMoved);
}


//////////////////////////////|	UPDATING FUNCTIONS
//Keep the camera in a spherical area around our target object
void CameraOrbitBehaviour::update (float pStep)
{
	//Rotate the camera based on mouse input
	if (_curX < _oldX - MIN_MOUSE_MOVEMENT)
	{
		_rotation.y -= _moveSpeed;
	}
	else if (_curX > _oldX + MIN_MOUSE_MOVEMENT)
	{
		_rotation.y += _moveSpeed;
	}
	if (_curY < _oldY - MIN_MOUSE_MOVEMENT)
	{
		_rotation.x -= _moveSpeed;
		if (_rotation.x < _minTiltAngle)
		{
			_rotation.x = _minTiltAngle;
		}
	}
	else if (_curY > _oldY + MIN_MOUSE_MOVEMENT)
	{
		_rotation.x += _moveSpeed;
		if (_rotation.x > _maxTiltAngle)
		{
			_rotation.x = _maxTiltAngle;
		}
	}

	if (JCPPEngine::InputManager::GetKey (sf::Keyboard::Up))
	{
		_distanceFromTarget -= 1;
	}
	else if (JCPPEngine::InputManager::GetKey (sf::Keyboard::Down))
	{
		_distanceFromTarget += 1;
	}

	//Recalculate our transform relative to the targetobject
	glm::mat4 matrix1 = _targetObject->getWorldTransform ();
	//Calculate rotation matrix
	glm::mat4 matrix2 = glm::rotate (_rotation.y, glm::vec3 (0, 1, 0)) * glm::rotate (_rotation.x, glm::vec3 (1, 0, 0)) * glm::rotate (_rotation.z, glm::vec3 (0, 0, 1));
	//Calculate the relative transform matrix
	glm::mat4 matrix3 = glm::translate (glm::vec3 (0, 0, _distanceFromTarget));

	//Combine and apply matrices
	glm::mat4 transform = matrix1 * matrix2 * matrix3;
	_owner->setTransform (transform);
}
//Change camera movement based on mouse movement
void CameraOrbitBehaviour::ProcessEvent (sf::Event pEvent)
{
	if (pEvent.type != sf::Event::MouseMoved)
	{
		return;
	}

	int newX = pEvent.mouseMove.x;
	int newY = pEvent.mouseMove.y;

	//This shuffeling of variables enables continuous camera movement.
	if ((newX < _curX - MIN_MOUSE_MOVEMENT) || (newX > _curX + MIN_MOUSE_MOVEMENT))
	{
		_oldX = _curX;
		_curX = newX;
	}
	else//If the mouse wasn't move far enough, stop moving.
	{
		_oldX = newX;
		_curX = newX;
	}
	if ((newY < _curY - MIN_MOUSE_MOVEMENT) || (newY > _curY + MIN_MOUSE_MOVEMENT))
	{
		_oldY = _curY;
		_curY = newY;
	}
	else
	{
		_oldY = newY;
		_curY = newY;
	}
}