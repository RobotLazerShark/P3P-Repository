#include "Credits.hpp"
#include <JCPPEngine/MouseEvent.hpp>
#include "P3P/Menu.hpp"
#include "P3P/MenuButton.hpp"

void BackPress()
{
	//Back To Menu
	printf("\nBack Button has Been Pressed \n");
	Credits::singletonInstance->doHide = true;
	Menu::singletonInstance->UnHide();
}

Credits* Credits::singletonInstance = nullptr;

Credits::Credits(Camera * pCamera, sf::RenderWindow * pRend, World * pWorld) : GameObject()
{
	if (singletonInstance != nullptr)
		return;
	singletonInstance = this;

	renderWindow = pRend;
	camera = pCamera;
	world = pWorld;

	registerForEvent(JCPPEngine::Event::EventType::MouseDown);
	CreateButtons(camera);
}

Credits::~Credits()
{
	unregisterForEvent(JCPPEngine::Event::EventType::MouseDown);
	singletonInstance = nullptr;
	setParent(nullptr);
	GameObject::~GameObject();
}

void Credits::ProcessEvent(JCPPEngine::Event* pEvent)
{
	//Deactivate menu PrecessEvent if isHidden
	if (isHidden)
		return;

	JCPPEngine::MouseEvent* mouseDown = (JCPPEngine::MouseEvent*)pEvent;

	if (_lastSelectedButton != nullptr)
	{
		//Call push button function if mouse is above a button
		_lastSelectedButton->PushButton();
	}
}

void Credits::update(float pStep, bool pUpdateWorldTransform)
{
	//Deactivate update if isHidden
	if (isHidden)
		return;

	//If doHide flag is set then "hide" 
	if (doHide)
		Hide();

	for (MenuButton * pMenuButton : _buttons)
	{
		//Render sprites
		renderWindow->pushGLStates();
		renderWindow->draw(*pMenuButton->sprite);
		renderWindow->popGLStates();
	}

	//Foreach button in the vector
	for (MenuButton * pMenuButton : _buttons)
	{
		sf::Sprite * sprite = pMenuButton->sprite;
		sf::Vector2i position = sf::Mouse::getPosition(*renderWindow);

		//If no collision with a sprite is detected
		if (!(position.x >= sprite->getPosition().x && position.x <= sprite->getPosition().x + sprite->getTextureRect().width * sprite->getScale().x
			&&position.y >= sprite->getPosition().y && position.y <= sprite->getPosition().y + sprite->getTextureRect().height * sprite->getScale().y))
		{
			if (pMenuButton->isRotated && !pMenuButton->animating)
			{
				//If the button box is rotated but no mosue is above it
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
			if (position.x >= sprite->getPosition().x && position.x <= sprite->getPosition().x + sprite->getTextureRect().width * sprite->getScale().x
				&&position.y >= sprite->getPosition().y && position.y <= sprite->getPosition().y + sprite->getTextureRect().height * sprite->getScale().y)
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

void Credits::Hide() {
	//Remove all objects in credits
	_buttonBoxOne->setParent(nullptr);
	_buttonOne->setParent(nullptr);

	//Clear vector and set flag
	_buttons.clear();
	isHidden = true;
	doHide = false;
}

void Credits::UnHide() {
	//Create new credits objects
	isHidden = false;
	CreateButtons(camera);
}

void Credits::CreateButtons(Camera * pCamera) {
	//Load credits screen background
	//Empty

	//Setup buttons
	_buttonOne = new MenuButton("mge/textures/White.png", "Play_Button.obj",
		sf::Vector2f(100, 100), pCamera->getTransform(), &BackPress, "ButtonPush.txt", "ButtonPull.txt", glm::vec3(-0.75f, 0.40f, -1), 1);

	_buttonBoxOne = new MenuButton("mge/textures/Default.png", "Play_Button_Case_1.obj",
		sf::Vector2f(100, 100), pCamera->getTransform(), &BackPress, "BoxOpen.txt", "BoxClose.txt", glm::vec3(-0.75f, 0.45f, -1), 0);

	//Set buttons parent
	_buttonBoxOne->setParent(this);
	_buttonOne->setParent(this);


	//Add button to vector
	_buttons.push_back(_buttonBoxOne);
	_buttons.push_back(_buttonOne);
}