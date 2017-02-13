#include <P3P/objects/Npc.hpp>
#include <P3P/Level.hpp>

//static
std::vector <Quest*> Npc::givenQuests;
std::vector<Dialog*> Npc::untalkedDialogs;
bool Npc::startingQuestGiven = false;

Npc::Npc(int pX, int pZ) : GameObject()
{
	//Set up model
	_model = new GameObject("cube_flat.obj");
	LitMaterial* mat = new LitMaterial("Npc.jpg");
	_model->setMaterial(mat);
	_model->translate(glm::vec3(0, 0.5f, 0));
	_model->setParent(this);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
	

	//add quests (later gonna be done with lua)
	if (!startingQuestGiven)
	{
		Dialog * dialog1 = new Dialog("lalala1", 
			new Dialog("lalala2", nullptr, new Quest("Go collect item called itemA", "itemA")), 
			nullptr);

		addDialog(dialog1);

		startingQuestGiven = true;
	}
}

void Npc::talk()
{
	//-------------------dialogs-----------------------
	//clear talked dialog
	int dialogToDeleteIndex;
	Dialog * dialogToDelete = nullptr;
	for (int i = 0; i < untalkedDialogs.size(); i++)
	{
		if (untalkedDialogs[i]->talked)
		{
			//save index of talked dialog
			dialogToDeleteIndex = i;
			//save talked dialog
			dialogToDelete = untalkedDialogs[i];
		}
	}
	if (dialogToDelete != nullptr)
	{
		//remove talked dialog from the vector
		untalkedDialogs.erase(untalkedDialogs.begin() + dialogToDeleteIndex);
		//delete talked dialog
		delete dialogToDelete;
	}

	//talk through first dialog in vector
	bool alreadyTalked = false;
	for (int i = 0; i < untalkedDialogs.size(); i++)
	{
		if (!alreadyTalked && untalkedDialogs[i] != nullptr)
		{
			std::cout << untalkedDialogs[i]->talk() << std::endl;
			alreadyTalked = true;
		}
	}
	
	//-------------------quests-----------------------
	if (!alreadyTalked)
	{
		//complete all quests
		for (Quest * quest : givenQuests)
		{
			if (quest != nullptr)
			{
				for (int j = 0; j < Player::collectedItems.size(); j++)
				{
					if (quest->_goalItem == Player::collectedItems[j])
					{
						std::cout << "quest completed" << endl;
						delete quest;
						return;
					}
				}
			}
		}

		//if any single quests wasnt completed output info of the first quest in the vector
		for (Quest * quest : givenQuests)
		{
			if (quest != nullptr)
			{
				std::cout << quest->_info << endl;
			}
		}
	}
}

//add new quest
void Npc::addQuest(Quest * newQuest)
{
	givenQuests.push_back(newQuest);
}

//add new dialog
void Npc::addDialog(Dialog * newDialog)
{
	untalkedDialogs.push_back(newDialog);
}

