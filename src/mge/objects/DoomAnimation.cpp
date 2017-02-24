#include <mge/objects/DoomAnimation.hpp>
#include <mge/core/AbstractGame.hpp>


//////////////////////////////|	INSTANCE FUNCTIONS
//Constructor for when we don't want to pass in the position
DoomAnimation::DoomAnimation (std::string pFilename, int pFrameWidth, int pFrameHeight, float pFrameTime) : DoomObject ()
{
	pFilename = "images/" + pFilename;
	sf::Texture* texture = JCPPEngine::TextureManager::GetTexture (pFilename);
	if (texture != nullptr)
	{
		_columns = ((int)texture->getSize ().x) / pFrameWidth;
		_rows = ((int)texture->getSize ().y) / pFrameHeight;
		_frameWidth = pFrameWidth;
		_frameHeight = pFrameHeight;
		_frameCount = _columns * _rows;
		_frameDelay = pFrameTime;
		_imageHalfSize = glm::vec2 (_frameWidth * 0.5f, _frameHeight * 0.5f);
		_hitboxHalfSize = _imageHalfSize;
		_hitboxOffset = glm::vec2 (0, 0);
		_sprite = new sf::Sprite (*texture, sf::IntRect (0, 0, _frameWidth, _frameHeight));
		_sprite->setOrigin (_hitboxHalfSize.x, _frameHeight);//Set origin at center - bottom
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
DoomAnimation::DoomAnimation (glm::vec3 pPosition, std::string pFilename, int pFrameWidth, int pFrameHeight, float pFrameTime) : DoomObject (pPosition)
{
	pFilename = "images/" + pFilename;
	sf::Texture* texture = JCPPEngine::TextureManager::GetTexture (pFilename);
	if (texture != nullptr)
	{
		_columns = ((int)texture->getSize ().x) / pFrameWidth;
		_rows = ((int)texture->getSize ().y) / pFrameHeight;
		_frameWidth = pFrameWidth;
		_frameHeight = pFrameHeight;
		_frameCount = _columns * _rows;
		_frameDelay = pFrameTime;
		_imageHalfSize = glm::vec2 (_frameWidth * 0.5f, _frameHeight * 0.5f);
		_hitboxHalfSize = _imageHalfSize;
		_hitboxOffset = glm::vec2 (0, 0);
		_sprite = new sf::Sprite (*texture, sf::IntRect (0, 0, _frameWidth, _frameHeight));
		_sprite->setOrigin (_hitboxHalfSize.x, _frameHeight);//Set origin at center - bottom
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


//////////////////////////////|	GAMEOBJECT FUNCTIONS
void DoomAnimation::update (float pStep)
{
	//update animation
	if (_frameTimer >= _frameDelay)
	{
		_frameTimer = 0;
		_currentFrame = (_currentFrame + 1)%_frameCount;
		int col = _currentFrame%_columns;
		int row = _currentFrame/_rows;
		_sprite->setTextureRect (sf::IntRect (col * _frameWidth, row * _frameHeight, _frameWidth, _frameHeight));
	}
	else
	{
		_frameTimer += pStep;
	}
	GameObject::update (pStep);
}