#ifndef ShaderDataUtil_Def
#define ShaderDataUtil_Def


//Include files
#include <glm.hpp>
#include <vector>

class Camera;
class Light;


class ShaderDataUtil
{
	public:
		//Setting data
		static void SetAmbientLight (glm::vec3 pColor, float pIntensity);
		static void AddLight (Light* pLight);
		static void RemoveLight (Light* pLight);
		static void SetCamera (Camera* pCamera);
		static void UpdateCameraInfo ();
		//Retrieving data
		static glm::vec4 GetCameraWorldPosition ();
		static int GetDirectionalLightCount ();
		static int GetPointLightCount ();
		static int GetSpotLightCount ();
		static glm::vec3 GetAmbientColor ();
		static std::vector<glm::vec3> GetDirectionalLightColors ();
		static std::vector<glm::vec4> GetDirectionalLightNormals ();
		static std::vector<glm::vec3> GetPointLightColors ();
		static std::vector<glm::vec4> GetPointLightPositions ();
		static std::vector<glm::vec2> GetPointLightFalloffs ();
		static std::vector<glm::vec3> GetSpotLightColors ();
		static std::vector<glm::vec4> GetSpotLightPositions ();
		static std::vector<glm::vec4> GetSpotLightNormals ();
		static std::vector<float> GetSpotLightAngledots ();
		static std::vector<glm::vec2> GetSpotLightFalloffs ();
};

#endif