#include "mge/materials/TerrainMaterial.hpp"
#include <mge/core/AbstractGame.hpp>
#include "mge/core/Texture.hpp"
#include <mge/core/Mesh.hpp>
#include <mge/core/TerrainMesh.hpp>


//Initialize static values
ShaderProgram* TerrainMaterial::_shaderProgram = nullptr;
GLint TerrainMaterial::_timeLoc = 0;
GLint TerrainMaterial::_waterIndexLoc = 0;
GLint TerrainMaterial::_waveHeightLoc = 0;
GLint TerrainMaterial::_waveCountLoc = 0;
GLint TerrainMaterial::_splatTexCountLoc = 0;
GLint TerrainMaterial::_splatMapLoc = 0;
GLint TerrainMaterial::_splatTexRLoc = 0;
GLint TerrainMaterial::_splatTexGLoc = 0;
GLint TerrainMaterial::_splatTexBLoc = 0;
GLint TerrainMaterial::_splatTexALoc = 0;
GLint TerrainMaterial::_MVPmatrixLoc = 0;
GLint TerrainMaterial::_modelMatrixLoc = 0;
GLint TerrainMaterial::_vertexLoc = 0;
GLint TerrainMaterial::_normalLoc = 0;
GLint TerrainMaterial::_uvLoc = 0;
GLint TerrainMaterial::_uvScaleRLoc = 0;
GLint TerrainMaterial::_uvScaleGLoc = 0;
GLint TerrainMaterial::_uvScaleBLoc = 0;
GLint TerrainMaterial::_uvScaleALoc = 0;
GLint TerrainMaterial::_litLoc = 0;
GLint TerrainMaterial::_cameraPositionLoc = 0;
GLint TerrainMaterial::_shininessLoc = 0;
GLint TerrainMaterial::_specularColorLoc = 0;
GLint TerrainMaterial::_ambientColorLoc = 0;
GLint TerrainMaterial::_directionalCountLoc = 0;
GLint TerrainMaterial::_directionalColorsLoc = 0;
GLint TerrainMaterial::_directionalNormalsLoc = 0;
GLint TerrainMaterial::_pointCountLoc = 0;
GLint TerrainMaterial::_pointColorsLoc = 0;
GLint TerrainMaterial::_pointPositionsLoc = 0;
GLint TerrainMaterial::_pointFalloffsLoc = 0;
GLint TerrainMaterial::_spotCountLoc = 0;
GLint TerrainMaterial::_spotColorsLoc = 0;
GLint TerrainMaterial::_spotPositionsLoc = 0;
GLint TerrainMaterial::_spotNormalsLoc = 0;
GLint TerrainMaterial::_spotAngledotsLoc = 0;
GLint TerrainMaterial::_spotFalloffsLoc = 0;
GLint TerrainMaterial::_maxHeightLoc = 0;
GLint TerrainMaterial::_pixelOffsetLoc = 0;
GLint TerrainMaterial::_terrainSizeLoc = 0;
GLint TerrainMaterial::_heightMapLoc = 0;

//////////////////////////////|	INSTANCE MANAGEMENT
//Constructor for 2 splattextures
TerrainMaterial::TerrainMaterial (std::string pSplatMapFile, std::string pSplatTexRFile, float pUVScaleR, std::string pSplatTexGFile, float pUVScaleG, int pWaterIndex, float pWaveHeight, float pWaveCount)
{
	std::string failedFiles = "";
	std::string folder = config::MGE_TEXTURE_PATH;
	_waterIndex = pWaterIndex;
	_waveHeight = pWaveHeight;
	_waveCount = pWaveCount;
	_uvScaleR = pUVScaleR;
	_uvScaleG = pUVScaleG;
	_splatTexCount = 2;
	_splatMap = Texture::load (folder + pSplatMapFile);
	failedFiles += (_splatMap == nullptr) ? folder + pSplatMapFile + "\n" : "";
	if (_splatMap == nullptr)
	{
		_splatMap = Texture::load(folder + "Default.png");
	}
	_splatTexR = Texture::load (folder + pSplatTexRFile);
	failedFiles += (_splatTexR == nullptr) ? folder + pSplatTexRFile + "\n" : "";
	if (_splatTexR == nullptr)
	{
		_splatTexR = Texture::load(folder + "Default.png");
	}
	_splatTexG = Texture::load (folder + pSplatTexGFile);
	failedFiles += (_splatTexG == nullptr) ? folder + pSplatTexGFile + "\n" : "";
	if (_splatTexG == nullptr)
	{
		_splatTexG = Texture::load(folder + "Default.png");
	}
	if (failedFiles.length () > 0)
	{
		cout << "There was a problem loading '"<<failedFiles<<"'. Using the default one instread." << endl;
		_splatTexCount = 0;
	}
	if (_shaderProgram == nullptr)
	{
		initializeShader ();
	}
}
//Constructor for 3 splattextures
TerrainMaterial::TerrainMaterial (std::string pSplatMapFile, std::string pSplatTexRFile, float pUVScaleR, std::string pSplatTexGFile, float pUVScaleG, std::string pSplatTexBFile, float pUVScaleB, int pWaterIndex, float pWaveHeight, float pWaveCount)
{
	std::string failedFiles = "";
	std::string folder = config::MGE_TEXTURE_PATH;
	_waterIndex = pWaterIndex;
	_waveHeight = pWaveHeight;
	_waveCount = pWaveCount;
	_uvScaleR = pUVScaleR;
	_uvScaleG = pUVScaleG;
	_uvScaleB = pUVScaleB;
	_splatTexCount = 3;
	_splatMap = Texture::load (folder + pSplatMapFile);
	failedFiles += (_splatMap == nullptr) ? folder + pSplatMapFile + "\n" : "";
	if (_splatMap == nullptr)
	{
		_splatMap = Texture::load(folder + "Default.png");
	}
	_splatTexR = Texture::load (folder + pSplatTexRFile);
	failedFiles += (_splatTexR == nullptr) ? folder + pSplatTexRFile + "\n" : "";
	if (_splatTexR == nullptr)
	{
		_splatTexR = Texture::load(folder + "Default.png");
	}
	_splatTexG = Texture::load (folder + pSplatTexGFile);
	failedFiles += (_splatTexG == nullptr) ? folder + pSplatTexGFile + "\n" : "";
	if (_splatTexG == nullptr)
	{
		_splatTexG = Texture::load(folder + "Default.png");
	}
	_splatTexB = Texture::load (folder + pSplatTexBFile);
	failedFiles += (_splatTexB == nullptr) ? folder + pSplatTexBFile + "\n" : "";
	if (_splatTexB == nullptr)
	{
		_splatTexB = Texture::load(folder + "Default.png");
	}
	if (failedFiles.length () > 0)
	{
		cout << "There was a problem loading:\n"<<failedFiles<<". Using the default one instread." << endl;
		_splatTexCount = 0;
	}
	if (_shaderProgram == nullptr)
	{
		initializeShader ();
	}
}
//Constructor for 4 splattextures
TerrainMaterial::TerrainMaterial (std::string pSplatMapFile, std::string pSplatTexRFile, float pUVScaleR, std::string pSplatTexGFile, float pUVScaleG, std::string pSplatTexBFile, float pUVScaleB, std::string pSplatTexAFile, float pUVScaleA, int pWaterIndex, float pWaveHeight, float pWaveCount)
{
	std::string failedFiles = "";
	std::string folder = config::MGE_TEXTURE_PATH;
	_waterIndex = pWaterIndex;
	_waveHeight = pWaveHeight;
	_waveCount = pWaveCount;
	_uvScaleR = pUVScaleR;
	_uvScaleG = pUVScaleG;
	_uvScaleB = pUVScaleB;
	_uvScaleA = pUVScaleA;
	_splatTexCount = 4;
	_splatMap = Texture::load (folder + pSplatMapFile);
	failedFiles += (_splatMap == nullptr) ? folder + pSplatMapFile + "\n" : "";
	if (_splatMap == nullptr)
	{
		_splatMap = Texture::load(folder + "Default.png");
	}
	_splatTexR = Texture::load (folder + pSplatTexRFile);
	failedFiles += (_splatTexR == nullptr) ? folder + pSplatTexRFile + "\n" : "";
	if (_splatTexR == nullptr)
	{
		_splatTexR = Texture::load(folder + "Default.png");
	}
	_splatTexG = Texture::load (folder + pSplatTexGFile);
	failedFiles += (_splatTexG == nullptr) ? folder + pSplatTexGFile + "\n" : "";
	if (_splatTexG == nullptr)
	{
		_splatTexG = Texture::load(folder + "Default.png");
	}
	_splatTexB = Texture::load (folder + pSplatTexBFile);
	failedFiles += (_splatTexB == nullptr) ? folder + pSplatTexBFile + "\n" : "";
	if (_splatTexB == nullptr)
	{
		_splatTexB = Texture::load(folder + "Default.png");
	}
	_splatTexA = Texture::load (folder + pSplatTexAFile);
	failedFiles += (_splatTexA == nullptr) ? folder + pSplatTexAFile + "\n" : "";
	if (_splatTexA == nullptr)
	{
		_splatTexA = Texture::load(folder + "Default.png");
	}
	if (failedFiles.length () > 0)
	{
		cout << "There was a problem loading:\n"<<failedFiles<<". Using the default one instread." << endl;
		_splatTexCount = 0;
	}
	if (_shaderProgram == nullptr)
	{
		initializeShader ();
	}
}
//Constructor for an unknown amount of splattextures
TerrainMaterial::TerrainMaterial (std::string pSplatMapFile, std::vector <std::string> pTextureFiles, bool pLit)
{
	_lit = pLit;
	std::string failedFiles = "";
	std::string folder = config::MGE_TEXTURE_PATH;
	_splatTexCount = pTextureFiles.size ();
	_splatMap = Texture::load (folder + pSplatMapFile);
	failedFiles += (_splatMap == nullptr) ? folder + pSplatMapFile + "\n" : "";
	if (_splatMap == nullptr)
	{
		_splatMap = Texture::load(folder + "Default.png");
	}

	//Load all provided splattextures
	if (_splatTexCount >= 2)
	{
		_splatTexR = Texture::load (folder + pTextureFiles [0]);
		failedFiles += (_splatTexR == nullptr) ? folder + pTextureFiles [0] + "\n" : "";
		if (_splatTexR == nullptr)
		{
			_splatTexR = Texture::load(folder + "Default.png");
		}
		_splatTexG = Texture::load (folder + pTextureFiles [1]);
		failedFiles += (_splatTexG == nullptr) ? folder + pTextureFiles [1] + "\n" : "";
		if (_splatTexG == nullptr)
		{
			_splatTexG = Texture::load(folder + "Default.png");
		}

		if (_splatTexCount > 2)
		{
			_splatTexB = Texture::load (folder + pTextureFiles [2]);
			failedFiles += (_splatTexB == nullptr) ? folder + pTextureFiles [2] + "\n" : "";
			if (_splatTexB == nullptr)
			{
				_splatTexB = Texture::load(folder + "Default.png");
			}

			if (_splatTexCount > 3)
			{
				_splatTexA = Texture::load (folder + pTextureFiles [3]);
				failedFiles += (_splatTexA == nullptr) ? folder + pTextureFiles [3] + "\n" : "";
				if (_splatTexA == nullptr)
				{
					_splatTexA = Texture::load(folder + "Default.png");
				}
				_splatTexCount = 4;
			}
			else
			{
				_splatTexCount = 3;
			}
		}
		else
		{
			_splatTexCount = 2;
		}
	}
	else
	{
		cout << "Too little textures supplied, filling it out with the default file." << endl;
		_splatTexR = Texture::load(folder + "Default.png");
		_splatTexG = Texture::load(folder + "Default.png");
		_splatTexCount = 2;
	}
}
//Destructor
TerrainMaterial::~TerrainMaterial ()
{
}
void TerrainMaterial::clearShaderProgram()
{
	if (_shaderProgram != nullptr)
	{
		delete _shaderProgram;
		_shaderProgram = nullptr;
	}
}


//////////////////////////////|	SETTERS
void TerrainMaterial::SetLit (bool pLit)
{
	_lit = pLit;
}
void TerrainMaterial::SetShininess (float pShininess)
{
	_shininess = pShininess;
}
void TerrainMaterial::SetSpecularColor (glm::vec3 pSpecularColor)
{
	_specularColor = pSpecularColor;
}
void TerrainMaterial::SetSplatMap (std::string pFilename)
{
	string filePath = config::MGE_TEXTURE_PATH + pFilename;
	Texture* tempTex = Texture::load (filePath);
	if (tempTex == nullptr)
	{
		cout << "Failed to load '" << filePath << "'. The splatmap will not be replaced." << endl;
	}
	else
	{
		_splatMap = tempTex;
	}
}
void TerrainMaterial::SetSplatTexR (std::string pFilename)
{
	string filePath = config::MGE_TEXTURE_PATH + pFilename;
	Texture* tempTex = Texture::load (filePath);
	if (tempTex == nullptr)
	{
		cout << "Failed to load '" << filePath << "'. The R-texture will not be replaced." << endl;
	}
	else
	{
		_splatTexR = tempTex;
	}
}
void TerrainMaterial::SetSplatTexG (std::string pFilename)
{
	string filePath = config::MGE_TEXTURE_PATH + pFilename;
	Texture* tempTex = Texture::load (filePath);
	if (tempTex == nullptr)
	{
		cout << "Failed to load '" << filePath << "'. The G-texture will not be replaced." << endl;
	}
	else
	{
		_splatTexG = tempTex;
	}
}
void TerrainMaterial::SetSplatTexB (std::string pFilename)
{
	string filePath = config::MGE_TEXTURE_PATH + pFilename;
	Texture* tempTex = Texture::load (filePath);
	if (tempTex == nullptr)
	{
		cout << "Failed to load '" << filePath << "'. The B-texture will not be replaced." << endl;
	}
	else
	{
		_splatTexB = tempTex;
	}
}
void TerrainMaterial::SetSplatTexA (std::string pFilename)
{
	string filePath = config::MGE_TEXTURE_PATH + pFilename;
	Texture* tempTex = Texture::load (filePath);
	if (tempTex == nullptr)
	{
		cout << "Failed to load '" << filePath << "'. The A-texture will not be replaced." << endl;
	}
	else
	{
		_splatTexA = tempTex;
	}
}
void TerrainMaterial::SetUVScales (float pUVScaleR, float pUVScaleG, float pUVScaleB, float pUVScaleA)
{
	_uvScaleR = pUVScaleR;
	_uvScaleG = pUVScaleG;
	_uvScaleB = pUVScaleB;
	_uvScaleA = pUVScaleA;
}
void TerrainMaterial::SetTexRScale (float pUVScaleR)
{
	_uvScaleR = pUVScaleR;
}
void TerrainMaterial::SetTexGScale (float pUVScaleG)
{
	_uvScaleG = pUVScaleG;
}
void TerrainMaterial::SetTexBScale (float pUVScaleB)
{
	_uvScaleB = pUVScaleB;
}
void TerrainMaterial::SetTexAScale (float pUVScaleA)
{
	_uvScaleA = pUVScaleA;
}
void TerrainMaterial::SetWaterIndex (int pWaterIndex)
{
	//1 for splatTexR, 2 for splatTexB, 3 for splatTexG, 4 for splatTexA.
	//Any other number will result in no water simulation.
	_waterIndex = pWaterIndex;
}
void TerrainMaterial::SetWaveHeight (float pWaveHeight)
{
	_waveHeight = pWaveHeight;
}
void TerrainMaterial::SetWaveCount (float pWaveCount)
{
	_waveCount = pWaveCount;
}


//////////////////////////////|	RENDERING
void TerrainMaterial::initializeShader ()
{
	_shaderProgram = new ShaderProgram ();
	_shaderProgram->addShader (GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "terrain.vs");
	_shaderProgram->addShader (GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "terrain.fs");
	_shaderProgram->finalize ();

	//Store uniform and attribute indices
	_timeLoc = _shaderProgram->getUniformLocation ("time");
	_waterIndexLoc = _shaderProgram->getUniformLocation ("waterIndex");
	_waveHeightLoc = _shaderProgram->getUniformLocation ("waveHeight");
	_waveCountLoc = _shaderProgram->getUniformLocation ("waveCount");
	_splatTexCountLoc = _shaderProgram->getUniformLocation ("splatTexCount");
	_splatMapLoc = _shaderProgram->getUniformLocation ("splatMap");
	_splatTexRLoc = _shaderProgram->getUniformLocation ("splatTexR");
	_splatTexGLoc = _shaderProgram->getUniformLocation ("splatTexG");
	_splatTexBLoc = _shaderProgram->getUniformLocation ("splatTexB");
	_splatTexALoc = _shaderProgram->getUniformLocation ("splatTexA");
	_MVPmatrixLoc = _shaderProgram->getUniformLocation ("MVPmatrix");
	_modelMatrixLoc = _shaderProgram->getUniformLocation ("modelMatrix");
	_vertexLoc = _shaderProgram->getAttribLocation ("vertex");
	_normalLoc = _shaderProgram->getAttribLocation ("normal");
	_uvLoc = _shaderProgram->getAttribLocation ("uv");
	_uvScaleRLoc = _shaderProgram->getUniformLocation ("uvScaleR");
	_uvScaleGLoc = _shaderProgram->getUniformLocation ("uvScaleG");
	_uvScaleBLoc = _shaderProgram->getUniformLocation ("uvScaleB");
	_uvScaleALoc = _shaderProgram->getUniformLocation ("uvScaleA");
	_litLoc = _shaderProgram->getUniformLocation ("lit");
	_cameraPositionLoc = _shaderProgram->getUniformLocation ("cameraPosition");
	_shininessLoc = _shaderProgram->getUniformLocation ("shininess");
	_specularColorLoc = _shaderProgram->getUniformLocation ("specularColor");
	_ambientColorLoc = _shaderProgram->getUniformLocation ("ambientColor");
	_directionalCountLoc = _shaderProgram->getUniformLocation ("directionalCount");
	_directionalColorsLoc = _shaderProgram->getUniformLocation ("directionalColors");
	_directionalNormalsLoc = _shaderProgram->getUniformLocation ("directionalNormals");
	_pointCountLoc = _shaderProgram->getUniformLocation ("pointCount");
	_pointColorsLoc = _shaderProgram->getUniformLocation ("pointColors");
	_pointPositionsLoc = _shaderProgram->getUniformLocation ("pointPositions");
	_pointFalloffsLoc = _shaderProgram->getUniformLocation ("pointFalloffs");
	_spotCountLoc = _shaderProgram->getUniformLocation ("spotCount");
	_spotColorsLoc = _shaderProgram->getUniformLocation ("spotColors");
	_spotPositionsLoc = _shaderProgram->getUniformLocation ("spotPositions");
	_spotNormalsLoc = _shaderProgram->getUniformLocation ("spotNormals");
	_spotAngledotsLoc = _shaderProgram->getUniformLocation ("spotAngledots");
	_spotFalloffsLoc = _shaderProgram->getUniformLocation ("spotFalloffs");
	_maxHeightLoc = _shaderProgram->getUniformLocation ("maxHeight");
	_pixelOffsetLoc = _shaderProgram->getUniformLocation ("pixelOffset");
	_terrainSizeLoc = _shaderProgram->getUniformLocation ("terrainSize");
	_heightMapLoc = _shaderProgram->getUniformLocation ("heightMap");
}


void TerrainMaterial::render (Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix)
{
	_shaderProgram->use ();

	//Send textures to the shader
	if (_splatTexCount >= 2)
	{
		glActiveTexture (GL_TEXTURE0);
		glBindTexture (GL_TEXTURE_2D, _splatMap->getId ());
		glUniform1i (_splatMapLoc, 0);
		glActiveTexture (GL_TEXTURE1);
		glBindTexture (GL_TEXTURE_2D, _splatTexR->getId ());
		glUniform1i (_splatTexRLoc, 1);
		glActiveTexture (GL_TEXTURE2);
		glBindTexture (GL_TEXTURE_2D, _splatTexG->getId ());
		glUniform1i (_splatTexGLoc, 2);
		if (_splatTexCount >= 3)
		{
			glActiveTexture (GL_TEXTURE3);
			glBindTexture (GL_TEXTURE_2D, _splatTexB->getId ());
			glUniform1i (_splatTexBLoc, 3);
			if (_splatTexCount >= 4)
			{
				glActiveTexture (GL_TEXTURE4);
				glBindTexture (GL_TEXTURE_2D, _splatTexA->getId ());
				glUniform1i (_splatTexALoc, 4);
			}
		}
	}

	//Send uniform data to shaderprogram
	glUniform1f (_timeLoc, AbstractGame::Time ());
	glUniform1i (_waterIndexLoc, _waterIndex);
	glUniform1f (_waveHeightLoc, _waveHeight);
	glUniform1f (_waveCountLoc, _waveCount);
	glUniform1i (_splatTexCountLoc, _splatTexCount);
	glUniform1f (_uvScaleRLoc, _uvScaleR);
	glUniform1f (_uvScaleGLoc, _uvScaleG);
	glUniform1f (_uvScaleBLoc, _uvScaleB);
	glUniform1f (_uvScaleALoc, _uvScaleA);
	glm::mat4 MVPmatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv (_MVPmatrixLoc, 1, GL_FALSE, glm::value_ptr (MVPmatrix));
	glUniform1i (_litLoc, _lit);
	if (_lit)
	{
		glUniformMatrix4fv (_modelMatrixLoc, 1, GL_FALSE, glm::value_ptr (pModelMatrix));
		glUniform1f (_shininessLoc, _shininess);
		glUniform3fv (_specularColorLoc, 1, glm::value_ptr (_specularColor));
		glUniform4fv (_cameraPositionLoc, 1, glm::value_ptr (ShaderDataUtil::GetCameraWorldPosition ()));
		glUniform3fv (_ambientColorLoc, 1, glm::value_ptr (ShaderDataUtil::GetAmbientColor ()));
		int dirCount = ShaderDataUtil::GetDirectionalLightCount ();
		int pointCount = ShaderDataUtil::GetPointLightCount ();
		int spotCount = ShaderDataUtil::GetSpotLightCount ();
		if (dirCount > 0)//Send directional light data (if there are any directional lights)
		{
			glUniform3fv (_directionalColorsLoc, dirCount, glm::value_ptr (ShaderDataUtil::GetDirectionalLightColors () [0]));
			glUniform4fv (_directionalNormalsLoc, dirCount, glm::value_ptr (ShaderDataUtil::GetDirectionalLightNormals () [0]));
		}
		else
		{
			glUniform3fv (_directionalColorsLoc, 0, nullptr);
			glUniform4fv (_directionalNormalsLoc, 0, nullptr);
		}
		glUniform1i (_directionalCountLoc, dirCount);
		if (pointCount > 0)//Send point light data (if there are any point lights)
		{
			glUniform3fv (_pointColorsLoc, pointCount, glm::value_ptr (ShaderDataUtil::GetPointLightColors () [0]));
			glUniform4fv (_pointPositionsLoc, pointCount, glm::value_ptr (ShaderDataUtil::GetPointLightPositions () [0]));
			glUniform2fv (_pointFalloffsLoc, pointCount, glm::value_ptr (ShaderDataUtil::GetPointLightFalloffs () [0]));
		}
		else
		{
			glUniform3fv (_pointColorsLoc, 0, nullptr);
			glUniform4fv (_pointPositionsLoc, 0, nullptr);
			glUniform2fv (_pointFalloffsLoc, 0, nullptr);
		}
		glUniform1i (_pointCountLoc, pointCount);
		if (spotCount > 0)//Send spot light data (if there are any spot lights)
		{
			glUniform3fv (_spotColorsLoc, spotCount, glm::value_ptr (ShaderDataUtil::GetSpotLightColors () [0]));
			glUniform4fv (_spotPositionsLoc, spotCount, glm::value_ptr (ShaderDataUtil::GetSpotLightPositions () [0]));
			glUniform4fv (_spotNormalsLoc, spotCount, glm::value_ptr (ShaderDataUtil::GetSpotLightNormals () [0]));
			glUniform1fv (_spotAngledotsLoc, spotCount, &ShaderDataUtil::GetSpotLightAngledots () [0]);
			glUniform2fv (_spotFalloffsLoc, spotCount, glm::value_ptr (ShaderDataUtil::GetSpotLightFalloffs () [0]));
		}
		else
		{
			glUniform3fv (_spotColorsLoc, 0, nullptr);
			glUniform4fv (_spotPositionsLoc, 0, nullptr);
			glUniform4fv (_spotNormalsLoc, 0, nullptr);
			glUniform1fv (_spotAngledotsLoc, 0, nullptr);
			glUniform2fv (_spotFalloffsLoc, 0, nullptr);
		}
		glUniform1i (_spotCountLoc, spotCount);
	}
	glUniform1i (_terrainSizeLoc, TerrainMesh::SIZE);
	TerrainMesh* terrain = dynamic_cast <TerrainMesh*> (pMesh);
	if (terrain != nullptr)
	{
		glUniform1f (_maxHeightLoc, terrain->maxHeight ());
		glUniform2fv (_pixelOffsetLoc, 1, glm::value_ptr (terrain->pixelOffset ()));
		glActiveTexture (GL_TEXTURE5);
		glBindTexture (GL_TEXTURE_2D, terrain->heightMapId ());
		glUniform1i (_heightMapLoc, 5);
	}
	else
	{
		//A maxHeight value of 0 will tell the shader the heightmap textureslot is empty.
		glUniform1f (_maxHeightLoc, 0);
	}

	//Send basic vertex data to shader
	pMesh->streamToOpenGL (_vertexLoc, _normalLoc, _uvLoc);

	glGetError ();//Read errors from openGL, if there are any
}