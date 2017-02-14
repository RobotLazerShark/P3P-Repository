#include <P3P/objects/Npc.hpp>
#include <P3P/Level.hpp>


//Static variables
Npc* Npc::singletonInstance = nullptr;


//Constructor
Npc::Npc(int pX, int pZ) : GameObject()
{
	if (singletonInstance != nullptr)
	{
		return;
	}
	singletonInstance = this;

	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->setMaterial(new LitMaterial("Npc.jpg"));
	_model->translate(glm::vec3(0, 0.5f, 0));
	_model->setParent(this);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
}
//Destructor
Npc::~Npc ()
{
	singletonInstance = nullptr;
}

void Npc::talk()
{
	//check if quests are completed
	for (Quest * quest : activeQuests)
	{
		if (quest != nullptr)
		{
			for (int j = 0, size = Player::singletonInstance->inventory.size(); j < size; j++)
			{
				if (quest->_goalItem == Player::singletonInstance->inventory[j]->getName())
				{
					//The item will remain in the player's inventory, so in lua dialog can respond to the completion of this quest
					delete quest;
					return;
				}
			}
		}
	}
}

//add new quest
void Npc::addQuest(Quest * newQuest)
{
	activeQuests.push_back(newQuest);
}