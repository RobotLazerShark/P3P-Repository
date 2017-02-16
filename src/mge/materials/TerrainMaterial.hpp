#ifndef TerrainMaterial_Def
#define TerrainMaterial_Def


//Include files
#include <glm.hpp>
#include <vector>
#include <GL/glew.h>
#include <mge/config.hpp>
#include "mge/core/ShaderProgram.hpp"
#include "mge/materials/AbstractMaterial.hpp"

class Mesh;
class Texture;


class TerrainMaterial : public AbstractMaterial
{

	public:
		TerrainMaterial (std::string pSplatMapFile, std::string pSplatTexRFile, float pUVScaleR, std::string pSplatTexGFile, float pUVScaleG, int pWaterIndex = -1, float pWaveHeight = 0.08f, float pWaveCount = 1);
		TerrainMaterial (std::string pSplatMapFile, std::string pSplatTexRFile, float pUVScaleR, std::string pSplatTexGFile, float pUVScaleG, std::string pSplatTexBFile, float pUVScaleB, int pWaterIndex = -1, float pWaveHeight = 0.08f, float pWaveCount = 1);
		TerrainMaterial (std::string pSplatMapFile, std::string pSplatTexRFile, float pUVScaleR, std::string pSplatTexGFile, float pUVScaleG, std::string pSplatTexBFile, float pUVScaleB, std::string pSplatTexAFile, float pUVScaleA, int pWaterIndex = -1, float pWaveHeight = 0.08f, float pWaveCount = 1);
		TerrainMaterial (std::string pSplatMapFile, std::vector <std::string> pTextureFiles, bool pLit);
		virtual ~TerrainMaterial ();
		static void clearShaderProgram ();
		void SetLit (bool pLit);
		void SetShininess (float pShininess = 0);
		void SetSpecularColor (glm::vec3 pSpecularColor = glm::vec3 (1, 1, 1));
		void SetSplatMap (std::string pFilename);
		void SetSplatTexR (std::string pFilename);
		void SetSplatTexG (std::string pFilename);
		void SetSplatTexB (std::string pFilename);
		void SetSplatTexA (std::string pFilename);
		void SetUVScales (float pUVScaleR, float pUVScaleG, float pUVScaleB, float pUVScaleA);
		void SetTexRScale (float pUVScaleR);
		void SetTexGScale (float pUVScaleG);
		void SetTexBScale (float pUVScaleB);
		void SetTexAScale (float pUVScaleA);
		void SetWaterIndex (int pWaterIndex);
		void SetWaveHeight (float pWaveHeight);
		void SetWaveCount (float pWaveCount);
		virtual void render (Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;
	private:
		static ShaderProgram* _shaderProgram;
		static void initializeShader ();
		static GLint _timeLoc;
		static GLint _waterIndexLoc;
		static GLint _waveHeightLoc;
		static GLint _waveCountLoc;
		static GLint _splatTexCountLoc;
		static GLint _splatMapLoc;
		static GLint _splatTexRLoc;
		static GLint _splatTexGLoc;
		static GLint _splatTexBLoc;
		static GLint _splatTexALoc;
		static GLint _MVPmatrixLoc;
		static GLint _modelMatrixLoc;
		static GLint _vertexLoc;
		static GLint _normalLoc;
		static GLint _uvLoc;
		static GLint _uvScaleRLoc;
		static GLint _uvScaleGLoc;
		static GLint _uvScaleBLoc;
		static GLint _uvScaleALoc;
		static GLint _litLoc;
		static GLint _cameraPositionLoc;
		static GLint _shininessLoc;
		static GLint _specularColorLoc;
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
		static GLint _maxHeightLoc;
		static GLint _pixelOffsetLoc;
		static GLint _terrainSizeLoc;
		static GLint _heightMapLoc;
		int _waterIndex;
		float _waveHeight;
		float _waveCount;
		float _uvScaleR = 1;
		float _uvScaleG = 1;
		float _uvScaleB = 1;
		float _uvScaleA = 1;
		bool _lit = false;
		float _shininess = 0;
		glm::vec3 _specularColor = glm::vec3 (1, 1, 1);
		int _splatTexCount;
		Texture* _splatMap = nullptr;
		Texture* _splatTexR = nullptr;
		Texture* _splatTexG = nullptr;
		Texture* _splatTexB = nullptr;
		Texture* _splatTexA = nullptr;
		TerrainMaterial (const TerrainMaterial&);
		TerrainMaterial& operator= (const TerrainMaterial&);

};

#endif