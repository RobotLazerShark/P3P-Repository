#ifndef CREDITS_HPP
#define CREDITS_HPP
//Include files
#include "mge/core/GameObject.hpp"
#include "P3P/MenuButton.hpp"
#include "mge/materials/LitMaterial.hpp"
#include "mge/objects/Camera.hpp"
#include "mge/behaviours/AnimationBehaviour.hpp"
#include "mge/core/World.hpp"
#include "SFML/Graphics.hpp"
#include "JCPPEngine/AbstractListener.hpp"

class Menu;

class Credits : public GameObject, public JCPPEngine::AbstractListener
{
private:
	MenuButton * _lastSelectedButton = nullptr;
	MenuButton * _lastSelectedBox = nullptr;
	MenuButton * _buttonBoxOne = nullptr;
	MenuButton * _buttonOne = nullptr;
	MenuButton * _background = nullptr;
	std::vector<MenuButton*> _buttons;

public:
	static Credits* singletonInstance;
	sf::RenderWindow * renderWindow = nullptr;
	Camera * camera = nullptr;
	World * world = nullptr;
	bool doHide = false;
	bool isHidden = true;

	Credits(Camera * pCamera, sf::RenderWindow * pRend, World * pWorld);
	~Credits();

	void ProcessEvent(JCPPEngine::Event* pEvent) override;
	void update(float pStep, bool pUpdateWorldTransform = false) override;
	void Hide();
	void UnHide();
	void CreateButtons(Camera * pCamera);
};
#endif