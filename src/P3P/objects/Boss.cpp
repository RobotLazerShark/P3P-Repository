#include <P3P/objects/Boss.hpp>
#include <P3P/Level.hpp>
#include <mge/materials/TextureMaterial.hpp>
#include <JCPPEngine/SoundManager.hpp>
#include <JCPPEngine/Random.hpp>

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
void stopFunctionEnd (int pAnimIndex, GameObject* pOwner)
{
	Boss* boss = (Boss*)pOwner;
	Projectile* proj = nullptr;
	sf::Sound* sound = nullptr;
	switch (pAnimIndex)
	{
		case 1:
			JCPPEngine::SoundManager::StopMusicLoop (0);
			JCPPEngine::SoundManager::Clean ();//Stop all playing sounds
			JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/RecoveringMemory.wav")));
			Level::singletonInstance->startFade ();
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
	//Play bosslevel music
	JCPPEngine::SoundManager::PlayMusicLoop ("sounds/BackgroundLoop2.wav");

	//Set up model
	GameObject* modelParent = new GameObject ();//Prevent shadow animating with boss
	modelParent->setParent (this);
	_model = new GameObject("BossBody.obj");
	_model->translate(glm::vec3(0, 2, 0));
	_model->setMaterial(new LitMaterial("BossBody.png"));
	_model->setParent(modelParent);
	_bodyAnimator = new AnimationBehaviour({ "BossFloat.txt", "BossFinal.txt" });
	_model->setBehaviour(_bodyAnimator);
	_bodyAnimator->playAnimation(0, true);

	_shadow = new GameObject ("ShadowPlane.obj");
	_shadow->setMaterial (new TextureMaterial ("BossShadow.png"));
	_shadow->setParent (modelParent);
	_shadow->translate (glm::vec3 (0, 0.1f, 0));
	GameObject* faceModel = new GameObject ("BossFace.obj");
	faceModel->setParent (_model);
	_faceMaterial = new LitMaterial ("BossFace1.png");
	faceModel->setMaterial (_faceMaterial);
	GameObject* barrel1Model = new GameObject ("BossBarrelBase.obj");
	barrel1Model->setMaterial (new LitMaterial ("BossGun.png"));
	barrel1Model->setParent (_model);
	_barrel1Animator = new AnimationBehaviour ({ "Barrel1Open.txt", "Barrel1Fire.txt" });
	barrel1Model->setBehaviour (_barrel1Animator);
	GameObject* barrel2Model = new GameObject ("BossBarrelEnd.obj");
	barrel2Model->setMaterial (new LitMaterial ("BossGun.png"));
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
	JCPPEngine::SoundManager::StopMusicLoop (0);
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
		if (_shadow != nullptr)
		{
			Player::singletonInstance->_noMove = true;
			JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/BossDeath.wav")));
			_shadow->setParent (nullptr);
			delete _shadow;
			_shadow = nullptr;
			_bodyAnimator->playAnimation (1, false, false, &stopFunctionEnd, this);
		}
	}
	else if (!_noFire)
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
	sf::Sound* sound = new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/BossFire.wav"));
	sound->setPitch (1 + (JCPPEngine::Random::Value () - 0.5f) * 0.5f);
	sound->setVolume (40);
	JCPPEngine::SoundManager::PlaySound (sound);
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
	JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/BossHurt.wav")));
	liveCount--;
	if (liveCount == 3)
	{
		_faceMaterial->SetTexture ("BossFace2.png");
	}
	else if (liveCount == 1)
	{
		_faceMaterial->SetTexture ("BossFace3.png");
	}
	else if (liveCount <= 0)
	{
		_faceMaterial->SetTexture ("BossFace0.png");
		singletonInstance = nullptr;
		_dead = true;
		for (Projectile * proj : projectiles)
		{
			proj->stopUpdating = true;
			proj->setParent (nullptr);
			delete proj;
		}
		projectiles.clear();
	}
}