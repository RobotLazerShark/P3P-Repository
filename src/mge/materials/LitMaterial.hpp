#ifndef LitMaterial_Def
#define LitMaterial_Def


//Include files
#include <glm.hpp>
#include <string>
#include <mge/config.hpp>
#include <mge/core/Mesh.hpp>
#include <mge/core/Texture.hpp>
#include <mge/core/GameObject.hpp>
#include <mge/core/AbstractGame.hpp>
#include <mge/core/ShaderProgram.hpp>
#include <mge/util/ShaderDataUtil.hpp>
#include <mge/materials/AbstractMaterial.hpp>


class LitMaterial : public AbstractMaterial
{
	public:
		LitMaterial (glm::vec3 pColor, float pShininess = 0, glm::vec3 pSpecularColor = glm::vec3 (1,1,1));
		LitMaterial (std::string pFilename, float pShininess = 0, glm::vec3 pSpecularColor = glm::vec3 (1,1,1));
		virtual ~LitMaterial ();
		void SetColor (glm::vec3 pColor);
		void SetTexture (std::string pFilename);
		void SetTextureScale (float pScale);
		void SetShininess (float pShininess);
		void SetSpecularColor (glm::vec3 pColor);
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
		static ShaderProgram* _shaderProgram;
		bool _useTexture;
		float _shininess;
		glm::vec3 _specularColor;
		glm::vec3 _color;
		Texture* _texture;
		float _texScale = 1;
		Texture* _normalMap;
		static void initializeShader ();
};

#endif