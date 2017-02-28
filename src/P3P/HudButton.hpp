#ifndef HUDBUTTON_Def
#define HUDBUTTON_Def

#include <mge/core/GameObject.hpp>
#include <SFML/Graphics.hpp>

class HudButton
{
public:
	HudButton(std::string path, sf::Vector2f pos, void(* function)()); 
	~HudButton();
	void setActive(bool active);
	

	sf::Sprite * _sprite = nullptr;
	bool _active = true;

	void press();

private:
	void(*pressFunction)() = nullptr;
};

#endif