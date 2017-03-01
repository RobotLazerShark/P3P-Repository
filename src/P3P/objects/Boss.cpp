#include <P3P/objects/Boss.hpp>
#include <P3P/Level.hpp>
#include <mge/materials/TextureMaterial.hpp>

//Static fields
Boss* Boss::singletonInstance = nullptr;

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
	barrel2Model->setParent (_model);
	_barrel2Animator = new AnimationBehaviour ({ "Barrel2Open.txt", "Barrel2Fire.txt" });
	barrel2Model->setBehaviour (_barrel2Animator);
	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//save position
	_position[0] = pX;
	_position[1] = pZ;
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

	_timer += pStep;
	if (_timer >= SHOOTING_FREQUENCY) 
	{
		_timer -= SHOOTING_FREQUENCY;
		shoot();
	}	
}

void Boss::shoot()
{
	Projectile * proj = new Projectile(getWorldPosition(), Player::singletonInstance->_currentTile[0], Player::singletonInstance->_currentTile[1], this);
	proj->setParent(Level::singletonInstance);
	projectiles.push_back(proj);
}

void Boss::damage()
{
	liveCount--;
	if (liveCount <= 0)
	{
		singletonInstance = nullptr;
		setParent(nullptr);
		delete this;
	}
}