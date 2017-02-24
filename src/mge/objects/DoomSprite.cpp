#include <mge/objects/DoomSprite.hpp>
#include <mge/core/AbstractGame.hpp>


//////////////////////////////|	INSTANCE FUNCTIONS
//Constructor for when we don't want to pass in the position
DoomSprite::DoomSprite (std::string pFilename) : DoomObject ()
{
	pFilename = "images/" + pFilename;
	sf::Texture* texture = JCPPEngine::TextureManager::GetTexture (pFilename);
	if (texture != nullptr)
	{
		_sprite = new sf::Sprite (*texture);
		sf::Vector2u size = texture->getSize ();
		_imageHalfSize = glm::vec2 (size.x * 0.5f, size.y * 0.5f);
		_hitboxHalfSize = _imageHalfSize;
		_hitboxOffset = glm::vec2 (0, 0);
		_sprite->setOrigin (_hitboxHalfSize.x, size.y);//Set origin at center - bottom
		_screenPosition = glm::vec2 (AbstractGame::windowWidth * 0.5f, AbstractGame::windowHeight * 0.5f);
		_sprite->setPosition (_screenPosition.x, _screenPosition.y);
		_valid = true;
	}
	else
	{
		_valid = false;
	}
	setMaterial ((AbstractMaterial*)(this));
}
//Constructor for when we know the position
DoomSprite::DoomSprite (glm::vec3 pPosition, std::string pFilename) : DoomObject (pPosition)
{
	pFilename = "images/" + pFilename;
	sf::Texture* texture = JCPPEngine::TextureManager::GetTexture (pFilename);
	if (texture != nullptr)
	{
		_sprite = new sf::Sprite (*texture);
		sf::Vector2u size = texture->getSize ();
		_imageHalfSize = glm::vec2 (size.x * 0.5f, size.y * 0.5f);
		_hitboxHalfSize = _imageHalfSize;
		_hitboxOffset = glm::vec2 (0, 0);
		_sprite->setOrigin (_hitboxHalfSize.x, size.y);//Set origin at center - bottom
		_screenPosition = glm::vec2 (AbstractGame::windowWidth * 0.5f, AbstractGame::windowHeight * 0.5f);
		_sprite->setPosition (_screenPosition.x, _screenPosition.y);
		_valid = true;
	}
	else
	{
		_valid = false;
	}
	setMaterial ((AbstractMaterial*)(this));
}