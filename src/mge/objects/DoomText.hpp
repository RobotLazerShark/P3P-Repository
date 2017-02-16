#ifndef DoomText_Def
#define DoomText_Def


//Include files
#include <mge/objects/DoomObject.hpp>


class DoomText : public DoomObject
{

	public:
		DoomText (std::string pText, std::string pFontFile, unsigned int pSize, glm::vec3 pColor);
		DoomText (std::string pText, std::string pFontFile, unsigned int pSize, glm::vec3 pPosition, glm::vec3 pColor);
		void render (Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;
	protected:
		sf::Text* _text = nullptr;

};

#endif