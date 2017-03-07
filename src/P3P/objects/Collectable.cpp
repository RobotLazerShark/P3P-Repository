#include "P3P/objects/Collectable.hpp"
#include <mge/materials/LitMaterial.hpp>
#include <P3P/objects/Player.hpp>
#include <P3P/Level.hpp>
#include <mge/core/AbstractGame.hpp>
#include <JCPPEngine/InputManager.hpp>
#include <JCPPEngine/TextureManager.hpp>
#include <JCPPEngine/FontManager.hpp>
#include <JCPPEngine/SoundManager.hpp>
#include <JCPPEngine/Random.hpp>
#include <mge/objects/DoomAnimation.hpp>

//Constructor
Collectable::Collectable(int pX, int pZ, std::string pName, bool pMark) : GameObject()//Quest collectable
{
	_name = pName;
	_hasDialog = false;
	_copyCollect = false;

	if (pMark)
	{
		_mark = new DoomAnimation (glm::vec3 (pX, 0.5f, pZ), "Mark.png", 64, 64, 0.08f);
		_mark->setParent (Level::singletonInstance->transparencyLayer2);
	}

	//Set up model
	_model = new GameObject ("RepairTools.obj");
	_model->setMaterial(new LitMaterial("RepairTools.png"));
	_model->setParent (this);
	_model->rotate (glm::radians (JCPPEngine::Random::Range (1, 3) * 90.0f), glm::vec3 (0, 1, 0));

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
	_position [0] = pX;
	_position [1] = pZ;
}
Collectable::Collectable(int pX, int pZ, std::string pName, std::string pDialog, bool pCopyCollect, bool pMark) : GameObject()
{
	_name = pName;
	_hasDialog = true;
	_copyCollect = pCopyCollect;
	registerForEvent (JCPPEngine::Event::EventType::KeyDown);

	//Set up model
	if (_copyCollect)//Tutorial computer
	{
		if (pMark)
		{
			_mark = new DoomAnimation (glm::vec3 (pX, 1.5f, pZ), "Mark.png", 64, 64, 0.08f);
			_mark->setParent (Level::singletonInstance->transparencyLayer3);
		}
		_model = new GameObject ("Computer.obj");
		_model->setMaterial (new LitMaterial ("Computer.png"));
		_model->translate (glm::vec3 (0, 0.05f, 0));
		_model->rotate (glm::radians (-90.0f), glm::vec3 (0, 1, 0));
	}
	else//Story collectable
	{
		if (pMark)
		{
			_mark = new DoomAnimation (glm::vec3 (pX, 0.25f,pZ), "Mark.png", 64, 64, 0.08f);
			_mark->setParent (Level::singletonInstance->transparencyLayer2);
		}
		_model = new GameObject ("cube_flat.obj");
		_model->setMaterial(new LitMaterial(glm::vec3(0.6f,0.6f,0)));
		_model->translate(glm::vec3(0, 0.25f, 0));
		_model->scale(0.5f);
	}
	_model->setParent(this);

	//Set up textbox (won't be visible yet)
	_textBox = new sf::Sprite (*JCPPEngine::TextureManager::GetTexture ("images/TextBox.png"));
	_text = new sf::Text (pDialog, *JCPPEngine::FontManager::GetFont ("fonts/Font3.ttf"), 35);
	_text->setFillColor (sf::Color::White);
	sf::FloatRect size = _text->getLocalBounds ();
	_text->setOrigin (0, size.height * 0.5f);//Set origin at center
	_text->setPosition (AbstractGame::windowHalfWidth * 0.6f, AbstractGame::windowHalfHeight);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
	_position [0] = pX;
	_position [1] = pZ;
}
Collectable::~Collectable ()
{
	setParent (nullptr);
	if (_model != nullptr)
	{
		_model->setParent (nullptr);
		delete _model;
		_model = nullptr;
	}
	if (_hasDialog)
	{
		unregisterForEvent (JCPPEngine::Event::EventType::KeyDown);
	}
	if (_textBox != nullptr)
	{
		delete _textBox;
		_textBox = nullptr;
	}
	if (_text != nullptr)
	{
		delete _text;
		_text = nullptr;
	}
	GameObject::~GameObject ();
}


//Check if this collectable is a quest item
bool Collectable::isQuestItem ()
{
	return !_hasDialog;
}


//Update
void Collectable::update (float pStep, bool pUpdateWorldTransform)
{
	GameObject::update (pStep, pUpdateWorldTransform);
	if (_hasDialog)
	{
		if (Player::singletonInstance->_currentTile [0] == _playerPosition [0] && Player::singletonInstance->_currentTile [1] == _playerPosition [1])
		{
			//Display dialog
			_showingDialog = true;
			Level::singletonInstance->drawBuffer.push_back (_textBox);
			Level::singletonInstance->drawBuffer.push_back (_text);
		}
		else if (_showingDialog)
		{
			//Stop displaying dialog
			_showingDialog = false;
			if (!_copyCollect)//Don't show the dialog again if not 'copied'
			{
				setParent (nullptr);
				unregisterForEvent (JCPPEngine::Event::EventType::KeyDown);
				if (_textBox != nullptr)
				{
					delete _textBox;
					_textBox = nullptr;
				}
				if (_text != nullptr)
				{
					delete _text;
					_text = nullptr;
				}
				Level::singletonInstance->deleteBuffer.push_back (this);
				_hasDialog = false;
			}
			else
			{
				_playerPosition [0] = -1;
				_playerPosition [1] = -1;
			}
		}
	}
}
//Close textbox on keypress
void Collectable::ProcessEvent (JCPPEngine::Event* pEvent)
{
	JCPPEngine::KeyEvent* tempEvent = (JCPPEngine::KeyEvent*)pEvent;
	if (tempEvent->key () == sf::Keyboard::Key::Space && _showingDialog)
	{
		if (_showingDialog)
		{
			//Stop displaying dialog
			_showingDialog = false;
			if (!_copyCollect)//Don't show the dialog again if not 'copied'
			{
				setParent (nullptr);
				unregisterForEvent (JCPPEngine::Event::EventType::KeyDown);
				if (_textBox != nullptr)
				{
					delete _textBox;
					_textBox = nullptr;
				}
				if (_text != nullptr)
				{
					delete _text;
					_text = nullptr;
				}
				Level::singletonInstance->deleteBuffer.push_back (this);
				_hasDialog = false;
			}
			else
			{
				_playerPosition [0] = -1;
				_playerPosition [1] = -1;
			}
		}
	}
}


//Get this collectable's name
std::string Collectable::getName ()
{
	return _name;
}


//Add item to player inventory
bool Collectable::collect (int pOldX, int pOldZ)
{
	bool alreadyInInventory = false;
	if (!_hasDialog)
	{
		JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/Collectable.wav")));
	}
	else
	{
		JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/Popup.wav")));
	}
	if (!_collected)
	{
		_collected = true;
		if (_mark != nullptr)
		{
			_mark->setParent (nullptr);
			delete _mark;
			_mark = nullptr;
		}
		//Add this object to the player's inventory
		for (int i = 0, size = Player::singletonInstance->inventory.size (); i < size; i ++)
		{
			if (Player::singletonInstance->inventory [i] == _name)
			{
				alreadyInInventory = true;
			}
		}
		if (!alreadyInInventory)
		{
			Player::singletonInstance->inventory.push_back (_name);
			Stats::singletonInstance->data.itemsCollected++;
			Stats::singletonInstance->refreshText();
		}
	}
	if (!_copyCollect)//Make object invisible/uninteractable
	{
		_model->setParent (nullptr);
		delete _model;
		_model = nullptr;
		Level::map->objectTiles [_position [0]] [_position [1]] = (int)nullptr;
		_playerPosition [0] = Player::singletonInstance->_currentTile [0];
		_playerPosition [1] = Player::singletonInstance->_currentTile [1];
		return false;
	}
	else
	{
		_playerPosition [0] = pOldX;
		_playerPosition [1] = pOldZ;
	}
	return true;
}