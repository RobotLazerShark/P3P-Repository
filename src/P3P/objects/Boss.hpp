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
	private:
		bool _dead = false;
		GameObject* _model = nullptr;
		LitMaterial* _faceMaterial = nullptr;
		int liveCount = 1;
		int _position[2] = { 0, 0 };
		float _timer = 0;
		const float SHOOTING_FREQUENCY = 1;
		std::vector<Projectile*> projectiles;
		AnimationBehaviour* _barrel1Animator = nullptr;
		AnimationBehaviour* _barrel2Animator = nullptr;
		void shoot();
};

#endif