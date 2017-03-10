#ifndef MENUBUTTON_HPP
#define MENUBUTTON_HPP
//Include files
#include "mge/core/GameObject.hpp"
#include "mge/materials/LitMaterial.hpp"
#include "mge/behaviours/AnimationBehaviour.hpp"
#include "SFML/Graphics.hpp"

class MenuButton : public GameObject
{
private:
public:
	AnimationBehaviour * animator = nullptr;
	void(*_pressFunction)() = nullptr;
	glm::vec3 offsetVec = glm::vec3 (0,0,0);
	sf::Sprite * sprite = nullptr;
	GameObject * model = nullptr;
	bool boxIsOpen = false;
	bool animating = false;
	bool isRotated = false;
	int indexOfType = 0;

	MenuButton(std::string pSpritePath, std::string pModelPath, sf::Vector2f pSpritePosition, 
		glm::mat4 pModelPosition, void(*pPressPunction)(), std::string OnAnim, std::string OffAnim, glm::vec3 pModelOffset, int pType);
	~MenuButton();

	void OpenButton();
	void CloseButton();
	void PushButton();
};
#endif