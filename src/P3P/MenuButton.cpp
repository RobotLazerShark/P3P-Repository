#include "P3P/MenuButton.hpp"
#include <JCPPEngine/TextureManager.hpp>
#include <JCPPEngine/SoundManager.hpp>

void stopFunctionOpen(int pAnimIndex, GameObject* pOwner)
{
	//Rotate Box To Open State & Set Flags
	MenuButton* menuButton = (MenuButton*)pOwner;
	menuButton->model->rotate(-1.74f, glm::vec3(1, 0, 0));
	menuButton->boxIsOpen = true;
	menuButton->animating = false;
	menuButton->isRotated = true;
}

void stopFunctionClose(int pAnimIndex, GameObject* pOwner)
{
	MenuButton* menuButton = (MenuButton*)pOwner;
	//If type is a box
	if (menuButton->indexOfType == 0) 
	{
		//Rotate it to closed position
		menuButton->model->rotate(1.74f, glm::vec3(1, 0, 0));
		menuButton->boxIsOpen = false;
		menuButton->animating = false;
		menuButton->isRotated = false;
	}
	else
	{
		//Call the buttons press function and set flag
		menuButton->_pressFunction();
		menuButton->animating = false;
	}
}

MenuButton::MenuButton(std::string pSpritePath, std::string pModelPath, sf::Vector2f pSpritePosition,
	glm::mat4 pModelPosition, void(*pPressPunction)(), std::string OnAnim, std::string OffAnim, glm::vec3 pOffset, int pType) : GameObject()
{
	indexOfType = pType;
	_pressFunction = pPressPunction;

	//Set Offset Vec
	offsetVec = glm::vec3(pOffset.x, pOffset.y, pOffset.z);

	//Setup Animatior
	animator = new AnimationBehaviour({ OnAnim, OffAnim });

	//Setup Button Model
	model = new GameObject(pModelPath);
	std::string texture;
	switch (pType)
	{
		case 0:
			texture = "ButtonCap.png";
			break;
		case 1:
			texture = "ButtonRed.png";
			break;
		case 2:
		case 3:
			texture = "ButtonGreen.png";
			break;
		default:
			texture = "ButtonYellow.png";
			break;
	}
	model->setMaterial(new LitMaterial(texture));
	model->setBehaviour(animator);
	model->setParent(this);

	//Set transform and translate
	this->setTransform(pModelPosition);
	this->translate(offsetVec);

	//Set sprites texture and position
	sprite = new sf::Sprite(*JCPPEngine::TextureManager::GetTexture(pSpritePath));
	//sprite->setPosition(pSpritePosition*1.35f); //Scale For 1080p

	//Scale the Buttons And Boxes
	if (indexOfType < 5)
		sprite->setScale(3.4f,3.4f);
	else 
		sprite->setScale(1.5f, 1.5f);

}

MenuButton::~MenuButton()
{
	setParent(nullptr);
	delete sprite;
	GameObject::~GameObject();
}

void MenuButton::OpenButton()
{
	//If the box is not already open
	if (!boxIsOpen) 
	{
		//Play the open animation and set flag
		animator->playAnimation(0, false, true, &stopFunctionOpen, this);
		JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/Button Hover.wav")));
		animating = true;
	}
}

void MenuButton::CloseButton()
{
	//Play the close animation
	animator->playAnimation(1, false, true, &stopFunctionClose, this);
	JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/Button Hover.wav")));
	animating = true;
}

void MenuButton::PushButton()
{
	JCPPEngine::SoundManager::PlaySound (new sf::Sound (*JCPPEngine::SoundManager::GetBuffer ("sounds/Button Click.wav")));
	//If type is not a box
	if (indexOfType != 0) 
	{
		animator->playAnimation(0, false, true, &stopFunctionClose, this);
	}
}