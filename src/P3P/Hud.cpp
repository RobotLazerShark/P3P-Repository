#include <P3P/Hud.hpp>
#include <P3P/objects/Player.hpp>
#include <P3P/Level.hpp>
#include <JCPPEngine/MouseEvent.hpp>
#include <JCPPEngine/InputManager.hpp>
#include <mge/core/AbstractGame.hpp>
#include <P3P/Menu.hpp>
#include <P3P/objects/Boss.hpp>

//---------------button press functions---------------------
bool statsOn = false;
int hintIndex = -1;
int oldState = 0;
int oldPauseState = 0;

void reloadFunction()
{
	Player::singletonInstance->die();
}

void quitToHudFunction()
{
	hintIndex = -1;
	statsOn = false;
	
	if (Stats::singletonInstance != nullptr)
	{
		Stats::singletonInstance->setActive(statsOn);
	}
	Level::singletonInstance->loadLevel(0);
}

void quitToMenuFunction()
{
	Menu::singletonInstance->_game->clear ();
	delete Menu::singletonInstance->_game;
	delete Menu::singletonInstance->_fader;
	AbstractGame::showCursor (true);
	Menu::singletonInstance->UnHide ();
}

void hintFunction()
{
	if (hintIndex != -1)
	{
		Level::singletonInstance->hints[hintIndex]->setActive(false);
	}
	hintIndex++;
	if (hintIndex == Level::singletonInstance->hints.size())
	{
		hintIndex = -1;
	}
	else
	{
		Stats::singletonInstance->data.hintsUsed++;
		Stats::singletonInstance->refreshText();
		Level::singletonInstance->hints[hintIndex]->setActive(true);
	}
}

void statsFunction()
{
	oldPauseState = Hud::singletonInstance->state;
	Hud::singletonInstance->blockStatsReturn = true;

	statsOn = !statsOn;
	if (Stats::singletonInstance != nullptr)
	{
		Stats::singletonInstance->setActive(statsOn);
	}
	Hud::singletonInstance->setState(6);
	Hud::singletonInstance->_levelPauseSprite->setColor(sf::Color(255, 255, 255, 0));
	Hud::singletonInstance->_hubPauseSprite->setColor(sf::Color(255, 255, 255, 0));
	Hud::singletonInstance->_statsSprite->setColor(sf::Color(255, 255, 255, 255));
}

void levelPauseFunction()
{
	oldState = Hud::singletonInstance->state;

	Hud::singletonInstance->setState(4);
	Hud::singletonInstance->showPauseMenu = true;
	Hud::singletonInstance->_levelPauseSprite->setColor(sf::Color(255, 255, 255, 255));
	Hud::singletonInstance->_hubPauseSprite->setColor(sf::Color(255, 255, 255, 0));
	Hud::singletonInstance->_statsSprite->setColor(sf::Color(255, 255, 255, 0));

	if (Boss::singletonInstance != nullptr)
	{
		Boss::singletonInstance->pause(true);
	}
}
void hubPauseFunction()
{
	oldState = Hud::singletonInstance->state;

	Hud::singletonInstance->setState(5);
	Hud::singletonInstance->showPauseMenu = true;
	Hud::singletonInstance->_levelPauseSprite->setColor(sf::Color(255, 255, 255, 0));
	Hud::singletonInstance->_hubPauseSprite->setColor(sf::Color(255, 255, 255, 255));
	Hud::singletonInstance->_statsSprite->setColor(sf::Color(255, 255, 255, 0));
	
	if (Boss::singletonInstance != nullptr)
	{
		Boss::singletonInstance->pause(true);
	}
}

void continueFunction()
{
	statsOn = false;
	if (Stats::singletonInstance != nullptr)
	{
		Stats::singletonInstance->setActive(statsOn);
	}
	Hud::singletonInstance->setState(oldState);
	Hud::singletonInstance->_levelPauseSprite->setColor(sf::Color(255, 255, 255, 0));
	Hud::singletonInstance->_hubPauseSprite->setColor(sf::Color(255, 255, 255, 0));
	Hud::singletonInstance->_statsSprite->setColor(sf::Color(255, 255, 255, 0));

	AbstractGame::showCursor (false);
	Player::singletonInstance->blockMovement = false;

	if (Boss::singletonInstance != nullptr)
	{
		Boss::singletonInstance->pause(false);
	}
}

void statsReturnFunctin()
{
	if (!Hud::singletonInstance->blockStatsReturn)
	{
		statsOn = false;

		if (Stats::singletonInstance != nullptr)
		{
			Stats::singletonInstance->setActive(statsOn);
		}

		Hud::singletonInstance->setState(oldPauseState);
		switch (oldPauseState)
		{
		case 4: //level
			Hud::singletonInstance->_levelPauseSprite->setColor(sf::Color(255, 255, 255, 255));
			Hud::singletonInstance->_hubPauseSprite->setColor(sf::Color(255, 255, 255, 0));
			Hud::singletonInstance->_statsSprite->setColor(sf::Color(255, 255, 255, 0));
			break;
		case 5: //hub
			Hud::singletonInstance->_levelPauseSprite->setColor(sf::Color(255, 255, 255, 0));
			Hud::singletonInstance->_hubPauseSprite->setColor(sf::Color(255, 255, 255, 255));
			Hud::singletonInstance->_statsSprite->setColor(sf::Color(255, 255, 255, 0));
			break;
		}
	}
}

//------------------------------------------------------------

//static variables
Hud* Hud::singletonInstance = nullptr;

Hud::Hud(sf::RenderWindow* pWindow)
{
	if (singletonInstance != nullptr)
	{
		delete singletonInstance;
	}
	singletonInstance = this;

	_levelPauseSprite = new sf::Sprite(*JCPPEngine::TextureManager::GetTexture("images/InLevel_NoSelection.png"));
	_hubPauseSprite = new sf::Sprite(*JCPPEngine::TextureManager::GetTexture("images/InHUB_NoSelection.png"));
	_statsSprite = new sf::Sprite(*JCPPEngine::TextureManager::GetTexture("images/Stats_NoSelection.png"));

	sf::Vector2f* windowSize = new sf::Vector2f(pWindow->getSize().x, pWindow->getSize().y);
	sf::Vector2f* spriteSize = new sf::Vector2f(_levelPauseSprite->getTextureRect().width, _levelPauseSprite->getTextureRect().height);
	sf::Vector2f spritePos = sf::Vector2f(windowSize->x / 2 - spriteSize->x / 2, windowSize->y / 2 - spriteSize->y / 2);

	sf::Sprite * hitbox = new sf::Sprite(*JCPPEngine::TextureManager::GetTexture("images/ButtonHitBox.png"));
	float hitBoxPosX = windowSize->x / 2 - hitbox->getTextureRect().width/2;

	_hubPauseSprite->setPosition(spritePos);
	_levelPauseSprite->setPosition(spritePos);
	_statsSprite->setPosition(sf::Vector2f(windowSize->x / 2- _statsSprite->getTextureRect().width/2, windowSize->y / 2 - _statsSprite->getTextureRect().height / 2));

	//create buttons
	buttons.push_back(new HudButton("images/HudButtonHitBox.png","", sf::Vector2f(1680, 948), spritePos, &levelPauseFunction, _debugMode)); //0 level
	buttons.push_back(new HudButton("images/HudButtonHitBox.png", "", sf::Vector2f(1680, 948), spritePos, &hubPauseFunction, _debugMode)); //1 hub
	buttons.push_back(new HudButton("images/HudButtonHitBox.png", "", sf::Vector2f(1697, 65), spritePos, &reloadFunction, _debugMode)); //2
	buttons.push_back(new HudButton("images/HudButtonHitBox.png", "", sf::Vector2f(120, 948), spritePos, &hintFunction, _debugMode)); //3
	int hitBoxPosY = 434;
	int step = 137;
	buttons.push_back(new HudButton("images/ButtonHitBox.png", "images/InLevel_Continue.png", sf::Vector2f(hitBoxPosX, hitBoxPosY), spritePos, &continueFunction, _debugMode)); //4 level
	buttons.push_back(new HudButton("images/ButtonHitBox.png", "images/InLevel_BacktoHUB.png", sf::Vector2f(hitBoxPosX, hitBoxPosY + step), spritePos, &quitToHudFunction, _debugMode)); //5  level
	buttons.push_back(new HudButton("images/ButtonHitBox.png", "images/InLevel_BacktoMenu.png", sf::Vector2f(hitBoxPosX, hitBoxPosY + step*2), spritePos, &quitToMenuFunction, _debugMode)); //6 level
	buttons.push_back(new HudButton("images/ButtonHitBox.png", "images/InLevel_Stats.png", sf::Vector2f(hitBoxPosX, hitBoxPosY + step*3), spritePos, &statsFunction, _debugMode)); //7 level
	hitBoxPosY = 472;
	step = 140;
	buttons.push_back(new HudButton("images/ButtonHitBox.png", "images/InHUB_Continue.png", sf::Vector2f(hitBoxPosX, hitBoxPosY), spritePos, &continueFunction, _debugMode)); //8 hub
	buttons.push_back(new HudButton("images/ButtonHitBox.png", "images/InHUB_BacktoMenu.png", sf::Vector2f(hitBoxPosX, hitBoxPosY + step), spritePos, &quitToMenuFunction, _debugMode)); //9 hub
	buttons.push_back(new HudButton("images/ButtonHitBox.png", "images/InHUB_Stats.png", sf::Vector2f(hitBoxPosX, hitBoxPosY + step*2), spritePos, &statsFunction, _debugMode)); //10 hub
	
	buttons.push_back(new HudButton("images/StatsHitBox.png", "images/Stats.png", sf::Vector2f(_statsSprite->getPosition().x,765), sf::Vector2f(_statsSprite->getPosition().x, _statsSprite->getPosition().y), &statsReturnFunctin, _debugMode)); //11

	registerForEvent(JCPPEngine::Event::EventType::MouseDown);
	registerForEvent(sf::Event::MouseMoved);

	//show pause menu with no selection
	switch (state)
	{
	case 4: //level
		_levelPauseSprite->setColor(sf::Color(255, 255, 255, 255));
		_hubPauseSprite->setColor(sf::Color(255, 255, 255, 0));
		_statsSprite->setColor(sf::Color(255, 255, 255, 0));
		break;
	case 5: //hub
		_levelPauseSprite->setColor(sf::Color(255, 255, 255, 0));
		_hubPauseSprite->setColor(sf::Color(255, 255, 255, 255));
		_statsSprite->setColor(sf::Color(255, 255, 255, 0));
		break;
	case 6: //stats
		_levelPauseSprite->setColor(sf::Color(255, 255, 255, 0));
		_hubPauseSprite->setColor(sf::Color(255, 255, 255, 0));
		_statsSprite->setColor(sf::Color(255, 255, 255, 255));
		break;
	default:
		_levelPauseSprite->setColor(sf::Color(255, 255, 255, 0));
		_hubPauseSprite->setColor(sf::Color(255, 255, 255, 0));
		_statsSprite->setColor(sf::Color(255, 255, 255, 0));
		break;
	}

	if (Stats::singletonInstance == nullptr)
	{
		Stats * _stats = new Stats(sf::Vector2f (_statsSprite->getPosition().x, _statsSprite->getPosition().y));
	}
}

Hud::~Hud()
{
	unregisterForEvent(JCPPEngine::Event::EventType::MouseDown);
	unregisterForEvent(sf::Event::MouseMoved);
	for (HudButton * button : buttons)
	{
		delete button;
	}
	if (Stats::singletonInstance != nullptr)
	{
		delete Stats::singletonInstance;
		Stats::singletonInstance = nullptr;
	}

	singletonInstance = nullptr;
	setParent(nullptr);
	GameObject::~GameObject();
}

void Hud::update(float pStep, bool pUpdateWorldTransform)
{
	if (blockStatsReturn)
	{
		_statsReturnBlockedFor += pStep;
		if (_statsReturnBlockedFor >= _statsBlockingTime)
		{
			_statsReturnBlockedFor = 0;
			blockStatsReturn = false;
		}
	}

	GameObject::update(pStep, pUpdateWorldTransform);
}

void Hud::setState(int newState)
{
	state = newState;
	for(HudButton * button : buttons)
	{
		button->setActive(false);
	}
	switch (state)
	{
	case 0: //menu
		break;
	case 1: //hub
		buttons[1]->setActive(true);
		buttons[2]->setActive(true);
		buttons[3]->setActive(true);
		break;
	case 2: //normal level
		buttons[0]->setActive(true);
		buttons[2]->setActive(true);
		buttons[3]->setActive(true);
		break;
	case 3://boss level
		//todo
		break;
	case 4://level pause
		buttons[4]->setActive(true);
		buttons[5]->setActive(true);
		buttons[6]->setActive(true);
		buttons[7]->setActive(true);
		break;
	case 5://hub pause
		buttons[8]->setActive(true);
		buttons[9]->setActive(true);
		buttons[10]->setActive(true);
		break;
	case 6://stats
		buttons[11]->setActive(true);
		break;
	}
}

void Hud::ProcessEvent(JCPPEngine::Event* pEvent)
{
	JCPPEngine::MouseEvent* mouseDownEvent = (JCPPEngine::MouseEvent*)pEvent;

	if (mouseDownEvent == nullptr || mouseDownEvent->eventType() != JCPPEngine::Event::EventType::MouseDown)
	{
		return;
	}
	
	if (_active)
	{
		sf::Vector2f  mousePos =(sf::Vector2f) mouseDownEvent->position();
		for (HudButton * button : buttons)
		{
			button->tryPressing(mousePos);
		}
	}
}

void Hud::ProcessEvent(sf::Event pEvent)
{
	if (pEvent.type == sf::Event::MouseMoved)
	{

		if (_active)
		{
			bool atLeastOneButtonIsSelected = false;

			sf::Vector2f mousePos = sf::Vector2f(pEvent.mouseMove.x, pEvent.mouseMove.y);
			for (HudButton * button : buttons)
			{
				if (button->tryHovering(mousePos))
				{
					atLeastOneButtonIsSelected = true;
				}
			}

			if (!atLeastOneButtonIsSelected)
			{
				//show pause menu with no selection
				switch (state)
				{
				case 4: //level
					_levelPauseSprite->setColor(sf::Color(255, 255, 255, 255));
					_hubPauseSprite->setColor(sf::Color(255, 255, 255, 0));
					_statsSprite->setColor(sf::Color(255, 255, 255, 0));
					break;
				case 5: //hub
					_levelPauseSprite->setColor(sf::Color(255, 255, 255, 0));
					_hubPauseSprite->setColor(sf::Color(255, 255, 255, 255));
					_statsSprite->setColor(sf::Color(255, 255, 255, 0));
					break;
				case 6: //stats
					_levelPauseSprite->setColor(sf::Color(255, 255, 255, 0));
					_hubPauseSprite->setColor(sf::Color(255, 255, 255, 0));
					_statsSprite->setColor(sf::Color(255, 255, 255, 255));
					break;
				default:
					_levelPauseSprite->setColor(sf::Color(255, 255, 255, 0));
					_hubPauseSprite->setColor(sf::Color(255, 255, 255, 0));
					_statsSprite->setColor(sf::Color(255, 255, 255, 0));
					break;
				}
			}
			else
			{
				_levelPauseSprite->setColor(sf::Color(255, 255, 255, 0));
				_hubPauseSprite->setColor(sf::Color(255, 255, 255, 0));
				_statsSprite->setColor(sf::Color(255, 255, 255, 0));
			}
		}
	}
}

std::vector<sf::Drawable*> Hud::getAllDrawables()
{
	std::vector<sf::Drawable*> drawables;
	
	drawables.push_back(_levelPauseSprite);
	drawables.push_back(_hubPauseSprite);
	drawables.push_back(_statsSprite);

	//get all buttons
	for (HudButton * button : buttons)
	{
		for (sf::Drawable * drawable : button->getAllDrawables())
		{
			drawables.push_back(drawable);
		}
	}
	//get all stats drawables
	if (Stats::singletonInstance != nullptr)
	{
		for (sf::Drawable * drawable : Stats::singletonInstance->getAllDrawables())
		{
			drawables.push_back(drawable);
		}
	}
	//get hint info
	if (_active)
	{
		if (hintIndex != -1)
		{
			drawables.push_back(Level::singletonInstance->hints[hintIndex]->_text);
		}
	}
	
	return drawables;
}

void Hud::disable()
{
	_active = false;
	hintIndex = -1;
}

void Hud::enable()
{
	_active = true;
}

void Hud::pressButton(int button)
{
	switch (button)
	{
		case 1:
			hintFunction();
			break;
		case 2:
			switch (state)
			{
			case 1:
				hubPauseFunction();
				break;
			case 2:
				levelPauseFunction();
				break;
			default:
				break;
			}
			break;
		default:
			break;
	}
}