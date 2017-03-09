#ifndef BOSS_Def
#define BOSS_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <P3P/Projectile.hpp>
#include <mge/behaviours/AnimationBehaviour.hpp>


class Boss : public GameObject
{
	public:
		static Boss* singletonInstance;
		Boss(int pX, int pZ);
		~Boss();
		void update(float pStep, bool pUpdateWorldTransform = false) override;
		void damage();
		void removeProjectile (Projectile* pProjectile);
		//Preferably these variables would be private, but they are needed in a globalspace function
		//They use the private name conventions because they should be used as such
		bool _noFire = false;
		AnimationBehaviour* _barrel1Animator = nullptr;
		AnimationBehaviour* _barrel2Animator = nullptr;
		int _position[2] = { 0, 0 };
		std::vector<Projectile*> projectiles;

		void pause(bool active);
		void lookAt(glm::vec3 pos);

	private:
		bool _dead = false;
		bool _deathAnimationPlayed = false;
		GameObject* _model = nullptr;
		LitMaterial* _faceMaterial = nullptr;
		int liveCount = 4;
		float _timer = 0;
		const float SHOOTING_FREQUENCY = 2.0f;
		AnimationBehaviour* _bodyAnimator = nullptr;
		AnimationBehaviour* _cutSceneAnimator = nullptr;

		int _bulletCount = 0;
		int _superAttackSize = 6;
		void shoot();
		void superAttack();

		bool _pause = false;
		glm::vec2  _oldDirection;
};

#endif