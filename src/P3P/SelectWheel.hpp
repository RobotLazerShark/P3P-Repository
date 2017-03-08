#ifndef SELECTWHEEL_HPP
#define SELECTWHEEL_HPP
//Include files
#include "mge/core/GameObject.hpp"
#include "mge/materials/LitMaterial.hpp"
#include "mge/behaviours/AnimationBehaviour.hpp"
#include <SFML/Graphics.hpp>

class Menu;

class SelectWheel : public GameObject
{
private:
	GameObject * _selectButton = nullptr;
	std::vector<sf::String> _skins;
	Menu * _menu = nullptr;
	bool _selectLightHidden = false;
	int _selectedSkin = 0;
	int _maxSkins = 0;
	
public:
	static SelectWheel* singletonInstance;
	void(*_pressFunction)() = nullptr;
	AnimationBehaviour * animator = nullptr;
	GameObject * characterModel = nullptr;
	GameObject * frontModel = nullptr;
	GameObject * backModel = nullptr;
	glm::vec3 offsetVec = glm::vec3 (0,0,0);
	sf::Sprite * sprite = nullptr;
	bool animating = false;
	int onSkin = 0;

	SelectWheel(std::string pSpritePath, std::string pBackModelPath, std::string pFrontModelPath, std::string pCharacterModelPath, sf::Vector2f pSpritePosition,
		glm::mat4 pModelPosition, void(*pPressPunction)(), std::string NextCharAnim, std::string LastCharAnim, glm::vec3 pModelOffset, std::string pSelectButton, Menu *pMenu);
	~SelectWheel();

	void NextTexture();
	void LastTexture();
	void ShowSelectLight();
	void HideSelectLight();
	void SelectTexture();
	void DisplayCharacter(int pskinNumber);
};
#endif