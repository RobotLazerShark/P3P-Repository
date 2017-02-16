#include <mge/util/ShaderDataUtil.hpp>
#include <mge/objects/Camera.hpp>
#include <mge/objects/Light.hpp>


//Static fields
static Camera* _camera = nullptr;
static glm::vec4 _cameraWorldPosition;
static glm::vec3 _ambientColor;						//Light class properties	->	Shader properties
static std::vector <Light*> _directionalLights;//color, intensity, direction			->	color, normal
static std::vector <Light*> _pointLights;//color, intensity, position, falloff			->	color, position, falloff
static std::vector <Light*> _spotLights;//color, intensity, position, direction, angle, falloff	->	color, position, normal, angledot, falloff


//////////////////////////////|	MODIFYING DATA
//Set the ambient light (there can only be one)
void ShaderDataUtil::SetAmbientLight (glm::vec3 pColor, float pIntensity)
{
	_ambientColor = pColor * pIntensity;
}
//Add a light of any type, so that their data can be extracted when requested by materials/shaders, while still allowing their properties to change
void ShaderDataUtil::AddLight (Light* pLight)
{
	switch (pLight->shape)//The type-based sorting is done here so that the correct data-retrieval functions are called when information is requested
	{
		case Light::Shapes::Directional:
			if (_directionalLights.size () == 50) return;
			_directionalLights.push_back (pLight);
			break;
		case Light::Shapes::Point:
			if (_pointLights.size () == 50) return;
			_pointLights.push_back (pLight);
			break;
		case Light::Shapes::Spot:
			if (_spotLights.size () == 50) return;
			_spotLights.push_back (pLight);
			break;
		default:
			break;
	}
}
//Remove a light of any type
void ShaderDataUtil::RemoveLight (Light* pLight)
{
	switch (pLight->shape)
	{
		case Light::Shapes::Directional:
			for (int i = 0, size = _directionalLights.size (); i < size; i++)
			{
				if (_directionalLights [i] == pLight)
				{
					_directionalLights.erase (_directionalLights.begin () + i);
					return;
				}
			}
			break;
		case Light::Shapes::Point:
			for (int i = 0, size = _pointLights.size (); i < size; i++)
			{
				if (_pointLights [i] == pLight)
				{
					_pointLights.erase (_pointLights.begin () + i);
					return;
				}
			}
			break;
		case Light::Shapes::Spot:
			for (int i = 0, size = _spotLights.size (); i < size; i++)
			{
				if (_spotLights [i] == pLight)
				{
					_spotLights.erase (_spotLights.begin () + i);
					return;
				}
			}
			break;
		default:
			break;
	}
}
//Set the camera that will be used for rendering (there can only be one)
void ShaderDataUtil::SetCamera (Camera* pCamera)
{
	_camera = pCamera;
}
//Update the camera information (this function is called once each frame, so we calculate this data as little as possible
void ShaderDataUtil::UpdateCameraInfo ()
{
	_cameraWorldPosition = glm::vec4 (_camera->getWorldPosition (), 1);
}


//////////////////////////////|	RETRIEVING DATA
////////////////////|	CAMERA
//Get the camera's world position
glm::vec4 ShaderDataUtil::GetCameraWorldPosition ()
{
	return _cameraWorldPosition;
}
////////////////////|	AMBIENT
//Get ambient light color
glm::vec3 ShaderDataUtil::GetAmbientColor ()
{
	return _ambientColor;
}
////////////////////|	DIRECTIONAL
//Get the amount of directional lights in the world
int ShaderDataUtil::GetDirectionalLightCount ()
{
	return _directionalLights.size ();
}
//Get directional light colors
std::vector<glm::vec3> ShaderDataUtil::GetDirectionalLightColors ()
{
	std::vector <glm::vec3> colorVec;
	for (int i = 0, size = _directionalLights.size (); i < size; i ++)
	{
		colorVec.push_back (_directionalLights [i]->GetTotalColor ());
	}
	return colorVec;
}
//Get directional light normals
std::vector<glm::vec4> ShaderDataUtil::GetDirectionalLightNormals ()
{
	std::vector <glm::vec4> normalVec;
	for (int i = 0, size = _directionalLights.size (); i < size; i ++)
	{
		normalVec.push_back (_directionalLights [i]->GetWorldNormal ());
	}
	return normalVec;
}
////////////////////|	POINT
//Get the amount of point lights in the world
int ShaderDataUtil::GetPointLightCount ()
{
	return _pointLights.size ();
}
//Get point light colors
std::vector<glm::vec3> ShaderDataUtil::GetPointLightColors ()
{
	std::vector <glm::vec3> colorVec;
	for (int i = 0, size = _pointLights.size (); i < size; i ++)
	{
		colorVec.push_back (_pointLights [i]->GetTotalColor ());
	}
	return colorVec;
}
//Get point light positions
std::vector<glm::vec4> ShaderDataUtil::GetPointLightPositions ()
{
	std::vector <glm::vec4> posVec;
	for (int i = 0, size = _pointLights.size (); i < size; i ++)
	{
		posVec.push_back (_pointLights [i]->GetWorldPosition ());
	}
	return posVec;
}
//Get point light falloffs
std::vector <glm::vec2> ShaderDataUtil::GetPointLightFalloffs ()
{
	std::vector <glm::vec2> falloffVec;
	for (int i = 0, size = _pointLights.size (); i < size; i ++)
	{
		falloffVec.push_back (_pointLights [i]->GetFalloff ());
	}
	return falloffVec;
}
////////////////////|	SPOT
//Get the amount of spot lights in the world
int ShaderDataUtil::GetSpotLightCount ()
{
	return _spotLights.size ();
}
//Get spot light colors
std::vector<glm::vec3> ShaderDataUtil::GetSpotLightColors ()
{
	std::vector <glm::vec3> colorVec;
	for (int i = 0, size = _spotLights.size (); i < size; i ++)
	{
		colorVec.push_back (_spotLights [i]->GetTotalColor ());
	}
	return colorVec;
}
//Get spot light positions
std::vector<glm::vec4> ShaderDataUtil::GetSpotLightPositions ()
{
	std::vector <glm::vec4> posVec;
	for (int i = 0, size = _spotLights.size (); i < size; i ++)
	{
		posVec.push_back (_spotLights [i]->GetWorldPosition ());
	}
	return posVec;
}
//Get spot light normals
std::vector<glm::vec4> ShaderDataUtil::GetSpotLightNormals ()
{
	std::vector <glm::vec4> normalVec;
	for (int i = 0, size = _spotLights.size (); i < size; i ++)
	{
		normalVec.push_back (_spotLights [i]->GetWorldNormal ());
	}
	return normalVec;
}
//Get spot light angledot (the dot product of the angle between the spotlight's normal, and any ray at the falloff angle)
std::vector<float> ShaderDataUtil::GetSpotLightAngledots ()
{
	std::vector <float> dotVec;
	for (int i = 0, size = _spotLights.size (); i < size; i ++)
	{
		dotVec.push_back (cos (_spotLights [i]->GetAngle () * 0.5f));
	}
	return dotVec;
}
//Get spot light falloffs
std::vector<glm::vec2> ShaderDataUtil::GetSpotLightFalloffs ()
{
	std::vector <glm::vec2> falloffVec;
	for (int i = 0, size = _spotLights.size (); i < size; i ++)
	{
		falloffVec.push_back (_spotLights [i]->GetFalloff ());
	}
	return falloffVec;
}