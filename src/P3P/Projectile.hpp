#ifndef PROJECTILE_Def
#define PROJECTILE_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <P3P/objects/Player.hpp>
#include <P3P/objects/base objects/Mirror.hpp>

class Boss;

class Projectile : public GameObject
{
	public:
		Projectile(glm::vec3 pos, int targetX, int targetZ, Boss * pOwner);
		~Projectile ();
		void update(float pStep, bool pUpdateWorldTransform = false) override;
		void explode();

		bool stopUpdating = false;

	private:
		Boss * _owner = nullptr;
		GameObject* _model = nullptr;
		GameObject* _targetIcon = nullptr;
		int _targetTile[2];
		glm::vec3 _startPosition = glm::vec3(0, 0, 0);
		glm::vec3 _target = glm::vec3(0, 0, 0);
		const float SPEED = 4; //units per sec
		bool reflected = false;
};

#endif