#ifndef MENU_HPP
#define MENU_HPP
//Include files
#include "mge/core/GameObject.hpp"
#include "P3P/MenuButton.hpp"
#include "P3P/SelectWheel.hpp"
#include "Credits.hpp"
#include "mge/materials/LitMaterial.hpp"
#include "mge/objects/Camera.hpp"
#include "mge/behaviours/AnimationBehaviour.hpp"
#include "mge/core/World.hpp"
#include "SFML/Graphics.hpp"
#include <JCPPEngine/AbstractListener.hpp>
#include <P3P/SceneFader.hpp>
#include <P3P/Level.hpp>

class Menu : public GameObject, public JCPPEngine::AbstractListener
{
private:
	GameObject * _menuBackground = nullptr;
	
	MenuButton * _lastSelectedBox = nullptr;
	MenuButton * _buttonBoxOne = nullptr;
	MenuButton * _buttonBoxTwo = nullptr;
	MenuButton * _buttonOne = nullptr;
	MenuButton * _buttonTwo = nullptr;
	MenuButton * _buttonThree = nullptr;
	MenuButton * _buttonNextSkin = nullptr;
	MenuButton * _buttonBoxThree = nullptr;
	MenuButton * _buttonLastSkin = nullptr;
	MenuButton * _buttonSelectSkin = nullptr;
	MenuButton * _lastSelectedButton = nullptr;
	std::vector<MenuButton*> _buttons;
	
public:
	static Menu* singletonInstance;
	sf::RenderWindow * renderWindow = nullptr;
	SelectWheel * selectWheel = nullptr;
	Camera * camera = nullptr;
	World * world = nullptr;
	std::string characterSkin;
	bool doHide = false;
	bool isHidden = false;
	bool createdCredits = false;
	int characterSkinIndex = 0;
	Level* _game = nullptr;
	SceneFader* _fader = nullptr;

	Menu(Camera * pCamera, sf::RenderWindow * pRend, World * pWorld);
	~Menu();

	void render ();
	void ProcessEvent(JCPPEngine::Event* pEvent) override;
	void update(float pStep, bool pUpdateWorldTransform = false) override;
	void Hide();
	void UnHide();
	void CreateButtons(Camera * pCamera);
};
#endif