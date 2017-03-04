#include <mge/objects/DoomObject.hpp>
#include <mge/lua/LuaParser.hpp>
#include <mge/core/AbstractGame.hpp>


//////////////////////////////|	INSTANCE FUNCTIONS
//Empty constructor
DoomObject::DoomObject () : GameObject () {}
//Constructor for when we know the position
DoomObject::DoomObject (glm::vec3 pPosition) : GameObject ("DoomObject", pPosition) {}
//Destructor
DoomObject::~DoomObject ()
{
	_material = nullptr;
	GameObject::~GameObject ();
}


//////////////////////////////|	INTERACTION
//Check if this image was clicked this frame
bool DoomObject::CheckIfClicked (sf::Vector2i pMouseClickPosition)
{
	int mouseX = pMouseClickPosition.x;
	int mouseY = pMouseClickPosition.y;
	glm::vec2 pos = _screenPosition + _hitboxOffset;
	pos.y -= _imageHalfSize.y * 2 * _imageScale;//Hitbox should be relative to center of image
	if
	(
		(mouseX >= pos.x - (_hitboxHalfSize.x * _imageScale) && mouseX <= pos.x + (_hitboxHalfSize.x * _imageScale)) &&
		(mouseY >= pos.y && mouseY <= pos.y + (_hitboxHalfSize.y * _imageScale * 2))
	)
	{
		return true;
	}
	return false;
}
//Set the 'hitbox' used to check for clicking
void DoomObject::SetClickHitbox (glm::vec2 pOffset, glm::vec2 pHalfSize)
{
	_hitboxOffset = pOffset;
	_hitboxHalfSize = pHalfSize;
}


//////////////////////////////|	MATERIAL FUNCTIONS
void DoomObject::render (Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix)
{
	//Calculate screen position from 3D worldspace position
	glm::mat4 VPmatrix = pProjectionMatrix * pViewMatrix;
	glm::vec4 worldPos = glm::vec4 (getWorldPosition (), 1);
	glm::vec4 screenPos = VPmatrix * worldPos;
	screenPos.x /= screenPos.w;
	screenPos.y /= screenPos.w;
	screenPos.z /= screenPos.w;
	_screenPosition = glm::vec2 ((screenPos.x + 1) * AbstractGame::windowHalfWidth, (1 - screenPos.y) * AbstractGame::windowHalfHeight);
	_sprite->setPosition (_screenPosition.x, _screenPosition.y);
	_imageScale = getWorldUniformScale () * std::max (0.0f, (1 - screenPos.z) * SCALE_FACTOR);
	_sprite->setScale (_imageScale, _imageScale);
	//Draw the image
	if (_valid)
	{
		LuaParser::_window->pushGLStates ();
		LuaParser::_window->draw (*_sprite);
		LuaParser::_window->popGLStates ();
	}
}