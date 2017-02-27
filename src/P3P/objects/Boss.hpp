#ifndef BOSS_Def
#define BOSS_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <P3P/Projectile.hpp>


class Boss : public GameObject
{
	public:
		static Boss* singletonInstance;
		Boss(int pX, int pZ);
		~Boss();
		void update(float pStep, bool pUpdateWorldTransform = false) override;
		void damage();

	private:
		GameObject* _model = nullptr;

		int liveCount = 5;
		int _position[2] = { 0, 0 };
		float _timer = 0;
		const float SHOOTING_FREQUENCY = 1;
		std::vector<Projectile*> projectiles;

		void shoot();
};

#endif