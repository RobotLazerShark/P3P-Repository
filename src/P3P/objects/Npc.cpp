#include <P3P/objects/Npc.hpp>
#include <P3P/Level.hpp>
#include <P3P/objects/Player.hpp>
#include <P3P/objects/Collectable.hpp>
#include <mge/core/AbstractGame.hpp>
#include <JCPPEngine/TextureManager.hpp>
#include <JCPPEngine/FontManager.hpp>


//Static variables
Npc* Npc::singletonInstance = nullptr;


//Constructor
Npc::Npc(int pX, int pZ) : GameObject()
{
	if (singletonInstance != nullptr)
	{
		singletonInstance->setParent (nullptr);
		delete singletonInstance;
	}
	singletonInstance = this;

	//Set up model
	_model = new GameObject("PowerBox.obj");
	_material = new GlitchMaterial("PowerBox.png");
	_model->setMaterial(_material);
	_model->setParent(this);
	//Set up textbox
	_textBox = new sf::Sprite (*JCPPEngine::TextureManager::GetTexture ("images/TextBox.png"));
	_text = new sf::Text ("...", *JCPPEngine::FontManager::GetFont ("fonts/Font1.ttf"), 30);
	_text->setFillColor (sf::Color::White);
	sf::FloatRect size = _text->getLocalBounds ();
	_text->setOrigin (size.width * 0.5f, size.height * 0.5f);//Set origin at center
	_text->setPosition (AbstractGame::windowHalfWidth, AbstractGame::windowHalfHeight);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
	position [0] = pX;
	position [1] = pZ;
}
//Destructor
Npc::~Npc ()
{
	singletonInstance->setParent (nullptr);
	singletonInstance = nullptr;
	delete _textBox;
	delete _text;
}


//update npc
void Npc::update (float pStep, bool pUpdateWorldTransform)
{
	if (talking)
	{
		if (Player::singletonInstance->_currentTile [0] != _playerPosition [0] || Player::singletonInstance->_currentTile [1] != _playerPosition [1])
		{
			stopDialog ();
		}
		else
		{	
			Level::singletonInstance->drawBuffer.push_back (_textBox);
			Level::singletonInstance->drawBuffer.push_back (_text);
		}
	}
}


//Start talking to npc
void Npc::talk()
{
	talking = true;//This variable is so we can check if the npc is talking to the player from lua
	_playerPosition [0] = Player::singletonInstance->_currentTile [0];
	_playerPosition [1] = Player::singletonInstance->_currentTile [1];
	questTalks ++;
	//check if quests are completed
	for (int i = 0, size = activeQuests.size (); i < size; i ++)
	{
		Quest* quest = activeQuests [i];
		if (quest != nullptr)
		{
			for (int j = 0, size = Player::singletonInstance->inventory.size(); j < size; j++)
			{
				if (quest->_goalItem == Player::singletonInstance->inventory[j])
				{
					_glitchIntensity -= quest->fixIntensity;
					_material->SetGlitchIntensity (_glitchIntensity);
					//The item will remain in the player's inventory, so in lua dialog can respond to the completion of this quest
					delete quest;
					activeQuests [i] = nullptr;
					activeQuests.erase (activeQuests.begin () + i);
					questTalks = 0;
					completedQuests ++;

					Stats::singletonInstance->data.questsCompleted = completedQuests;
					Stats::singletonInstance->refreshText();
					return;
				}
			}
		}
	}
}
//Show npc dialog (regardless of where player is)
void Npc::displayDialog (std::string pText)
{
	talking = true;
	_playerPosition [0] = Player::singletonInstance->_currentTile [0];
	_playerPosition [1] = Player::singletonInstance->_currentTile [1];
	_text->setString (pText);
	sf::FloatRect size = _text->getLocalBounds ();
	_text->setOrigin (size.width * 0.5f, size.height * 0.5f);//Set origin at center
	questTalks ++;
	//check if quests are completed
	for (int i = 0, size = activeQuests.size (); i < size; i ++)
	{
		Quest* quest = activeQuests [i];
		if (quest != nullptr)
		{
			for (int j = 0, size = Player::singletonInstance->inventory.size(); j < size; j++)
			{
				if (quest->_goalItem == Player::singletonInstance->inventory[j])
				{
					_glitchIntensity -= quest->fixIntensity;
					_material->SetGlitchIntensity (_glitchIntensity);
					//The item will remain in the player's inventory, so in lua dialog can respond to the completion of this quest
					delete quest;
					activeQuests [i] = nullptr;
					activeQuests.erase (activeQuests.begin () + i);
					questTalks = 0;
					completedQuests ++;

					Stats::singletonInstance->data.questsCompleted = completedQuests;
					Stats::singletonInstance->refreshText();
					return;
				}
			}
		}
	}
}
//Update dialog text
void Npc::updateDialog (std::string pText)
{
	_text->setString (pText);
	sf::FloatRect size = _text->getLocalBounds ();
	_text->setOrigin (size.width * 0.5f, size.height * 0.5f);//Set origin at center
}
//Stop showing npc dialog
void Npc::stopDialog ()
{
	talking = false;
	_text->setString ("...");
	_playerPosition [0] = -1;
	_playerPosition [1] = -1;
}

//add new quest
void Npc::addQuest(Quest * newQuest)
{
	activeQuests.push_back (newQuest);
}