#ifndef BREAKINGBLOCK_Def
#define BREAKINGBLOCK_Def


//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>


class BreakingBlock : public GameObject
{
public:
	BreakingBlock(int pX, int pZ);
	void breakBlock();
	void update (float pStep, bool pUpdateWorldTransform = false) override;
private:
	GameObject* _model;
	int _position [2] = { 0, 0 };
	bool _breaking = false;
	bool _updating = false;
};

#endif