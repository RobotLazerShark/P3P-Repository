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
	_model->setMaterial(new LitMaterial("Boss.jpg"));
	_model->setParent(modelParent);
	_bodyAnimator = new AnimationBehaviour({ "BossFloat.txt" });
	_model->setBehaviour(_bodyAnimator);
	_bodyAnimator->playAnimation(0, true);

	GameObject* shadow = new GameObject ("ShadowPlane.obj");
	shadow->setMaterial (new TextureMaterial ("BossShadow.png"));
	shadow->setParent (modelParent);
	shadow->translate (glm::vec3 (0, 0.1f, 0));
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
		JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/BossDeath.wav")));
		setParent (nullptr);
		delete this;
	}

	if (!_noFire)
	{
		_timer += pStep;
		if (_timer >= SHOOTING_FREQUENCY)
		{
			_timer -= SHOOTING_FREQUENCY;
			if (_bulletCount <= 5)
			{
				shoot();
				_bulletCount++;
			}
			else
			{
				superAttack();
				_bulletCount = 0;
			}
		}
	}
}

void Boss::shoot()
{
	JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/BossFire.wav")));
	Projectile * proj = new Projectile(getWorldPosition(), Player::singletonInstance->_currentTile[0], Player::singletonInstance->_currentTile[1], this, true);
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
	if (liveCount <= 0)
	{
		singletonInstance = nullptr;
		_dead = true;
	}
}

void Boss::superAttack()
{
	int attackPosition = JCPPEngine::Random::Range(1, 4);
	int reverser;
	if (JCPPEngine::Random::Range(0, 1)) {
		reverser = -1;
	}
	else {
		reverser = 1;
	}

	int attackStartPosition[2] = { 0,0 };
	int tileSize = Level::TILESIZE;

	//get starting tile of the super attack
	switch (attackPosition)
	{
	case 1:
		attackStartPosition[0] = _position[0];
		attackStartPosition[1] = _position[1] - (_superAttackSize / 2)* reverser;
		break;
	case 2:
		attackStartPosition[0] = _position[0] - (_superAttackSize / 2)* reverser;
		attackStartPosition[1] = _position[1] - (_superAttackSize / 2)* reverser;
		break;
	case 3:
		attackStartPosition[0] = _position[0] - (_superAttackSize / 2)* reverser;
		attackStartPosition[1] = _position[1];
		break;
	case 4:
		attackStartPosition[0] = _position[0] - (_superAttackSize / 2)* reverser;
		attackStartPosition[1] = _position[1] + (_superAttackSize / 2)* reverser;
		break;
	}
	glm::vec3 projStartingPosition = glm::vec3(attackStartPosition[0] * tileSize, 5, attackStartPosition[1] * tileSize);

	//spawn projectiles
	for (int i = 0; i < _superAttackSize; i++)
	{
		int skipBoss = 0;
		if (i == 3) //if we are on top of the boss - skip this tile
		{
			skipBoss = tileSize;
		}
		if (i != 0)
		{
			switch (attackPosition)
			{
			case 1:
				projStartingPosition = projStartingPosition + glm::vec3(0, 0.5f, reverser* tileSize + reverser*skipBoss);
				break;
			case 2:
				projStartingPosition = projStartingPosition + glm::vec3(reverser* tileSize + reverser*skipBoss, 0.5f, reverser* tileSize + reverser*skipBoss);
				break;
			case 3:
				projStartingPosition = projStartingPosition + glm::vec3((reverser* tileSize) + reverser*skipBoss, 0.5f, 0);
				break;
			case 4:
				projStartingPosition = projStartingPosition + glm::vec3(reverser* tileSize + reverser*skipBoss, 0.5f, -(reverser* tileSize + reverser*skipBoss));
				break;
			}
		}

		Projectile * proj = new Projectile(projStartingPosition, projStartingPosition.x / tileSize, projStartingPosition.z / tileSize, this, false);
		proj->setParent(Level::singletonInstance);
		projectiles.push_back(proj);

		//Play shooting animation, should be changed to boss super attack animation
		_barrel1Animator->playAnimation(1, false, false);
		_barrel2Animator->playAnimation(1, false, false, &stopFunctionBoss, this);
		_noFire = true;
	}
}