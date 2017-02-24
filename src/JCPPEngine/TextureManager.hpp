#ifndef TextureManager_Def
#define TextureManager_Def

//Includes
#include <map>
#include <SFML/Graphics.hpp>


namespace JCPPEngine
{
	class TextureManager
	{
		public:
			static sf::Texture* GetTexture (std::string pTextureFile);
	};
}

#endif