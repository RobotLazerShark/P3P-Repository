#ifndef Light_Def
#define Light_Def

//Include files
#include <glm.hpp>
#include <mge/config.hpp>
#include <mge/core/GameObject.hpp>

class ColorMaterial;


class Light : public GameObject
{
	private:
		glm::vec3 _color;
		float _intensity;
		glm::vec4 _normal;
		float _angle;
		glm::vec2 _falloff;//x = linear, y = exponential factor. total = 1.
	public:
		//Properties
		enum Shapes { Directional, Point, Spot };
		const Shapes shape;
		//Functions
		Light (glm::vec3 pColor, float pIntensity);//Directional
		Light (glm::vec3 pColor, float pIntensity, glm::vec2 pFalloff);//Point
		Light (glm::vec3 pColor, float pIntensity, glm::vec3 pPosition, glm::vec2 pFalloff);//Point
		Light (glm::vec3 pColor, float pIntensity, float pAngle, glm::vec2 pFalloff);//Spot
		Light (glm::vec3 pColor, float pIntensity, glm::vec3 pPosition, float pAngle, glm::vec2 pFalloff);//Spot
		~Light ();
		virtual void setParent (GameObject* pParent) override;
		//Setting data (for dynamically changing properties)
		void SetColor (glm::vec3 pColor);
		void SetIntensity (float pIntensity);
		void SetAngle (float pAngle);
		void SetFalloff (glm::vec2 pFalloff);
		//Retrieving data (for shaderutil mostly)
		glm::vec3 GetColor ();
		float GetIntensity ();
		glm::vec3 GetTotalColor ();
		glm::vec4 GetWorldPosition ();
		glm::vec4 GetWorldNormal ();
		float GetAngle ();
		glm::vec2 GetFalloff ();
};

#endif