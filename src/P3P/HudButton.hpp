#ifndef HUDBUTTON_Def
#define HUDBUTTON_Def

#include <mge/core/GameObject.hpp>
#include <SFML/Graphics.hpp>

class HudButton
{
public:
	HudButton(std::string hitBoxPath, std::string spritePath, sf::Vector2f hitBoxPos, sf::Vector2f spritePos, void(* function)(), bool pDebugMode);
	~HudButton();
	void setActive(bool active);
	
	std::vector<sf::Drawable*> getAllDrawables();
	void tryPressing(sf::Vector2f mousePos);
	bool tryHovering(sf::Vector2f mousePos);

	sf::Sprite * _hitbox = nullptr;
	sf::Sprite * _sprite = nullptr;

private:
	bool _active = true;

	void(*pressFunction)() = nullptr;
	void showSprite(bool show);
	void press();

	bool _debugMode;
};

#endif