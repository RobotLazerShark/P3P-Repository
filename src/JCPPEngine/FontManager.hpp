#ifndef FontManager_Def
#define FontManager_Def

//Includes
#include <map>
#include <SFML/Graphics.hpp>


namespace JCPPEngine
{
	class FontManager
	{
		public:
			static sf::Font* GetFont (std::string pFontFile);
			static void Clean ();
	};
}

#endif