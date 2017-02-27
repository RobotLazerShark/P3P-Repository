#include <P3P/objects/Boss.hpp>
#include <P3P/Level.hpp>

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
	_model = new GameObject("cube_flat.obj");
	_model->translate(glm::vec3(0, 2, 0));
	_model->setMaterial(new LitMaterial("Boss.jpg"));
	_model->setParent(this);
	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//save position
	_position[0] = pX;
	_position[1] = pZ;
}

Boss::~Boss()
{
	for (Projectile * proj : projectiles)
	{
		proj->stopUpdating = true;
	}
	projectiles.clear();
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
		Level::singletonInstance->map->objectTiles[_position[0]][_position[1]] = (int)nullptr;
		setParent(nullptr);
		delete this;
	}
}