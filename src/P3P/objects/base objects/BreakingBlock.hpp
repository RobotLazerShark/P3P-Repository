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
private:
	GameObject* _model;
	int _position [2] = { 0, 0 };
};

#endif