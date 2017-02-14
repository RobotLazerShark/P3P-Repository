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
	~Npc ();
	void talk();
	void addQuest (Quest * newQuest);
	void update (float pStep, bool pUpdateWorldTransform = false) override;
	std::vector<Quest*> activeQuests;
	static Npc* singletonInstance;
	bool talking = false;
	int position [2] = { 0, 0 };

private:
	GameObject* _model;
	int _playerPosition [2] = { 0, 0 };
	
};

#endif