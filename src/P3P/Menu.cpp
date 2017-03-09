#include "P3P/Menu.hpp"
#include <JCPPEngine/MouseEvent.hpp>
#include <mge/core/AbstractGame.hpp>
#include <mge/objects/Light.hpp>
#include <mge/util/ShaderDataUtil.hpp>

void NoPress()
{
	//Empty
}

void ButtonPress()
{
	//Render a temporary image in case loading the level takes a little while
	Menu::singletonInstance->renderWindow->pushGLStates();
	Menu::singletonInstance->renderWindow->draw(sf::Sprite (*JCPPEngine::TextureManager::GetTexture ("images/NoSignal.jpg")));
	Menu::singletonInstance->renderWindow->draw(sf::Sprite (*JCPPEngine::TextureManager::GetTexture ("images/Hud.png")));
	Menu::singletonInstance->renderWindow->popGLStates();
	Menu::singletonInstance->renderWindow->display ();
	//Set up P3P hub (starts the game)
	Menu::singletonInstance->doHide = true;
	AbstractGame::showCursor (false);
	Menu::singletonInstance->_fader = new SceneFader (Menu::singletonInstance->renderWindow);
	Menu::singletonInstance->_fader->setParent (World::singletonInstance);
	Menu::singletonInstance->_game = new Level (Menu::singletonInstance->characterSkinIndex + 1, Menu::singletonInstance->renderWindow);
}

void ButtonOnePress()
{
	//Credits
	if (Menu::singletonInstance->createdCredits)
	{
		Credits::singletonInstance->UnHide();
	} 
	else if(!Menu::singletonInstance->createdCredits) 
	{
		Credits * credits = new Credits(Menu::singletonInstance->camera, Menu::singletonInstance->renderWindow, World::singletonInstance);
		World::singletonInstance->add(credits);
		Menu::singletonInstance->createdCredits = true;
		Credits::singletonInstance->isHidden = false;
	}
	Menu::singletonInstance->doHide = true;
}

void ButtonTwoPress()
{
	//Exit game
	AbstractGame::singletonInstance->Stop ();
}

void NextPress()
{
	//Next skin
	Menu::singletonInstance->selectWheel->NextTexture();
}

void LastPress()
{
	//Last skin
	Menu::singletonInstance->selectWheel->LastTexture();
}

void SelectPress()
{
	//Select Current Skin
	Menu::singletonInstance->selectWheel->SelectTexture();
}

Menu* Menu::singletonInstance = nullptr;

Menu::Menu(Camera * pCamera, sf::RenderWindow * pRend, World * pWorld) : GameObject()
{
	if (singletonInstance != nullptr)
	{
		delete singletonInstance;
	}
	singletonInstance = this;

	renderWindow = pRend;
	camera = pCamera;
	world = pWorld;

	//Add light
	ShaderDataUtil::SetAmbientLight (glm::vec3 (0.8f, 0.9f, 1), 0.1f);
	Light* dirLight = new Light (glm::vec3 (1, 1, 1), 0.45f);
	dirLight->rotate (glm::radians (25.0f), glm::vec3 (0, 1, 0));
	dirLight->rotate (glm::radians (45.0f), glm::vec3 (1, 0, 0));
	dirLight->setParent (World::singletonInstance);

	registerForEvent(JCPPEngine::Event::EventType::MouseDown);
	CreateButtons(camera);
	AbstractGame::showCursor (true);
}

Menu::~Menu()
{
	if (_fader != nullptr)
	{
		_fader->setParent(nullptr);
		delete _fader;
		_fader = nullptr;
	}
	if (_game != nullptr)
	{
		_game->clear();
		delete _game;
		_game = nullptr;
	}
	unregisterForEvent(JCPPEngine::Event::EventType::MouseDown);
	singletonInstance = nullptr;
	setParent(nullptr);
	GameObject::~GameObject();
}

void Menu::ProcessEvent(JCPPEngine::Event* pEvent)
{
	//Deactivate precessEvent if isHidden
	if (isHidden)
		return;

	JCPPEngine::MouseEvent* mouseDown = (JCPPEngine::MouseEvent*)pEvent;

	if (_lastSelectedButton != nullptr) 
	{
		//Call push button function if mouse is above a button
		_lastSelectedButton->PushButton();
	}

}

void Menu::render ()
{
	//Don't render if hidden
	if (isHidden)
		return;

	for (MenuButton * pMenuButton: _buttons)
	{
		//Render sprites
		renderWindow->pushGLStates();
		renderWindow->draw(*pMenuButton->sprite);
		renderWindow->popGLStates();
	}
}

void Menu::update(float pStep, bool pUpdateWorldTransform)
{
	//Deactivate update if isHidden
	if (isHidden)
		return;

	//If doHide flag is set then "hide" 
	if (doHide)
		Hide();

	//Foreach button in the vector
	for (MenuButton * pMenuButton : _buttons)
	{
		sf::Sprite * sprite = pMenuButton->sprite;
		sf::Vector2i position = sf::Mouse::getPosition(*renderWindow);
		sf::Vector2f spritePos = sprite->getPosition ();

		//If no collision with a sprite is detected
		if (!(position.x >= spritePos.x && position.x <= spritePos.x + sprite->getTextureRect().width * sprite->getScale().x
			&&position.y >= spritePos.y && position.y <= spritePos.y + sprite->getTextureRect().height * sprite->getScale().y))
		{
			if (pMenuButton->isRotated && !pMenuButton->animating)
			{
				//If the button box  is rotated but no mosue is above it
				if (pMenuButton->indexOfType == 0) 
				{
					pMenuButton->CloseButton();
				}
				_lastSelectedButton = nullptr;
				_lastSelectedBox = nullptr;
			}
		}

		//If the button is not currently animating and the mosue is above it
		if (!pMenuButton->animating)
		{
			if (position.x >= spritePos.x && position.x <= spritePos.x + sprite->getTextureRect().width * sprite->getScale().x
				&&position.y >= spritePos.y && position.y <= spritePos.y + sprite->getTextureRect().height * sprite->getScale().y)
			{
				//If the button is not currently selected
				if (pMenuButton != _lastSelectedButton)
				{
					//If its a box, open it
					if (pMenuButton->indexOfType == 0) 
					{
						//Select Box
						_lastSelectedBox = pMenuButton;
						pMenuButton->OpenButton();
					}
					//Select Button
					_lastSelectedButton = pMenuButton;
				}
			}
			else
			{
				//If the button is selected button or box but the mosue is not over it
				if (pMenuButton == _lastSelectedButton || pMenuButton == _lastSelectedBox)
				{
					if (pMenuButton->indexOfType == 0) 
					{
						//If its a box close it
						pMenuButton->CloseButton();
					}
					//Deselect buttons
					_lastSelectedButton = nullptr;
					_lastSelectedBox = nullptr;
				}
			}
		}
	}
	GameObject::update(pStep, pUpdateWorldTransform);
}

void Menu::Hide() {
	setParent (nullptr);
	isHidden = true;
	doHide = false;
}

void Menu::UnHide(bool pDeleteLevel) {
	//Create New Menu Objects
	setParent (World::singletonInstance);
	isHidden = false;

	if (pDeleteLevel)
	{
		_fader->setParent (nullptr);
		delete _fader;
		_fader = nullptr;
		_game->clear ();
		delete _game;
		_game = nullptr;
		AbstractGame::showCursor (true);
	}
}

void Menu::CreateButtons(Camera * pCamera) {
	//Setup background & selectwheel
	_menuBackground = new GameObject("Menu_Background.obj");
	_menuBackground->setParent(this);
	_menuBackground->setMaterial(new LitMaterial("MenuBackground.png"));
	_menuBackground->setTransform(pCamera->getTransform());
	_menuBackground->translate(glm::vec3(0, 0, -1.09f));
	_menuBackground->scale(2.25f);

	selectWheel = new SelectWheel("mge/textures/White.png", "Menu_Wheel_Inside.obj", "Menu_Wheel_Outside.obj", "cube_flat.obj",
		sf::Vector2f(200, 100), pCamera->getTransform(), &NoPress, "WheelNext.txt", "WheelLast.txt", glm::vec3(0.85f, -0.15f, -0.985f), "General_Button.obj", this);

	//Setup buttons
	_buttonOne = new MenuButton("mge/textures/White.png", "Play_Button.obj",
		sf::Vector2f(500, 100), pCamera->getTransform(), &ButtonPress, "ButtonPush.txt", "ButtonPull.txt", glm::vec3(-0.221f, 0.297f, -0.985f), 1);
	_buttonTwo = new MenuButton("mge/textures/White.png", "General_Button.obj",
		sf::Vector2f(480, 420), pCamera->getTransform(), &ButtonOnePress, "ButtonPush.txt", "ButtonPull.txt", glm::vec3(-0.22f, -0.106f, -0.985f), 2);
	_buttonThree = new MenuButton("mge/textures/White.png", "General_Button.obj",
		sf::Vector2f(480, 630), pCamera->getTransform(), &ButtonTwoPress, "ButtonPush.txt", "ButtonPull.txt", glm::vec3(-0.22f, -0.43f, -0.985f), 3);
	_buttonBoxOne = new MenuButton("mge/textures/Default.png", "Play_Button_Case_2.obj",
		sf::Vector2f(500, 100), pCamera->getTransform(), &NoPress, "BoxOpen.txt", "BoxClose.txt", glm::vec3(-0.22f, 0.4685f, -0.985f), 0);
	_buttonBoxTwo = new MenuButton("mge/textures/Default.png", "General_Button_Case.obj",
		sf::Vector2f(480, 420), pCamera->getTransform(), &NoPress, "BoxOpen.txt", "BoxClose.txt", glm::vec3(-0.22f, -0.005f, -0.985f), 0);
	_buttonBoxThree = new MenuButton("mge/textures/Default.png", "General_Button_Case.obj",
		sf::Vector2f(480, 630), pCamera->getTransform(), &NoPress, "BoxOpen.txt", "BoxClose.txt", glm::vec3(-0.22f, -0.307f, -0.985f), 0);
	_buttonLastSkin = new MenuButton("mge/textures/Default.png", "General_Button.obj",
		sf::Vector2f(1160, 90), pCamera->getTransform(), &LastPress, "ButtonPush.txt", "ButtonPull.txt", glm::vec3(0.68f, 0.4f, -0.96f), 6);
	_buttonNextSkin = new MenuButton("mge/textures/Default.png", "General_Button.obj",
		sf::Vector2f(1075, 120), pCamera->getTransform(), &NextPress, "ButtonPush.txt", "ButtonPull.txt", glm::vec3(0.57f, 0.35f, -0.96f), 5);
	_buttonSelectSkin = new MenuButton("mge/textures/Default.png", "Play_Button.obj",
		sf::Vector2f(1250, 450), pCamera->getTransform(), &SelectPress, "ButtonPush.txt", "ButtonPull.txt", glm::vec3(0.84f, -0.15f, -0.92f), 4);

	//Scale buttons
	_buttonBoxOne->scale(2.25f);
	_buttonBoxTwo->scale(2.25f);
	_buttonBoxThree->scale(2.25f);
	_buttonOne->scale(2.4f);
	_buttonTwo->scale(2.4f);
	_buttonThree->scale(2.4f);
	_buttonLastSkin->scale(1.5f);
	_buttonNextSkin->scale(1.5f);
	_buttonSelectSkin->scale(1.5f);

	//Set buttons barent
	_buttonBoxOne->setParent(this);
	_buttonBoxTwo->setParent(this);
	_buttonBoxThree->setParent(this);
	_menuBackground->setParent(this);
	selectWheel->setParent(this);
	_buttonOne->setParent(this);
	_buttonTwo->setParent(this);
	_buttonThree->setParent(this);
	_buttonNextSkin->setParent(this);
	_buttonLastSkin->setParent(this);
	_buttonSelectSkin->setParent(this);

	//Add button to vector
	_buttons.push_back(_buttonBoxOne);
	_buttons.push_back(_buttonBoxTwo);
	_buttons.push_back(_buttonBoxThree);
	_buttons.push_back(_buttonOne);
	_buttons.push_back(_buttonTwo);
	_buttons.push_back(_buttonThree);
	_buttons.push_back(_buttonNextSkin);
	_buttons.push_back(_buttonLastSkin);
	_buttons.push_back(_buttonSelectSkin);
}