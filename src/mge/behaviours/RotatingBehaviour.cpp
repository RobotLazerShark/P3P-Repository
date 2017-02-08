#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/core/GameObject.hpp"

RotatingBehaviour::RotatingBehaviour (float pXAngle, float pYAngle, float pZAngle) : AbstractBehaviour ()
{
	_rotation = glm::vec3 (pXAngle, pYAngle, pZAngle);
}

RotatingBehaviour::~RotatingBehaviour ()
{
	//dtor
}

void RotatingBehaviour::update (float pStep)
{
	_owner->rotate (pStep * _rotation.y, glm::vec3 (0, 1, 0));
	_owner->rotate (pStep * _rotation.x, glm::vec3 (1, 0, 0));
	_owner->rotate (pStep * _rotation.z, glm::vec3 (0, 0, 1));
}
