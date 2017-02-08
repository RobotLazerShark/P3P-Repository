#include <mge/core/Collider.hpp>


//////////////////////////////|	INSTANCE MANAGEMENT
//Constructor for axis-aligned box collider
Collider::Collider (glm::vec3 pHalfExtents, glm::vec3 pCenterOffset, bool pBigBox, bool pStatic)
{
	_halfExtents = pHalfExtents;
	_worldHalfExtents = pHalfExtents;
	_centerOffset = pCenterOffset;//offset from the center of the gameobject this collider belongs to
	_worldCenterOffset = glm::vec4 (pCenterOffset, 1);
	_colliderType = 1;
	_bigBox = pBigBox;//Whether we use the smallest, or biggest possible box collider
	_static = pStatic;//Whether this object will be moved when it collides
}
//Constructor for sphere collider
Collider::Collider (float pRadius, glm::vec3 pCenterOffset, bool pStatic)
{
	_centerOffset = pCenterOffset;//offset from the center of the gameobject this collider belongs to
	_worldCenterOffset = glm::vec4 (pCenterOffset, 1);
	_radius = pRadius;
	_worldRadius = pRadius;
	_colliderType = 2;
	_static = pStatic;//Whether this object will be moved when it collides
}


//////////////////////////////|	CALCULATIONS
//Recalculate the collider's halfextents (if it's a box collider).
void Collider::CalculateWorldExtents (const glm::mat4 &pModelMatrix)
{
	_worldCenterOffset = pModelMatrix * glm::vec4 (_centerOffset, 1);
	if (_colliderType == 2)
	{
		float axisLength = _radius / sqrt (3);
		glm::vec4 point = glm::vec4 (axisLength, axisLength, axisLength, 0);//Position should not affect the size of the collider
		point = pModelMatrix * point;
		_worldRadius = glm::length (point);
		return;
	}

	if (!_colliderType == 1)
	{
		return;
	}

	if (_bigBox)
	{
		glm::vec4 points [8];
		_worldHalfExtents = glm::vec3 (0, 0, 0);
		for (int i = 0; i < 8; i ++)
		{
			//Calculate the worldspace coördinates of the cornerpoints of the box collider.
			points [i] = glm::vec4 (0, 0, 0, 0);//Position should not affect the size of the collider
			points [i].x += (i%2 == 1) ? -_halfExtents.x : _halfExtents.x;
			points [i].y += (i%4 > 1) ? -_halfExtents.y : _halfExtents.y;
			points [i].z += (i%8 > 3) ? -_halfExtents.z : _halfExtents.z;
			points [i] = pModelMatrix * points [i];
			//Set the worldspace halfextents to their correct size.
			if (points [i].x > _worldHalfExtents.x)
			{
				_worldHalfExtents.x = points [i].x;
			}
			if (points [i].y > _worldHalfExtents.y)
			{
				_worldHalfExtents.y = points [i].y;
			}
			if (points [i].z > _worldHalfExtents.z)
			{
				_worldHalfExtents.z = points [i].z;
			}
		}
	}
	else
	{
		glm::vec4 points [6];
		_worldHalfExtents = glm::vec3 (0, 0, 0);
		for (int i = 0; i < 6; i ++)
		{
			//Calculate the worldspace coördinates of the center points of the box colliders planes.
			points [i] = glm::vec4 (0, 0, 0, 0);//Position should not affect the size of the collider
			points [i] [i/2] += (i%2 == 1) ? -_halfExtents [i/2] : _halfExtents [i/2];
			points [i] = pModelMatrix * points [i];
			//Set the worldspace halfextents to their correct size.
			if (points [i].x > _worldHalfExtents.x)
			{
				_worldHalfExtents.x = points [i].x;
			}
			if (points [i].y > _worldHalfExtents.y)
			{
				_worldHalfExtents.y = points [i].y;
			}
			if (points [i].z > _worldHalfExtents.z)
			{
				_worldHalfExtents.z = points [i].z;
			}
		}
	}
}


//////////////////////////////|	GETTERS
//Get the box collider's worldspace half extents (all zero if not a box collider)
glm::vec3 Collider::worldHalfExtents ()
{
	return glm::vec3 (_worldHalfExtents);//Copy values to prevent accidental alterations
}
//Get the collider's worldspace offset
glm::vec3 Collider::worldCenterOffset ()
{
	return glm::vec3 (_worldCenterOffset);//Copy values to prevent accidental alterations
}
//Get the sphere collider's worldspace radius (zero if not a sphere collider)
float Collider::worldRadius ()
{
	return _worldRadius;
}
//Get the number indicating which type of collider this is
int Collider::colliderType ()
{
	return _colliderType;
}
//Get the flag that determines whether or not this object should be moved when resolving collisions
bool Collider::isStatic ()
{
	return _static;
}