#include <glm.hpp>

#include "mge/materials/TextureMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"

ShaderProgram* TextureMaterial::_shader = NULL;
GLint TextureMaterial::_MVPmatrixLoc = 0;
GLint TextureMaterial::_scaleLoc = 0;
GLint TextureMaterial::_textureLoc = 0;

TextureMaterial::TextureMaterial(Texture * pDiffuseTexture, float pScale):_diffuseTexture(pDiffuseTexture) {
	_scale = pScale;
    _lazyInitializeShader();
}

TextureMaterial::~TextureMaterial() {}

void TextureMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"texture.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"texture.fs");
        _shader->finalize();

	_scaleLoc = _shader->getUniformLocation ("scale");
	_textureLoc = _shader->getUniformLocation ("textureDiffuse");
	_MVPmatrixLoc = _shader->getUniformLocation ("MVPmatrix");
    }
}

void TextureMaterial::setDiffuseTexture (Texture* pDiffuseTexture) {
    _diffuseTexture = pDiffuseTexture;
}
void TextureMaterial::setTextureScale (float pScale)
{
	_scale = pScale;
}

void TextureMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (!_diffuseTexture) return;

    _shader->use();

    //setup texture slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
    glUniform1i (_textureLoc, 0);

    //pass in the MVP matrix
	glm::mat4 MVPmatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
    glUniformMatrix4fv ( _MVPmatrixLoc, 1, GL_FALSE, glm::value_ptr (MVPmatrix));

	glUniform1f (_scaleLoc, _scale);

    //now inform mesh of where to stream its data
    pMesh->streamToOpenGL(
        _shader->getAttribLocation("vertex"),
        _shader->getAttribLocation("normal"),
        _shader->getAttribLocation("uv")
    );
}
