#include <P3P/objects/Npc.hpp>
#include <P3P/Level.hpp>
#include <P3P/objects/Player.hpp>
#include <P3P/objects/Collectable.hpp>
#include <mge/core/AbstractGame.hpp>
#include <JCPPEngine/TextureManager.hpp>
#include <JCPPEngine/FontManager.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <mge/objects/Light.hpp>
#include <mge/materials/TextureMaterial.hpp>
#include <JCPPEngine/SoundManager.hpp>


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
	Level::map->objectTiles [pX-1] [pZ-1] = (int)this;
	Level::map->objectTiles [pX-1] [pZ] = (int)this;
	Level::map->objectTiles [pX] [pZ-1] = (int)this;

	//Set up model
	_model = new GameObject("BossBrokenBase.obj");
	_model->setMaterial (new LitMaterial ("BossBrokenBase.png"));
	_model->setParent(this);
	GameObject* shadow = new GameObject ("ShadowPlane.obj");
	shadow->badScale (glm::vec3 (2, 1, 2));
	shadow->setMaterial (new TextureMaterial ("BossBrokenShadow.png"));
	shadow->translate (glm::vec3 (-0.3f, 0.33f, -0.25f));
	shadow->setParent (_model);
	GameObject* boxes = new GameObject ("BossBrokenBoxes.obj");
	boxes->setMaterial (new LitMaterial ("BossBrokenBoxes.png"));
	boxes->setParent (_model);
	GameObject* subModel = new GameObject ("BossBroken.obj");
	_material = new GlitchMaterial("BossBroken.png");
	subModel->translate (glm::vec3 (-0.486f, 0.816f, -0.41f));
	subModel->setMaterial(_material);
	subModel->setParent (_model);
	//Set up textbox
	_textBox = new sf::Sprite (*JCPPEngine::TextureManager::GetTexture ("images/TextBox.png"));
	_text = new sf::Text ("...", *JCPPEngine::FontManager::GetFont ("fonts/Font1.ttf"), 20);
	_text->setFillColor (sf::Color::White);
	sf::FloatRect size = _text->getLocalBounds ();
	_text->setOrigin (0, size.height * 0.5f);//Set origin at center
	_text->setPosition (AbstractGame::windowHalfWidth*0.6f, AbstractGame::windowHalfHeight);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
	position [0] = pX;
	position [1] = pZ;

	Light* light = new Light(glm::vec3(1, 1, 0.8f), 12, glm::vec3(-0.5f, 2.5f, -0.5f), glm::radians (60.0f), glm::vec2 (0.25f, 0.75f));
	light->setParent(this);
}
//Destructor
Npc::~Npc ()
{
	singletonInstance->setParent (nullptr);
	singletonInstance = nullptr;
	Level::map->objectTiles [position [0]-1] [position[1] -1] = (int)nullptr;
	Level::map->objectTiles [position[0] -1] [position[1]] = (int)nullptr;
	Level::map->objectTiles [position[0]] [position[1] -1] = (int)nullptr;
	Level::map->objectTiles [position[0]] [position[1]] = (int)nullptr;
	delete _textBox;
	delete _text;
	if (_soundIndex != -1)
	{
		JCPPEngine::SoundManager::StopSoundLoop (_soundIndex);
		_soundIndex = -1;
	}
	GameObject::~GameObject ();
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
	_soundIndex = JCPPEngine::SoundManager::PlaySoundLoop (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/NpcTalking.wav")));
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
					JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/Repair.wav")));
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
	_soundIndex = JCPPEngine::SoundManager::PlaySoundLoop (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/NpcTalking.wav")));
	_playerPosition [0] = Player::singletonInstance->_currentTile [0];
	_playerPosition [1] = Player::singletonInstance->_currentTile [1];
	_text->setString (pText);
	sf::FloatRect size = _text->getLocalBounds ();
	_text->setOrigin (0, size.height * 0.5f);//Set origin at center
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
					JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/repair.wav")));
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
	_text->setOrigin (0, size.height * 0.5f);//Set origin at center
}
//Stop showing npc dialog
void Npc::stopDialog ()
{
	talking = false;
	JCPPEngine::SoundManager::StopSoundLoop (_soundIndex);
	_soundIndex = -1;
	_text->setString ("...");
	_playerPosition [0] = -1;
	_playerPosition [1] = -1;
}

//add new quest
void Npc::addQuest(Quest * newQuest)
{
	activeQuests.push_back (newQuest);
}