#include <P3P/Hud.hpp>
#include <P3P/objects/Player.hpp>
#include <P3P/Level.hpp>
#include <JCPPEngine/MouseEvent.hpp>
#include <JCPPEngine/InputManager.hpp>

//---------------button press functions---------------------
bool statsOn = false;
int hintIndex = -1;

void reloadFunction()
{
	Player::singletonInstance->die();
}
void quitToHudFunction()
{
	hintIndex = -1;
	Level::singletonInstance->loadLevel(0);
}
void quitToMenuFunction()
{
	
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
//------------------------------------------------------------

Hud::Hud()
{
	//create buttons
	buttons.push_back(new HudButton("mge/textures/ReloadButton.png", sf::Vector2f(0, 0), &reloadFunction));
	buttons.push_back(new HudButton("mge/textures/QuitToHudButton.png", sf::Vector2f(0, 95), &quitToHudFunction));
	buttons.push_back(new HudButton("mge/textures/QuitToMenuButton.png", sf::Vector2f(0, 2 * 95), &quitToMenuFunction));
	buttons.push_back(new HudButton("mge/textures/HintButton.png", sf::Vector2f(0, 3 * 95), &hintFunction));
	buttons.push_back(new HudButton("mge/textures/StatsButton.png", sf::Vector2f(0, 4 * 95), &statsFunction));

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
	GameObject::~GameObject();
}

void Hud::update(float pStep, bool pUpdateWorldTransform)
{
	GameObject::update(pStep, pUpdateWorldTransform);
}

void Hud::setState(int state)
{
	switch (state)
	{
	case 0: //menu
		buttons[0]->setActive(false);
		buttons[1]->setActive(false);
		buttons[2]->setActive(false);
		buttons[3]->setActive(false);
		buttons[4]->setActive(false);
		break;
	case 1: // hub
		buttons[0]->setActive(false);
		buttons[1]->setActive(false);
		buttons[2]->setActive(true);
		buttons[3]->setActive(false);
		buttons[4]->setActive(true);
		break;
	case 2: // normal level
		buttons[0]->setActive(true);
		buttons[1]->setActive(true);
		buttons[2]->setActive(true);
		buttons[3]->setActive(true);
		buttons[4]->setActive(true);
		break;
	case 3:// boss level
		//todo
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