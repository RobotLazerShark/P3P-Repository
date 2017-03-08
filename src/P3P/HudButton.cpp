#include  <P3P/HudButton.hpp>
#include <JCPPEngine/TextureManager.hpp>

HudButton::HudButton(std::string hitBoxPath, std::string spritePath, sf::Vector2f hitBoxPos, sf::Vector2f spritePos, void(*function)(), bool pDebugMode)
{
	_debugMode = pDebugMode;

	_hitbox = new sf::Sprite(*JCPPEngine::TextureManager::GetTexture (hitBoxPath));
	_hitbox->setPosition(hitBoxPos);

	if (spritePath != "")
	{
		_sprite = new sf::Sprite(*JCPPEngine::TextureManager::GetTexture(spritePath));
		_sprite->setPosition(spritePos);
	}
	pressFunction = function;
}

HudButton::~HudButton()
{
	delete _hitbox;
}

void HudButton::setActive(bool active)
{
	_active = active;
	if (_active)
	{
		_hitbox->setColor(sf::Color(255, 255, 255, 255));
	}
	else
	{
		_hitbox->setColor(sf::Color(255, 255, 255, 0));
	}
}

void HudButton::showSprite(bool show)
{
	if (show)
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

std::vector<sf::Drawable*> HudButton::getAllDrawables()
{
	std::vector< sf::Drawable*> drawables;
	if (_active)
	{
		drawables.push_back(_sprite);
		if (_debugMode)
		{
			drawables.push_back(_hitbox);
		}
	}
	return drawables;
}

bool HudButton::tryHovering(sf::Vector2f mousePos)
{
	bool atLeastOneButtonIsSelected = false;

	if (_sprite != nullptr)
	{
		if (_active)
		{
			if (mousePos.x >= _hitbox->getPosition().x && mousePos.x <= _hitbox->getPosition().x + _hitbox->getTextureRect().width
				&&mousePos.y >= _hitbox->getPosition().y && mousePos.y <= _hitbox->getPosition().y + _hitbox->getTextureRect().height)
			{
				showSprite(true);
				atLeastOneButtonIsSelected = true;
			}
			else
			{
				showSprite(false);
			}
		}
		else
		{
			showSprite(false);
		}
	}
	return atLeastOneButtonIsSelected;
}

void HudButton::tryPressing(sf::Vector2f mousePos)
{
	//if mouse is on top of a button
	if (mousePos.x >= _hitbox->getPosition().x && mousePos.x <= _hitbox->getPosition().x + _hitbox->getTextureRect().width
		&&mousePos.y >= _hitbox->getPosition().y && mousePos.y <= _hitbox->getPosition().y + _hitbox->getTextureRect().height)
	{
		//press button
		press();
	}
}