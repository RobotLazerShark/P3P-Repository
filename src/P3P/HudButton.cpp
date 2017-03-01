#include  <P3P/HudButton.hpp>
#include <JCPPEngine/TextureManager.hpp>

HudButton::HudButton(std::string path, sf::Vector2f pos, void(*function)())
{
	_sprite = new sf::Sprite(*JCPPEngine::TextureManager::GetTexture (path));
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

