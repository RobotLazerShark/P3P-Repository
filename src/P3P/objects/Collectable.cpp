#include "P3P/objects/Collectable.hpp"
#include <mge/materials/LitMaterial.hpp>
#include <P3P/objects/Player.hpp>
#include <P3P/Level.hpp>
#include <mge/core/AbstractGame.hpp>

//Constructor
Collectable::Collectable(int pX, int pZ, std::string pName, bool pCopyCollect) : GameObject()
{
	_name = pName;
	_copyCollect = pCopyCollect;

	//Set up model
	if (_copyCollect)
	{
		_model = new GameObject ("Computer.obj");
		_model->setMaterial (new LitMaterial ("Computer.png"));
		_model->rotate (glm::radians (-90.0f), glm::vec3 (0, 1, 0));
	}
	else
	{
		_model = new GameObject("cube_flat.obj");
		_model->setMaterial(new LitMaterial(glm::vec3(0.6f,0.6f,0)));
		_model->translate(glm::vec3(0, 0.5f, 0));
		_model->scale(0.5f);
	}

	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));
	_position [0] = pX;
	_position [1] = pZ;
}
Collectable::Collectable(int pX, int pZ, std::string pName, std::string pDialog, bool pCopyCollect) : GameObject()
{
	_name = pName;
	_hasDialog = true;
	_copyCollect = pCopyCollect;

	//Set up model
	if (_copyCollect)
	{
		_model = new GameObject ("Computer.obj");
		_model->setMaterial (new LitMaterial ("Computer.png"));
		_model->rotate (glm::radians (-90.0f), glm::vec3 (0, 1, 0));
	}
	else
	{
		_model = new GameObject("cube_flat.obj");
		_model->setMaterial(new LitMaterial(glm::vec3(0.6f,0.6f,0)));
		_model->translate(glm::vec3(0, 0.5f, 0));
		_model->scale(0.5f);
	}
	_model->setParent(this);
	//Set up textbox (won't be visible yet)
	_textBox = new sf::Sprite (*JCPPEngine::TextureManager::GetTexture ("images/TextBox.png"));
	_text = new sf::Text (pDialog, *JCPPEngine::FontManager::GetFont ("fonts/Font1.ttf"), 30);
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
	delete _textBox;
	delete _text;
	GameObject::~GameObject ();
}


//Update
void Collectable::update (float pStep, bool pUpdateWorldTransform)
{
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
			if (_collected && !_copyCollect)//Don't show the dialog again if not 'copied'
			{
				setParent (nullptr);
			}
			else
			{
				_playerPosition [0] = -1;
				_playerPosition [1] = -1;
			}
		}
	}
	GameObject::update (pStep, pUpdateWorldTransform);
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
		}
	}
	if (!_copyCollect)//Make object invisible/uninteractable
	{
		_model->setParent (nullptr);
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