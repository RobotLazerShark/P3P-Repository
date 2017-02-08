#ifndef DoomSprite_Def
#define DoomSprite_Def


//Include files
#include <mge/objects/DoomObject.hpp>


class DoomSprite : public DoomObject
{

	public:
		DoomSprite (std::string pFilename);
		DoomSprite (glm::vec3 pPosition, std::string pFilename);
	private:

};

#endif