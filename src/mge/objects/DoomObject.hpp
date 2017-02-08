#ifndef DoomObject_Def
#define DoomObject_Def


//Include files
#include <glm.hpp>
#include <SFML/Graphics.hpp>
#include <mge/core/GameObject.hpp>
#include <JCPPEngine/TextureManager.hpp>
#include <JCPPEngine/InputManager.hpp>
#include <mge/materials/AbstractMaterial.hpp>


class DoomObject : public GameObject, public AbstractMaterial
{

	public:
		DoomObject ();
		DoomObject (glm::vec3 pPosition);
		bool CheckIfClicked (sf::Vector2i pMouseClickPosition);
		void SetClickHitbox (glm::vec2 pOffset, glm::vec2 pHalfSize);
		virtual void render (Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix);
	protected:
		const float SCALE_FACTOR = 40;
		sf::Sprite* _sprite;
		bool _valid = false;
		glm::vec2 _screenPosition;
		float _imageScale;
		glm::vec2 _imageHalfSize;
		glm::vec2 _hitboxOffset;
		glm::vec2 _hitboxHalfSize;

};

#endif