#include <P3P/Projectile.hpp>
#include <P3P/Level.hpp>
#include <P3P/objects/Boss.hpp>
#include <mge/materials/TextureMaterial.hpp>
#include <JCPPEngine/SoundManager.hpp>

Projectile::Projectile(glm::vec3 pos, int targetX, int targetZ, Boss * pOwner, bool pNormalProjectile) : GameObject()
{
	//Set up model
	_model = new GameObject("Projectile.obj");
	_model->badScale(glm::vec3(3, 3, 1));
	_model->setMaterial(new TextureMaterial("Projectile.png"));
	_model->setParent(this);
	_targetIcon = new GameObject("ShadowPlane.obj");
	_targetIcon->setMaterial(new TextureMaterial("ProjectileTarget.png"));
	_targetIcon->setParent(Level::singletonInstance->transparencyLayer1);
	_targetIcon->translate(glm::vec3(targetX, 0.08f, targetZ));

	setWorldPosition(pos);
	translate(glm::vec3(0, 1, 0));

	//save owner
	_owner = pOwner;

	//save start position
	_startPosition = pos;

	//save target
	_targetTile[0] = targetX;
	_targetTile[1] = targetZ;
	_target = glm::vec3(targetX * Level::TILESIZE, 0, targetZ * Level::TILESIZE);

	//set rotation
	if (pNormalProjectile)
	{
		glm::vec3 forward = glm::normalize(_target - getWorldPosition());
		glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
		glm::vec3 up = glm::cross(forward, right);
		setTransform(
			glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(getWorldPosition(), 1))
			);
	}
	else
	{
		_model->rotate(glm::radians(90.0f), glm::vec3(1, 0, 0));
	}
	_normalProjectile = pNormalProjectile;

}
Projectile::~Projectile ()
{
	if (_targetIcon != nullptr)
	{
		_targetIcon->setParent (nullptr);
		delete _targetIcon;
		_targetIcon = nullptr;
	}
	GameObject::~GameObject ();
}

void Projectile::update(float pStep, bool pUpdateWorldTransform)
{
	GameObject::update(pStep, pUpdateWorldTransform);
	if (stopUpdating)
	{
		explode();
	}
	else
	{
		if (_normalProjectile)
		{
			//if distance to target bigger than step
			if (glm::distance(getWorldPosition(), _target) >= SPEED*pStep)
			{
				if (!reflected)
				{
					//move to target
					translate(glm::vec3(0, 0, SPEED*pStep));
				}
				else
				{
					//if reached boss
					if (glm::distance(getWorldPosition(), _target) < 1)
					{
						_owner->removeProjectile(this);
						_owner->damage();
						setParent(nullptr);
						delete this;
					}
					else
					{
						//move to owner
						translate(glm::vec3(0, 0, -SPEED*pStep));
					}
				}
			}
			else
			{
				Mirror* mirror = dynamic_cast <Mirror*> ((GameObject*)Level::map->baseTiles[_targetTile[0]][_targetTile[1]]);
				//if reached mirror
				if (mirror != nullptr && mirror->up && !mirror->broken)
				{
					mirror->broken = true;
					mirror->setActive(false);
					_target = _startPosition;
					reflected = true;
					_targetIcon->setParent(nullptr);
					delete _targetIcon;
					_targetIcon = nullptr;
					JCPPEngine::SoundManager::PlaySound(new sf::Sound(*JCPPEngine::SoundManager::GetBuffer("sounds/ProjectileBounce.wav")));
				}
				//if reached player
				else if (Player::singletonInstance->_currentTile[0] == _targetTile[0] && Player::singletonInstance->_currentTile[1] == _targetTile[1])
				{
					explode();
					//Player::singletonInstance->die();
				}
				//if reached empty floor
				else
				{
					explode();
				}
			}
		}
		else //super attack projectile
		{
			translate(glm::vec3(0, -0.1, 0));
			if (getWorldPosition().y <= 0.5f)
			{
				if (Player::singletonInstance->_currentTile[0] == _targetTile[0] && Player::singletonInstance->_currentTile[1] == _targetTile[1])
				{
					Player::singletonInstance->die();
				}
				explode();
			}
		}
	}
}

void Projectile::explode()
{
	_owner->removeProjectile (this);
	setParent(nullptr);
	if (_targetIcon != nullptr)
	{
		_targetIcon->setParent (nullptr);
		delete _targetIcon;
		_targetIcon = nullptr;
	}
	delete this;
}