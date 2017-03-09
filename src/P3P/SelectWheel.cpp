#include "P3P/SelectWheel.hpp"
#include "P3P/Menu.hpp"
#include <JCPPEngine/TextureManager.hpp>

void stopFunctionNext(int pAnimIndex, GameObject* pOwner)
{
	SelectWheel* selectWheel = (SelectWheel*)pOwner;
	selectWheel->animating = false;
	SelectWheel::singletonInstance->DisplayCharacter(SelectWheel::singletonInstance->onSkin);
}

void stopFunctionLast(int pAnimIndex, GameObject* pOwner)
{
	SelectWheel* selectWheel = (SelectWheel*)pOwner;
	selectWheel->animating = false;
	SelectWheel::singletonInstance->DisplayCharacter(SelectWheel::singletonInstance->onSkin);
}

SelectWheel* SelectWheel::singletonInstance = nullptr;

SelectWheel::SelectWheel(std::string pSpritePath, std::string pBackModelPath, std::string pFrontModelPath, std::string pCharacterModelPath, sf::Vector2f pSpritePosition,
	glm::mat4 pModelPosition, void(*pPressPunction)(), std::string NextCharAnim, std::string LastCharAnim, glm::vec3 pModelOffset, std::string pSelectButton, Menu *pMenu) : GameObject()
{
	if (singletonInstance != nullptr)
		return;
	singletonInstance = this;

	_menu = pMenu;
	_pressFunction = pPressPunction;

	//Create skins 
	sf::String SkinOne = "PlayerBase1.png";
	sf::String SkinTwo = "PlayerBase2.png";
	sf::String SkinThree = "PlayerBase3.png";
	sf::String SkinFour = "PlayerBase4.png";

	//Add skins to vector
	_skins.push_back(SkinOne);
	_skins.push_back(SkinTwo);
	_skins.push_back(SkinThree);
	_skins.push_back(SkinFour);

	//Set max skin size
	_maxSkins = _skins.size();

	//Setup Select Wheel
	offsetVec = glm::vec3(pModelOffset.x, pModelOffset.y, pModelOffset.z);
	animator = new AnimationBehaviour({ NextCharAnim, LastCharAnim });

	backModel = new GameObject(pBackModelPath);
	backModel->setParent(this);
	backModel->setMaterial(new LitMaterial("SelectWheelInner.png"));
	backModel->setBehaviour(animator);

	frontModel = new GameObject(pFrontModelPath);
	frontModel->setParent(this);
	frontModel->setMaterial(new LitMaterial("SelectWheelCase.png"));

	characterModel = new GameObject(pCharacterModelPath);
	characterModel->setParent(this);
	characterModel->setMaterial(new LitMaterial(_skins[0]));
	characterModel->scale(0.1f);
	characterModel->translate(glm::vec3(-1.8f, 0, 0));

	_selectButton = new GameObject(pSelectButton);
	_selectButton->setParent(this);
	_selectButton->setMaterial(new LitMaterial("ButtonGreen.png"));
	_selectButton->scale(0.5f);
	_selectButton->translate(glm::vec3(-0.55f, 0, 0.1f));

	//Set transform and translate
	this->setTransform(pModelPosition);
	this->translate(offsetVec);

	//Set sprite texture and position
	sprite = new sf::Sprite(*JCPPEngine::TextureManager::GetTexture(pSpritePath));
	sprite->setPosition(pSpritePosition);

	//Set  skin index
	_menu->characterSkin = _skins[onSkin];
	_menu->characterSkinIndex = onSkin;
}

SelectWheel::~SelectWheel()
{
	setParent(nullptr);
	singletonInstance = nullptr;
	delete sprite;
	GameObject::~GameObject();
}

void SelectWheel::NextTexture()
{
	//If skin is less then max and the wheel is not animating
	if (onSkin < (_maxSkins-1) && !animating) {
		//Play next texture animation and set onSkin int
		animator->playAnimation(0, false, true, &stopFunctionNext, this);
		animating = true;
		onSkin++;
	}
}

void SelectWheel::LastTexture()
{
	//If skin is greater then the minimum and the wheel is not animating
	if (onSkin > 0 && !animating) {
		//Play next texture animation and set onSkin int
		animator->playAnimation(1, false, true, &stopFunctionLast, this);
		animating = true;
		onSkin--;
	}
}

void SelectWheel::ShowSelectLight()
{
	//Move the select light into view
	_selectButton->translate(glm::vec3(0, 0, +2));
}

void SelectWheel::HideSelectLight()
{
	//Move the select light out of view
	_selectButton->translate(glm::vec3(0, 0, -2));
}

void SelectWheel::SelectTexture()
{
	//Set skin valiables
	_menu->characterSkin = _skins[onSkin];
	_menu->characterSkinIndex = onSkin;
	_selectedSkin = onSkin;
	if (_selectLightHidden)
	{
		//If light is hidden show it and set flag
		ShowSelectLight();
		_selectLightHidden = false;
	}
}

void SelectWheel::DisplayCharacter(int pskinNumber)
{
	//Set characters material euqal the skin in vector at index pskinNumber
	characterModel->setMaterial(new LitMaterial(_skins[pskinNumber]));

	//If the skin number is equal to selected skin and the select "light" is hidden
	if (pskinNumber == _selectedSkin && _selectLightHidden) 
	{
		//Show "light" and set flag
		ShowSelectLight();
		_selectLightHidden = false;
	}
	else if (!_selectLightHidden)
	{
		//Hide "light" and set flag
		HideSelectLight();
		_selectLightHidden = true;
	}
}