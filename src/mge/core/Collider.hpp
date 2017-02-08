#ifndef Collider_Def
#define Collider_Def


//Include files
#include <glm.hpp>
#include <cmath>

class Collider
{

	public:
		Collider (glm::vec3 pHalfExtents, glm::vec3 pCenterOffset, bool pBigBox, bool pStatic);//Note: only axis-aligned boxcolliders
		Collider (float pRadius, glm::vec3 pCenterOffset, bool pStatic);
		void CalculateWorldExtents (const glm::mat4 &pModelMatrix);//Note: only call when the collider is changed.
		glm::vec3 worldCenterOffset ();
		glm::vec3 worldHalfExtents ();
		float worldRadius ();
		int colliderType ();//1] axis-aligned box	2] sphere
		bool isStatic ();//Flag for resolving collisions. True means this object will not be moved when resolving collisions.
	private:
		glm::vec3 _centerOffset;
		glm::vec4 _worldCenterOffset;
		glm::vec3 _halfExtents = glm::vec3 (0, 0, 0);
		glm::vec3 _worldHalfExtents = glm::vec3 (0, 0, 0);
		float _radius = 0.0f;
		float _worldRadius = 0.0f;
		int _colliderType;
		bool _bigBox;
		bool _static = true;

};
#endif