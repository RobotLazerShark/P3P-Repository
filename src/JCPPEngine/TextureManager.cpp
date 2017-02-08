#include "TextureManager.hpp"


namespace JCPPEngine
{

	//Static fields
	static std::map <std::string, sf::Texture*> _textures;


	//If necessary, creates requested texture and then returns it
	sf::Texture* TextureManager::GetTexture (std::string pTextureFile)
	{
		if (_textures.count (pTextureFile) == 0)
		{
			sf::Texture* texture = new sf::Texture ();
			texture->loadFromFile (pTextureFile);
			_textures [pTextureFile] = texture;
			return texture;
		}
		return _textures [pTextureFile];
	}


	//Clear all used memory
	void TextureManager::Clean ()
	{
		std::map <std::string, sf::Texture*>::iterator end = _textures.end ();
		for (std::map <std::string, sf::Texture*>::iterator itr = _textures.begin (); itr != end; ++itr)
		{
			delete itr->second;
		}
		_textures.clear ();
	}

}