#include "P3P/objects/Collectable.hpp"
#include <mge/materials/LitMaterial.hpp>
#include <P3P/objects/Player.hpp>
#include <P3P/Level.hpp>
#include <mge/core/AbstractGame.hpp>
#include <JCPPEngine/InputManager.hpp>
#include <JCPPEngine/TextureManager.hpp>
#include <JCPPEngine/FontManager.hpp>

//Constructor
Collectable::Collectable(int pX, int pZ, std::string pName) : GameObject()
{
	_name = pName;
	_hasDialog = false;
	_copyCollect = false;

	//Set up model
	if (_copyCollect)
	{
		_model = new GameObject ("Computer.obj");
		_model->setMaterial (new LitMaterial ("Computer.png"));
		_model->translate (glm::vec3 (0, 0.05f, 0));
		_model->rotate (glm::radians (-90.0f), glm::vec3 (0, 1, 0));
	}
	else
	{
		_model = new GameObject ("cube_flat.obj");
		_model->setMaterial(new LitMaterial(glm::vec3(0.6f,0.6f,0)));
		_model->translate(glm::vec3(0, 0.5f, 0));
		_model->scale(0.5f);
	}
	_model->setParent (this);

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
	_position [0] = pX;
	_position [1] = pZ;
}
Collectable::Collectable(int pX, int pZ, std::string pName, std::string pDialog, bool pCopyCollect) : GameObject()
{
	_name = pName;
	_hasDialog = true;
	_copyCollect = pCopyCollect;
	registerForEvent (JCPPEngine::Event::EventType::KeyDown);

	//Set up model
	if (_copyCollect)
	{
		_model = new GameObject ("Computer.obj");
		_model->setMaterial (new LitMaterial ("Computer.png"));
		_model->translate (glm::vec3 (0, 0.05f, 0));
		_model->rotate (glm::radians (-90.0f), glm::vec3 (0, 1, 0));
	}
	else
	{
		_model = new GameObject ("cube_flat.obj");
		_model->setMaterial(new LitMaterial(glm::vec3(0.6f,0.6f,0)));
		_model->translate(glm::vec3(0, 0.5f, 0));
		_model->scale(0.5f);
	}
	_model->setParent(this);

	//Set up textbox (won't be visible yet)
	_textBox = new sf::Sprite (*JCPPEngine::TextureManager::GetTexture ("images/TextBox.png"));
	_text = new sf::Text (pDialog, *JCPPEngine::FontManager::GetFont ("fonts/Font1.ttf"), 20);
	_text->setFillColor (sf::Color::White);
	sf::FloatRect size = _text->getLocalBounds ();
	_text->setOrigin (size.width * 0.5f, size.height * 0.5f);//Set origin at center
	_text->setPosition (AbstractGame::windowHalfWidth, AbstractGame::windowHalfHeight);

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
	if (!_collected)
	{
		_collected = true;
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