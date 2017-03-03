#include <mge/objects/Light.hpp>
#include <mge/util/ShaderDataUtil.hpp>
#include <mge/core/Mesh.hpp>
#include <mge/materials/ColorMaterial.hpp>
#include <mge/core/World.hpp>


//////////////////////////////|	INSTANCE FUNCTIONS
//Directional light constructor
Light::Light (glm::vec3 pColor, float pIntensity) : shape (Shapes::Directional), GameObject ()
{
	_color = glm::vec4 (pColor, 1.0f);
	_intensity = pIntensity;
	_normal = glm::vec4 (0.0f, -1.0f, 0.0f, 0.0f);//Default direction is down. Rotate object for different directions.
}
//Point light constructor
Light::Light (glm::vec3 pColor, float pIntensity, glm::vec2 pFalloff) : shape (Shapes::Point), GameObject ()
{
	_color = glm::vec4 (pColor, 1.0f);
	_intensity = pIntensity;
	_falloff = pFalloff;
	_transform [3] = glm::vec4 (0, 0, 0, 1);
}
Light::Light (glm::vec3 pColor, float pIntensity, glm::vec3 pPosition, glm::vec2 pFalloff) : shape (Shapes::Point), GameObject ()
{
	_color = glm::vec4 (pColor, 1.0f);
	_intensity = pIntensity;
	_falloff = pFalloff;
	setLocalPosition (pPosition);
}
//Spot light constructor (Note: angles are always in radians, use glm::radians () to convert from degrees)
Light::Light (glm::vec3 pColor, float pIntensity, float pAngle, glm::vec2 pFalloff) : shape (Shapes::Spot), GameObject ()
{
	_color = glm::vec4 (pColor, 1.0f);
	_intensity = pIntensity;
	_normal = glm::vec4 (0.0f, -1.0f, 0.0f, 0.0f);//Default direction is down. Rotate object for different directions.
	_angle = pAngle;
	_falloff = pFalloff;
}
Light::Light (glm::vec3 pColor, float pIntensity, glm::vec3 pPosition, float pAngle, glm::vec2 pFalloff) : shape (Shapes::Spot), GameObject ()
{
	_color = glm::vec4 (pColor, 1.0f);
	_intensity = pIntensity;
	_normal = glm::vec4 (0.0f, -1.0f, 0.0f, 0.0f);//Default direction is down. Rotate object for different directions.
	_angle = pAngle;
	_falloff = pFalloff;
	setLocalPosition (pPosition);
}
//Destructor
Light::~Light () {}
//Setting a parent
void Light::setParent (GameObject* pParent)
{
	bool oldPartOfWorld = _partOfWorld;
	GameObject::setParent (pParent);
	if (oldPartOfWorld != _partOfWorld)
	{
		if (_partOfWorld)
		{
			ShaderDataUtil::AddLight (this);//When this light is a part of the world, its data should be used by the shaders
		}
		else
		{
			ShaderDataUtil::RemoveLight (this);//When this light is not a part of the world, the shaders shouldn't use its data
		}
	}
}


//////////////////////////////|	SETTERS
//Set this light's color (All)
void Light::SetColor (glm::vec3 pColor)
{
	_color = glm::vec4 (pColor, 1.0f);
}
//Set this light's intensity (All)
void Light::SetIntensity (float pIntensity)
{
	_intensity = pIntensity;
}
//Set this light's angle in radians (Spot)
void Light::SetAngle (float pAngle)
{
	_angle = pAngle;
}
//Set this light's falloff (Point, Spot)
void Light::SetFalloff (glm::vec2 pFalloff)
{
	_falloff = pFalloff;
}


//////////////////////////////|	GETTERS
//Get this light's color
glm::vec3 Light::GetColor ()
{
	return glm::vec3 (_color);//Make a copy so any changes won't affect the private variable
}
//Get this light's total color
glm::vec3 Light::GetTotalColor ()
{
	return _color * _intensity;
}
//Get this light's intensity
float Light::GetIntensity ()
{
	return _intensity;
}
//Get this light's world position (uses cached worldtransform, unlike GameObject's version)
glm::vec4 Light::GetWorldPosition ()
{
	return glm::vec4 (getWorldPosition (), 1);
}
//Get this light's worldspace direction-normal
glm::vec4 Light::GetWorldNormal ()
{
	return glm::normalize (getWorldTransform () * _normal);
}
//Get this light's angle
float Light::GetAngle ()
{
	return _angle;
}
//Get this light's falloff
glm::vec2 Light::GetFalloff ()
{
	return glm::vec2 (_falloff);
}