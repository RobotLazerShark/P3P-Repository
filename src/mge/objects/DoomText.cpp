#include <mge/objects/DoomText.hpp>
#include <mge/lua/LuaParser.hpp>
#include <mge/core/AbstractGame.hpp>


//////////////////////////////|	INSTANCE FUNCTIONS
//Empty constructor
DoomText::DoomText (std::string pText, std::string pFontFile, unsigned int pSize, glm::vec3 pColor) : DoomObject ()
{
	pFontFile = "fonts/" + pFontFile;
	_text = new sf::Text (pText, *JCPPEngine::FontManager::GetFont (pFontFile), pSize);
	sf::FloatRect size = _text->getLocalBounds ();
	_imageHalfSize = glm::vec2 (size.width * 0.5f, size.height * 0.5f);
	_hitboxHalfSize = _imageHalfSize;
	_hitboxOffset = glm::vec2 (0, 0);
	_text->setOrigin (_hitboxHalfSize.x, _hitboxHalfSize.y);//Set origin at center - center
	_screenPosition = glm::vec2 (AbstractGame::windowWidth * 0.5f, AbstractGame::windowHeight * 0.5f);
	_text->setPosition (_screenPosition.x, _screenPosition.y);
	_text->setFillColor (sf::Color ((int)(pColor.r * 255), (int)(pColor.g * 255), (int)(pColor.b * 255)));
	_valid = true;
	setMaterial ((AbstractMaterial*)(this));
}
//Constructor for when we know the position
DoomText::DoomText (std::string pText, std::string pFontFile, unsigned int pSize, glm::vec3 pPosition, glm::vec3 pColor) : DoomObject (pPosition)
{
	pFontFile = "fonts/" + pFontFile;
	_text = new sf::Text (pText, *JCPPEngine::FontManager::GetFont (pFontFile), pSize);
	sf::FloatRect size = _text->getLocalBounds ();
	_imageHalfSize = glm::vec2 (size.width * 0.5f, size.height * 0.5f);
	_hitboxHalfSize = _imageHalfSize;
	_hitboxOffset = glm::vec2 (0, 0);
	_text->setOrigin (_hitboxHalfSize.x, size.height);//Set origin at center - bottom
	_screenPosition = glm::vec2 (AbstractGame::windowWidth * 0.5f, AbstractGame::windowHeight * 0.5f);
	_text->setPosition (_screenPosition.x, _screenPosition.y);
	_text->setFillColor (sf::Color ((int)(pColor.r * 255), (int)(pColor.g * 255), (int)(pColor.b * 255)));
	_valid = true;
	setMaterial ((AbstractMaterial*)(this));
}


//////////////////////////////|	MATERIAL FUNCTIONS
void DoomText::render (Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix)
{
	//Calculate screen position from 3D worldspace position
	glm::mat4 VPmatrix = pProjectionMatrix * pViewMatrix;
	glm::vec4 worldPos = glm::vec4 (getWorldPosition (), 1);
	glm::vec4 screenPos = VPmatrix * worldPos;
	screenPos.x /= screenPos.w;
	screenPos.y /= screenPos.w;
	screenPos.z /= screenPos.w;
	_screenPosition = glm::vec2 ((screenPos.x + 1) * AbstractGame::windowHalfWidth, (1 - screenPos.y) * AbstractGame::windowHalfHeight);
	_text->setPosition (_screenPosition.x, _screenPosition.y);
	_imageScale = getWorldUniformScale () * std::max (0.0f, (1 - screenPos.z) * SCALE_FACTOR);
	_text->setScale (_imageScale, _imageScale);
	//Draw the image
	if (_valid)
	{
		LuaParser::_window->pushGLStates ();
		LuaParser::_window->draw (*_text);
		LuaParser::_window->popGLStates ();
	}
}