//Ensure we only include this header once
#ifndef CameraOrbitBehaviour_Def
#define CameraOrbitBehaviour_Def

//Include files
#include "mge/core/GameObject.hpp"
#include <glm.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include <JCPPEngine/AbstractListener.hpp>
#include <SFML/Graphics.hpp>
#include <JCPPEngine/InputManager.hpp>


//This behaviour is meant to be attached to a camera, but it should work on other gameobjects as well.
//WARNING: it is assumed that the camera will not be moved by any other means than this script.
//WARNING: this script will not work if the targetobject's transform does not match it's world transform. Don't target objects that have a parent other than the world.
class CameraOrbitBehaviour : public AbstractBehaviour, JCPPEngine::AbstractListener
{
	public:
		CameraOrbitBehaviour (float pDistanceFromTarget, float pMinTiltAngle, float pMaxTiltAngle, float pMoveSpeed, GameObject* pTargetObject);
		virtual void update (float pStep);
		virtual void ProcessEvent (sf::Event pEvent);
	private:
		GameObject* _targetObject;
		float _distanceFromTarget;
		float _minTiltAngle;
		float _maxTiltAngle;
		float _moveSpeed;
		const int MIN_MOUSE_MOVEMENT = 15;
		int _curX;
		int _curY;
		int _oldX;
		int _oldY;
		glm::vec3 _rotation;
		glm::mat4 _transform;
};

#endif