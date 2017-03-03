#include <P3P/objects/base objects/Mirror.hpp>
#include <P3P/Level.hpp>

Mirror::Mirror(int pX, int pZ) : ButtonTarget()
{
	//Set up model
	_model = new GameObject("MirrorBase.obj");
	_model->setMaterial(new LitMaterial("MirrorBase.png"));
	_model->setParent(this);
	GameObject* mirrorTop = new GameObject ("MirrorTop.obj");
	mirrorTop->setMaterial (new LitMaterial ("MirrorBase.png"));
	mirrorTop->setParent (_model);
	GameObject* mirrorOffset = new GameObject ();
	mirrorOffset->setParent (mirrorTop);
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
		_animator->playAnimation(0, false, false, &animationEndFunction, this);
	}
	if (!_active)
	{
		_animator->playAnimation(1, false, false, &animationEndFunction, this);
	}
	return true;
}