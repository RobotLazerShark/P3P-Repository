#ifndef BREAKINGBLOCK_Def
#define BREAKINGBLOCK_Def


//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/ColorMaterial.hpp>


class BreakingBlock : public GameObject
{
public:
	BreakingBlock(int pX, int pZ);
	void breakBlock(int pX, int pZ);
private:
	GameObject* _model;
};

#endif