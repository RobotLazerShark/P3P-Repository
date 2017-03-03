#include <P3P/objects/Boss.hpp>
#include <P3P/Level.hpp>
#include <mge/materials/TextureMaterial.hpp>

//Static fields
Boss* Boss::singletonInstance = nullptr;


//////////////////////////////|	ANIMATION
//Stop function you can pass in when playing an animation, that will then be called when the animation finishes playing
void stopFunctionBoss (int pAnimIndex, GameObject* pOwner)
{
	Boss* boss = (Boss*)pOwner;
	switch (pAnimIndex)
	{
		case 0:
		case 1:
			boss->_noFire = false;
			break;
		default:
			break;
	}
}

Boss::Boss(int pX, int pZ) : GameObject()
{
	if (singletonInstance != nullptr)
	{
		delete singletonInstance;
	}
	singletonInstance = this;

	//Set up model
	_model = new GameObject("BossBody.obj");
	_model->translate(glm::vec3(0, 2, 0));
	_model->setMaterial(new LitMaterial("Boss.jpg"));
	_model->setParent(this);
	_bodyAnimator = new AnimationBehaviour({ "BossFloat.txt" });
	_model->setBehaviour(_bodyAnimator);
	_bodyAnimator->playAnimation(0, true);

	GameObject* shadow = new GameObject ("ShadowPlane.obj");
	shadow->setMaterial (new TextureMaterial ("BossShadow.png"));
	shadow->setParent (_model);
	shadow->translate (glm::vec3 (0, -1.9f, 0));
	GameObject* faceModel = new GameObject ("BossFace.obj");
	faceModel->setParent (_model);
	_faceMaterial = new LitMaterial ("BossFace1.png");
	faceModel->setMaterial (_faceMaterial);
	GameObject* barrel1Model = new GameObject ("BossBarrelBase.obj");
	barrel1Model->setMaterial (new LitMaterial ("BossBarrelBase.png"));
	barrel1Model->setParent (_model);
	_barrel1Animator = new AnimationBehaviour ({ "Barrel1Open.txt", "Barrel1Fire.txt" });
	barrel1Model->setBehaviour (_barrel1Animator);
	GameObject* barrel2Model = new GameObject ("BossBarrelEnd.obj");
	barrel2Model->setMaterial (new LitMaterial ("BossBarrelEnd.png"));
	barrel2Model->setParent (barrel1Model);
	_barrel2Animator = new AnimationBehaviour ({ "Barrel2Open.txt", "Barrel2Fire.txt" });
	barrel2Model->setBehaviour (_barrel2Animator);
	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//save position
	_position[0] = pX;
	_position[1] = pZ;

	//Play startup animation, put guns out
	_barrel1Animator->playAnimation (0, false, false);
	_barrel2Animator->playAnimation (0, false, false, &stopFunctionBoss, this);
	_noFire = true;
}

Boss::~Boss()
{
	singletonInstance = nullptr;
	for (Projectile * proj : projectiles)
	{
		proj->stopUpdating = true;
		proj->setParent (nullptr);
		delete proj;
	}
	projectiles.clear();
	GameObject::~GameObject();
}

void Boss::removeProjectile (Projectile* pProjectile)
{
	std::vector<Projectile*>::iterator end = projectiles.end ();
	for (std::vector<Projectile*>::iterator i = projectiles.begin (); i != end; ++ i)
	{
		if (*i == pProjectile)
		{
			projectiles.erase (i);
			return;
		}
	}
}

void Boss::update(float pStep, bool pUpdateWorldTransform)
{
	GameObject::update(pStep, pUpdateWorldTransform);

	if (_dead)
	{
		setParent (nullptr);
		delete this;
	}

	if (!_noFire)
	{
		_timer += pStep;
		if (_timer >= SHOOTING_FREQUENCY)
		{
			_timer -= SHOOTING_FREQUENCY;
			shoot();
		}
	}
}

void Boss::shoot()
{
	Projectile * proj = new Projectile(getWorldPosition(), Player::singletonInstance->_currentTile[0], Player::singletonInstance->_currentTile[1], this);
	proj->setParent(Level::singletonInstance);
	projectiles.push_back(proj);
	//Play shooting animation
	_barrel1Animator->playAnimation (1, false, false);
	_barrel2Animator->playAnimation (1, false, false, &stopFunctionBoss, this);
	_noFire = true;
}

void Boss::damage()
{
	liveCount--;
	if (liveCount <= 0)
	{
		singletonInstance = nullptr;
		_dead = true;
	}
}