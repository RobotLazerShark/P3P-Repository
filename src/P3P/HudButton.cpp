#include  <P3P/HudButton.hpp>

HudButton::HudButton(std::string path, sf::Vector2f pos, void(*function)())
{
	sf::Texture * texture = new sf::Texture();
	texture->loadFromFile(path);

	_sprite = new sf::Sprite();
	_sprite->setTexture(*texture);
	_sprite->setPosition(pos);

	pressFunction = function;
}

HudButton::~HudButton()
{
	delete _sprite;
}

void HudButton::setActive(bool active)
{
	_active = active;
	if (_active)
	{
		_sprite->setColor(sf::Color(255, 255, 255, 255));
	}
	else
	{
		_sprite->setColor(sf::Color(255, 255, 255, 0));
	}
}

void HudButton::press()
{
	if (_active)
	{
		pressFunction();
	}
}

