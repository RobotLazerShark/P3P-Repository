#include "FontManager.hpp"


namespace JCPPEngine
{

	//Static fields
	static std::map <std::string, sf::Font*> _fonts;


	//If necessary, creates requested font and then returns it
	sf::Font* FontManager::GetFont (std::string pFontFile)
	{
		if (_fonts.count (pFontFile) == 0)
		{
			sf::Font* font = new sf::Font ();
			font->loadFromFile (pFontFile);
			_fonts [pFontFile] = font;
			return font;
		}
		return _fonts [pFontFile];
	}


	//Clear all used memory
	void FontManager::Clean ()
	{
		std::map <std::string, sf::Font*>::iterator end = _fonts.end ();
		for (std::map <std::string, sf::Font*>::iterator itr = _fonts.begin (); itr != end; ++itr)
		{
			delete itr->second;
		}
		_fonts.clear ();
	}

}