#include "mge/materials/WobbleMaterial.hpp"
#include <mge/core/AbstractGame.hpp>
#include <mge/core/Mesh.hpp>
#include <mge/core/Texture.hpp>
#include <mge/core/GameObject.hpp>


//Initialize static values
GLint WobbleMaterial::_MVPmatrixLoc = 0;
GLint WobbleMaterial::_vertexLoc = 0;
GLint WobbleMaterial::_normalLoc = 0;
GLint WobbleMaterial::_uvLoc = 0;
GLint WobbleMaterial::_timeLoc = 0;
ShaderProgram* WobbleMaterial::_shaderProgram = nullptr;


//////////////////////////////|	INSTANCE MANAGEMENT
//Constructor
WobbleMaterial::WobbleMaterial (std::string pFilename)
{
	_texture = Texture::load (config::MGE_TEXTURE_PATH + pFilename);
	if (_texture == nullptr)
	{
		cout << "There was a problem loading the texture. Using the default one instread." << endl;
		_texture = Texture::load (config::MGE_TEXTURE_PATH + "Default.png");
	}
	if (_shaderProgram == nullptr)//If the shader is not already loaded, load it.
	{
		initializeShader ();
	}
}
//Destructor
WobbleMaterial::~WobbleMaterial ()
{
}
void WobbleMaterial::clearShaderProgram ()
{
	if (_shaderProgram != nullptr)
	{
		delete _shaderProgram;
		_shaderProgram = nullptr;
	}
}
//Set up the shader
void WobbleMaterial::initializeShader ()
{
	_shaderProgram = new ShaderProgram ();
	_shaderProgram->addShader (GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "wobble.vs");
	_shaderProgram->addShader (GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "wobble.fs");
	_shaderProgram->finalize ();

	//Store uniform and attribute indices
	_MVPmatrixLoc = _shaderProgram->getUniformLocation ("MVPmatrix");
	_timeLoc = _shaderProgram->getUniformLocation ("time");
	_vertexLoc = _shaderProgram->getAttribLocation ("vertex");
	_normalLoc = _shaderProgram->getAttribLocation ("normal");
	_uvLoc = _shaderProgram->getAttribLocation ("uv");
}


//////////////////////////////|	RENDERING
//Render the material
void WobbleMaterial::render (Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix)
{
	_shaderProgram->use ();

	//Send textures to the shader
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, _texture->getId ());
	glUniform1i (_shaderProgram->getUniformLocation ("texture"), 0);

	//Send uniform data to shaderprogram
	glm::mat4 MVPmatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv (_MVPmatrixLoc, 1, GL_FALSE, glm::value_ptr (MVPmatrix));
	glUniform1f (_timeLoc, AbstractGame::Time ());
	pMesh->streamToOpenGL (_vertexLoc, _normalLoc, _uvLoc);

	glGetError ();//Read errors from openGL
}