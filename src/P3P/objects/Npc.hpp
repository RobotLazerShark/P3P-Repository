#ifndef NPC_Def
#define NPC_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <P3P/Quest.hpp>
#include <P3P/Dialog.hpp>

class Npc : public GameObject
{
public:
	Npc(int pX, int pZ);
	void talk();

	void addQuest(Quest * newQuest);
	void addDialog(Dialog* newDialog);

	static std::vector<Quest*> givenQuests;
	static std::vector<Dialog*> untalkedDialogs;
	static bool startingQuestGiven;
	
private:
	GameObject* _model;
	
};

#endif