#ifndef ROTATINGBEHAVIOUR_H
#define ROTATINGBEHAVIOUR_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include <glm.hpp>

/**
 * Simply rotates the object around its origin
 */
class RotatingBehaviour : public AbstractBehaviour
{
	public:
		RotatingBehaviour (float pXAngle, float pYAngle, float pZAngle);
		virtual ~RotatingBehaviour();

		virtual void update (float pStep) override;
	private:
		glm::vec3 _rotation;

};

#endif // ROTATINGBEHAVIOUR_H
