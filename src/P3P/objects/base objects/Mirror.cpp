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
	mirrorOffset->translate (glm::vec3 (0, 0.6f, 0));
	GameObject* mirror = new GameObject ("Mirror.obj");
	mirror->setMaterial (new LitMaterial ("Mirror.png", 250));
	mirror->setParent (mirrorOffset);
	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//animator
	_animator = new AnimationBehaviour({ "MirrorUp.txt","MirrorDown.txt" });
	_model->setBehaviour(_animator);
}

void animationEndFunctionUp(int pAnimIndex, GameObject* pOwner)
{
	Mirror* mirror = (Mirror*)pOwner;
	mirror->translate(glm::vec3(0, 0.8f, 0));
}
void animationEndFunctionDown(int pAnimIndex, GameObject* pOwner)
{
	Mirror* mirror = (Mirror*)pOwner;
	mirror->translate(glm::vec3(0, -1, 0));
}

bool Mirror::setActive(bool pActive)
{
	_active = pActive;
	if (_active && !up && !broken)
	{
		up = true;
		_animator->playAnimation(0, false, &animationEndFunctionUp, this);
	}
	if (!_active)
	{
		_animator->playAnimation(1, false, &animationEndFunctionDown, this);
	}
	return true;
}