#include <P3P/Projectile.hpp>
#include <P3P/Level.hpp>
#include <P3P/objects/Boss.hpp>

Projectile::Projectile(glm::vec3 pos, int targetX, int targetZ, Boss * pOwner) : GameObject()
{
	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->scale(0.5f);
	_model->setMaterial(new LitMaterial(glm::vec3(1, 0, 0)));
	_model->setParent(this);
	
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
	glm::vec3 forward = glm::normalize(_target - getWorldPosition());
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
	glm::vec3 up = glm::cross(forward, right);
	setTransform(
		glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(getWorldPosition(), 1))
		);
}

void Projectile::update(float pStep, bool pUpdateWorldTransform)
{
	if (stopUpdating)
	{
		explode();
	}
	else
	{
		GameObject::update(pStep, pUpdateWorldTransform);

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
					_owner->damage();
					explode();
				}
				//move to owner
				translate(glm::vec3(0, 0, -SPEED*pStep));
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
			}
			//if reached player
			else if (Player::singletonInstance->_currentTile[0] == _targetTile[0] && Player::singletonInstance->_currentTile[1] == _targetTile[1])
			{
				Player::singletonInstance->die();
				explode();
			}
			//if reached empty floor
			else
			{
				explode();
			}
		}
	}
}

void Projectile::explode()
{
	setParent(nullptr);
	delete this;
}