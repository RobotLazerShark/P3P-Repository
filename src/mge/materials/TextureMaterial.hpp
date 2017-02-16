#ifndef TEXTUREMATERIAL_H
#define TEXTUREMATERIAL_H

#include "mge/core/ShaderProgram.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include <GL/glew.h>

class Texture;


class TextureMaterial : public AbstractMaterial
{
    public:
        TextureMaterial (Texture* pDiffuseTexture, float pScale = 1);
        virtual ~TextureMaterial ();
	static void clearShaderProgram ();
        virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

        void setDiffuseTexture (Texture* pDiffuseTexture);
	void setTextureScale (float pScale);

    protected:
    private:
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();
	static GLint _scaleLoc;
	static GLint _textureLoc;
	static GLint _MVPmatrixLoc;

        Texture* _diffuseTexture = nullptr;
	float _scale;

        TextureMaterial(const TextureMaterial&);
        TextureMaterial& operator=(const TextureMaterial&);

};

#endif // TEXTUREMATERIAL_H
