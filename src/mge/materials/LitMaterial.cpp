#include "mge/materials/LitMaterial.hpp"


//Initialize static values
GLint LitMaterial::_MVPmatrixLoc = 0;
GLint LitMaterial::_textureLoc = 0;
GLint LitMaterial::_texScaleLoc = 0;
GLint LitMaterial::_vertexLoc = 0;
GLint LitMaterial::_normalLoc = 0;
GLint LitMaterial::_uvLoc = 0;
GLint LitMaterial::_ambientColorLoc = 0;
GLint LitMaterial::_modelMatrixLoc = 0;
GLint LitMaterial::_useTextureLoc = 0;
GLint LitMaterial::_colorLoc = 0;
GLint LitMaterial::_shininessLoc = 0;
GLint LitMaterial::_specularColorLoc = 0;
GLint LitMaterial::_cameraPositionLoc = 0;
GLint LitMaterial::_directionalCountLoc = 0;
GLint LitMaterial::_directionalColorsLoc = 0;
GLint LitMaterial::_directionalNormalsLoc = 0;
GLint LitMaterial::_pointCountLoc = 0;
GLint LitMaterial::_pointColorsLoc = 0;
GLint LitMaterial::_pointPositionsLoc = 0;
GLint LitMaterial::_pointFalloffsLoc = 0;
GLint LitMaterial::_spotCountLoc = 0;
GLint LitMaterial::_spotColorsLoc = 0;
GLint LitMaterial::_spotPositionsLoc = 0;
GLint LitMaterial::_spotNormalsLoc = 0;
GLint LitMaterial::_spotAngledotsLoc = 0;
GLint LitMaterial::_spotFalloffsLoc = 0;
ShaderProgram* LitMaterial::_shaderProgram = nullptr;


//////////////////////////////|	INSTANCE MANAGEMENT
//Constructor for when there is only a color
LitMaterial::LitMaterial (glm::vec3 pColor, float pShininess, glm::vec3 pSpecularColor)
{
	_shininess = pShininess;
	_specularColor = pSpecularColor;
	_color = pColor;
	_useTexture = false;
	if (_shaderProgram == nullptr)//If the shader is not already loaded, load it.
	{
		initializeShader ();
	}
}
//Constructor for when there is only a texture
LitMaterial::LitMaterial (std::string pFilename, float pShininess, glm::vec3 pSpecularColor)
{
	_shininess = pShininess;
	_specularColor = pSpecularColor;
	_texture = Texture::load (config::MGE_TEXTURE_PATH + pFilename);
	if (_texture == nullptr)
	{
		cout << "There was a problem loading the texture. Using the default one instread." << endl;
		_texture = Texture::load (config::MGE_TEXTURE_PATH + "White.png");
	}
	if (_shaderProgram == nullptr)//If the shader is not already loaded, load it.
	{
		initializeShader ();
	}
}
//Destructor
LitMaterial::~LitMaterial ()
{
	delete _texture;
}
//Set up the shader
void LitMaterial::initializeShader ()
{
	_shaderProgram = new ShaderProgram ();
	_shaderProgram->addShader (GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "lit.vs");
	_shaderProgram->addShader (GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "lit.fs");
	_shaderProgram->finalize ();

	//Store uniform and attribute indices
	_MVPmatrixLoc = _shaderProgram->getUniformLocation ("MVPmatrix");
	_textureLoc = _shaderProgram->getUniformLocation ("texture");
	_texScaleLoc = _shaderProgram->getUniformLocation ("texScale");
	_modelMatrixLoc = _shaderProgram->getUniformLocation ("modelMatrix");
	_useTextureLoc = _shaderProgram->getUniformLocation ("useTexture");
	_colorLoc = _shaderProgram->getUniformLocation ("color");
	_shininessLoc = _shaderProgram->getUniformLocation ("shininess");
	_specularColorLoc = _shaderProgram->getUniformLocation ("specularColor");
	_cameraPositionLoc = _shaderProgram->getUniformLocation ("cameraPosition");
	_vertexLoc = _shaderProgram->getAttribLocation ("vertex");
	_normalLoc = _shaderProgram->getAttribLocation ("normal");
	_uvLoc = _shaderProgram->getAttribLocation ("uv");
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
}


//////////////////////////////|	SETTERS
void LitMaterial::SetColor (glm::vec3 pColor)
{
	_color = pColor;
}
void LitMaterial::SetTexture (std::string pFilename)
{
	string filepath = config::MGE_TEXTURE_PATH + pFilename;
	Texture* temp = Texture::load (filepath);
	if (_texture == nullptr)
	{
		cout << "Failed to load '" << filepath << "'. The texture will not be replaced." << endl;
		return;
	}
	_texture = temp;
}
void LitMaterial::SetTextureScale (float pScale)
{
	_texScale = pScale;
}
void LitMaterial::SetShininess (float pShininess)
{
	_shininess = pShininess;
}
void LitMaterial::SetSpecularColor (glm::vec3 pColor)
{
	_specularColor = pColor;
}


//////////////////////////////|	RENDERING
//Render the material
void LitMaterial::render (Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix)
{
	_shaderProgram->use ();

	//Send textures to the shader
	if (_useTexture)
	{
		glActiveTexture (GL_TEXTURE0);
		glBindTexture (GL_TEXTURE_2D, _texture->getId ());
		glUniform1i (_textureLoc, 0);
		glUniform1f (_texScaleLoc, _texScale);
	}
	else
	{
		glUniform3fv (_colorLoc, 1, glm::value_ptr (_color));
	}

	//Send uniform data to shaderprogram
	glm::mat4 MVPmatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv (_MVPmatrixLoc, 1, GL_FALSE, glm::value_ptr (MVPmatrix));
	glUniformMatrix4fv (_modelMatrixLoc, 1, GL_FALSE, glm::value_ptr (pModelMatrix));
	glUniform1i (_useTextureLoc, _useTexture);
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

	//Send basic vertex data to shader
	pMesh->streamToOpenGL (_vertexLoc, _normalLoc, _uvLoc);

	glGetError ();//Read errors from openGL, if there are any
}