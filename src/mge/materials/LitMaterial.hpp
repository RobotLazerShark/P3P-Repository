#ifndef LitMaterial_Def
#define LitMaterial_Def


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


class LitMaterial : public AbstractMaterial
{
	public:
		LitMaterial (glm::vec3 pColor, float pShininess = 0, glm::vec3 pSpecularColor = glm::vec3 (1,1,1));
		LitMaterial (std::string pFilename, float pShininess = 0, glm::vec3 pSpecularColor = glm::vec3 (1,1,1));
		virtual ~LitMaterial ();
		static void clearShaderProgram ();
		void SetColor (glm::vec3 pColor);
		void SetTexture (std::string pFilename);
		void SetTextureScale (float pScale);
		void SetShininess (float pShininess);
		void SetSpecularColor (glm::vec3 pColor);
		void SetFade (bool pFade, float pFadeMin = 0, float pFadeMax = 1);
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
		static GLint _fadeLoc;
		static GLint _fadeMinLoc;
		static GLint _fadeMaxLoc;
		static ShaderProgram* _shaderProgram;
		bool _useTexture = false;
		float _shininess = 0;
		bool _fade = false;
		float _fadeMin = 0;
		float _fadeMax = 1;
		glm::vec3 _specularColor = glm::vec3 (1,1,1);
		glm::vec3 _color = glm::vec3 (1,1,1);
		Texture* _texture = nullptr;
		float _texScale = 1;
		static void initializeShader ();
};

#endif