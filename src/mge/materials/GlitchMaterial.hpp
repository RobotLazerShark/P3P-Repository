#ifndef GlitchMaterial_Def
#define GlitchMaterial_Def


//Include files
#include <glm.hpp>
#include <string>
#include <GL/glew.h>
#include <mge/config.hpp>
#include <mge/core/ShaderProgram.hpp>
#include <mge/util/ShaderDataUtil.hpp>
#include <mge/materials/AbstractMaterial.hpp>

class Texture;
class Mesh;


class GlitchMaterial : public AbstractMaterial
{
	public:
		GlitchMaterial(glm::vec3 pColor, float pShininess = 0, glm::vec3 pSpecularColor = glm::vec3 (1,1,1));
		GlitchMaterial(std::string pFilename, float pShininess = 0, glm::vec3 pSpecularColor = glm::vec3 (1,1,1));
		virtual ~GlitchMaterial();
		static void clearShaderProgram ();
		void SetColor (glm::vec3 pColor);
		void SetTexture (std::string pFilename);
		void SetTextureScale (float pScale);
		void SetShininess (float pShininess);
		void SetSpecularColor (glm::vec3 pColor);
		void SetGlitchIntensity (float pIntensity);
		void render (Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;
	private:
		static GLint _MVPmatrixLoc;
		static GLint _textureLoc;
		static GLint _texScaleLoc;
		static GLint _modelMatrixLoc;
		static GLint _useTextureLoc;
		static GLint _colorLoc;
		static GLint _cameraPositionLoc;
		static GLint _shininessLoc;
		static GLint _specularColorLoc;
		static GLint _vertexLoc;
		static GLint _normalLoc;
		static GLint _uvLoc;
		static GLint _ambientColorLoc;
		static GLint _directionalCountLoc;
		static GLint _directionalColorsLoc;
		static GLint _directionalNormalsLoc;
		static GLint _pointCountLoc;
		static GLint _pointColorsLoc;
		static GLint _pointPositionsLoc;
		static GLint _pointFalloffsLoc;
		static GLint _spotCountLoc;
		static GLint _spotColorsLoc;
		static GLint _spotPositionsLoc;
		static GLint _spotNormalsLoc;
		static GLint _spotAngledotsLoc;
		static GLint _spotFalloffsLoc;
		static GLint _timeLoc;
		static GLint _random1Loc;
		static GLint _random2Loc;
		static GLint _glitchIntensityLoc;
		static ShaderProgram* _shaderProgram;
		bool _useTexture;
		float _shininess;
		glm::vec3 _specularColor;
		glm::vec3 _color;
		Texture* _texture = nullptr;
		float _texScale = 1;
		float _glitchIntensity = 1;
		int _random1Array [15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		float _random2Array [15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		static void initializeShader ();
};

#endif