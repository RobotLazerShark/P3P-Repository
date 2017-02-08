#ifndef WobbleMaterial_Def
#define WobbleMaterial_Def


//Include files
#include <glm.hpp>
#include <string>
#include <mge/config.hpp>
#include <mge/core/Mesh.hpp>
#include <mge/core/Texture.hpp>
#include <mge/core/GameObject.hpp>
#include <mge/core/AbstractGame.hpp>
#include <mge/core/ShaderProgram.hpp>
#include <mge/materials/AbstractMaterial.hpp>


class WobbleMaterial : public AbstractMaterial
{
	public:
		WobbleMaterial (string pFilename);
		virtual ~WobbleMaterial ();
		void render (Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;
	private:
		std::string DEFAULT_TEXTURE = "Default.png";
		static GLint _MVPmatrixLoc;
		static GLint _timeLoc;
		static GLint _useNormalMapLoc;
		static GLint _vertexLoc;
		static GLint _normalLoc;
		static GLint _uvLoc;
		static ShaderProgram* _shaderProgram;
		Texture* _texture;
		static void initializeShader ();
};

#endif