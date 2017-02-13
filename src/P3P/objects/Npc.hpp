#ifndef NPC_Def
#define NPC_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <P3P/Quest.hpp>

class Npc : public GameObject
{
public:
	Npc(int pX, int pZ);
	void talk();
	void addQuest (Quest * newQuest);
	std::vector<Quest*> activeQuests;

private:
	GameObject* _model;
	
};

#endif