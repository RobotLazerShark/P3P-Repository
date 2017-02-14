#ifndef BREAKINGBLOCK_Def
#define BREAKINGBLOCK_Def


//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <mge/behaviours/AnimationBehaviour.hpp>


class BreakingBlock : public GameObject
{
public:
	BreakingBlock(int pX, int pZ);
	void update (float pStep, bool pUpdateWorldTransform = false) override;
	//Preferably these variables would be private, but they are needed in a globalspace function
	//They use the private name conventions because they should be used as such
	int _position [2] = { 0, 0 };
	bool _delete = false;
private:
	GameObject* _model;
	int _breakLevel = 0;
	bool _updating = false;
	AnimationBehaviour* _animator;
};

#endif