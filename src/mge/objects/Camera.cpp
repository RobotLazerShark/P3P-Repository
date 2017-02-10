#include <iostream>
using namespace std;

#include <glm.hpp>
#include "mge/objects/Camera.hpp"

Camera::Camera( std::string pName, glm::vec3 pPosition, glm::mat4 pProjectionMatrix )
:	GameObject(pName, pPosition), _projection(pProjectionMatrix)
{
}
Camera::Camera (float pAngle) : GameObject ("Camera", glm::vec3 (0, 0, 0))
{
	_projection = glm::perspective (glm::radians (pAngle), 16.0f / 9.0f, 0.1f, 1000.0f);
}

Camera::~Camera()
{
	//dtor
}

glm::mat4& Camera::getProjection() {
    return _projection;
}

