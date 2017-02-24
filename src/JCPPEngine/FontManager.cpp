#include "FontManager.hpp"
#include <iostream>


namespace JCPPEngine
{

	//Static fields
	static std::map <std::string, sf::Font> _fonts;


	//If necessary, creates requested font and then returns it
	sf::Font* FontManager::GetFont (std::string pFontFile)
	{
		if (_fonts.count (pFontFile) == 0)
		{
			_fonts [pFontFile] = sf::Font ();
			if (!_fonts [pFontFile].loadFromFile (pFontFile))
			{
				std::cout<<"[ERROR]: could not load the font '"<<pFontFile<<"'!"<<std::endl;
			}
		}
		return &_fonts [pFontFile];
	}

}