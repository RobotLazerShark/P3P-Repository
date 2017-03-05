#include <P3P/objects/base objects/Mirror.hpp>
#include <P3P/Level.hpp>
#include <P3P/objects/Boss.hpp>
#include <mge/core/World.hpp>
#include <mge/objects/Camera.hpp>
#include <mge/behaviours/BossCameraBehaviour.hpp>

Mirror::Mirror(int pX, int pZ) : ButtonTarget()
{
	//Set up model
	GameObject* mirrorBase = new GameObject("MirrorBase.obj");
	mirrorBase->setMaterial(new LitMaterial("MirrorBase.png"));
	mirrorBase->setParent(this);
	GameObject* floortile = new GameObject ("Floor1.obj");
	floortile->setMaterial (new LitMaterial ("Floor1.png"));
	floortile->translate (glm::vec3 (0, -0.05f, 0));
	floortile->setParent (mirrorBase);
	_model = new GameObject ("MirrorTop.obj");
	_model->setMaterial (new LitMaterial ("MirrorBase.png"));
	_model->setParent (mirrorBase);
	GameObject* mirrorOffset = new GameObject ();
	mirrorOffset->setParent (_model);
	mirrorOffset->translate (glm::vec3 (0, 0.7f, 0));
	GameObject* mirror = new GameObject ("Mirror.obj");
	mirror->setMaterial (new LitMaterial ("Mirror.png", 250));
	mirror->setParent (mirrorOffset);
	mirror->translate (glm::vec3 (0, -0.55f, 0));
	mirror->rotate (glm::radians (-90.0f), glm::vec3 (0, 0, 1));
	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//animator
	_animator = new AnimationBehaviour({ "MirrorUp.txt","MirrorDown.txt" });
	mirror->setBehaviour(_animator);
}

void animationEndFunction(int pAnimIndex, GameObject* pOwner)
{
	Mirror* mirror = (Mirror*)pOwner;
	switch (pAnimIndex)
	{
		case 1:
			mirror->up = false;
			break;
		default:
			break;
	}
}

bool Mirror::setActive(bool pActive)
{
	_active = pActive;
	if (_active && !up && !broken)
	{
		up = true;
		//start look transition
		_cameraPositionBeforeTransition = World::singletonInstance->getMainCamera()->getWorldPosition();
		glm::vec3 bossPos = Boss::singletonInstance->getWorldPosition();
		bossPos.y = 4;
		((BossCameraBehaviour*)World::singletonInstance->getMainCamera()->getBehaviour())->startLookTransition(false, getWorldPosition());
		((BossCameraBehaviour*)World::singletonInstance->getMainCamera()->getBehaviour())->startTransition(bossPos);

		_cameraIsMovingTowardsMirror = true;
	}
	if (!_active)
	{
		_animator->playAnimation(1, false, false, &animationEndFunction, this);
	}
	return true;
}

void Mirror::update(float pStep, bool pUpdateWorldTransform)
{
	GameObject::update(pStep, pUpdateWorldTransform);
	
	if (!broken && Boss::singletonInstance != nullptr && !_facingBoss)
	{
		glm::vec3 bossPosFlat = Boss::singletonInstance->getWorldPosition();
		bossPosFlat.y = 0;
		glm::vec3 posFlat = getWorldPosition();
		posFlat.y = 0;

		glm::vec3 forward = glm::normalize(glm::cross(glm::vec3(0, 1, 0), glm::normalize(posFlat - bossPosFlat)));  
		glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
		glm::vec3 up = glm::cross(forward, right);
		_model->setTransform(glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(0,0,0, 1)));

		_facingBoss = true;
	}

	//if camera is done mobing towards the mirror play animation
	if (_cameraIsMovingTowardsMirror)
	{
		if (((BossCameraBehaviour*)World::singletonInstance->getMainCamera()->getBehaviour())->_currentLookAtPos == getWorldPosition())
		{
			_animator->playAnimation(0, false, false, &animationEndFunction, this);

			_cameraLookingAtMirror = true;
			_cameraIsMovingTowardsMirror = false;
		}
	}
	
	//make camera look at mirror for 1.5 seconds
	if (_cameraLookingAtMirror)
	{
		_lookedAtFor += pStep;
		if (_lookedAtFor >= _lookingDuration)
		{
			((BossCameraBehaviour*)World::singletonInstance->getMainCamera()->getBehaviour())->startLookTransition(true);
			((BossCameraBehaviour*)World::singletonInstance->getMainCamera()->getBehaviour())->startTransition(_cameraPositionBeforeTransition);
			_cameraLookingAtMirror = false;
		}
		else
		{
			Player::singletonInstance->_noMove = true;
		}
	}
}