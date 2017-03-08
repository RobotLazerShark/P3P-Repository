#include <P3P/Hud.hpp>
#include <P3P/objects/Player.hpp>
#include <P3P/Level.hpp>
#include <JCPPEngine/MouseEvent.hpp>
#include <JCPPEngine/InputManager.hpp>
#include <mge/core/AbstractGame.hpp>
#include <P3P/Menu.hpp>


//---------------button press functions---------------------
bool statsOn = false;
int hintIndex = -1;
int oldState = 0;

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
	statsOn = !statsOn;
	if (Stats::singletonInstance != nullptr)
	{
		Stats::singletonInstance->setActive(statsOn);
	}
}

void levelPauseFunction()
{
	oldState = Hud::singletonInstance->state;
	Hud::singletonInstance->setState(4);
}
void hubPauseFunction()
{
	oldState = Hud::singletonInstance->state;
	Hud::singletonInstance->setState(5);
	//TODO: pause game.
}

void continueFunction()
{
	statsOn = false;
	if (Stats::singletonInstance != nullptr)
	{
		Stats::singletonInstance->setActive(statsOn);
	}
	Hud::singletonInstance->setState(oldState);
	AbstractGame::showCursor (false);
	Player::singletonInstance->blockMovement = false;
}

//------------------------------------------------------------

//static variables
Hud* Hud::singletonInstance = nullptr;

Hud::Hud()
{
	if (singletonInstance != nullptr)
	{
		delete singletonInstance;
	}
	singletonInstance = this;

	//create buttons
	buttons.push_back(new HudButton("mge/textures/PauseButton.png", sf::Vector2f(0, 0), &levelPauseFunction)); //0
	buttons.push_back(new HudButton("mge/textures/PauseButton.png", sf::Vector2f(0, 0), &hubPauseFunction)); //1
	buttons.push_back(new HudButton("mge/textures/ReloadButton.png", sf::Vector2f(0, 95), &reloadFunction)); //2

	buttons.push_back(new HudButton("mge/textures/HintButton.png", sf::Vector2f(500, 2 * 95), &hintFunction)); //3
	buttons.push_back(new HudButton("mge/textures/ContinueButton.png", sf::Vector2f(500, 3 * 95), &continueFunction)); //4
	buttons.push_back(new HudButton("mge/textures/StatsButton.png", sf::Vector2f(500, 4 * 95), &statsFunction)); //5
	buttons.push_back(new HudButton("mge/textures/QuitToMenuButton.png", sf::Vector2f(500, 5 * 95), &quitToMenuFunction)); //6
	buttons.push_back(new HudButton("mge/textures/QuitToHudButton.png", sf::Vector2f(500, 6* 95), &quitToHudFunction)); //7
	
	registerForEvent(JCPPEngine::Event::EventType::MouseDown);

	if (Stats::singletonInstance == nullptr)
	{
		Stats * _stats = new Stats();
	}
}

Hud::~Hud()
{
	unregisterForEvent(JCPPEngine::Event::EventType::MouseDown);
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
		break;
	case 2: //normal level
		buttons[0]->setActive(true);
		buttons[1]->setActive(true);
		buttons[2]->setActive(true);
		break;
	case 3://boss level
		//todo
		break;
	case 4://level pause
		buttons[3]->setActive(true);
		buttons[4]->setActive(true);
		buttons[5]->setActive(true);
		buttons[6]->setActive(true);
		buttons[7]->setActive(true);
		break;
	case 5://hub pause
		buttons[4]->setActive(true);
		buttons[5]->setActive(true);
		buttons[6]->setActive(true);
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
	
	sf::Vector2i mousePos = mouseDownEvent->position();

	if (_active)
	{
		for (HudButton * button : buttons)
		{
			sf::Sprite * sprite = button->_sprite;
			//if mouse is on top of a button
			if (mousePos.x >= sprite->getPosition().x && mousePos.x <= sprite->getPosition().x + sprite->getTextureRect().width
				&&mousePos.y >= sprite->getPosition().y && mousePos.y <= sprite->getPosition().y + sprite->getTextureRect().height)
			{
				//press button
				button->press();
			}
		}
	}
}

std::vector<sf::Drawable*> Hud::getAllDrawables()
{
	std::vector<sf::Drawable*> drawables;
	
	//get all button sprites
	for (HudButton * button : buttons)
	{
		drawables.push_back(button->_sprite);
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