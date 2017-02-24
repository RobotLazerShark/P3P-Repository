#include "TextureManager.hpp"
#include <iostream>


namespace JCPPEngine
{

	//Static fields
	static std::map <std::string, sf::Texture> _textures;


	//If necessary, creates requested texture and then returns it
	sf::Texture* TextureManager::GetTexture (std::string pTextureFile)
	{
		if (_textures.count (pTextureFile) == 0)
		{
			_textures [pTextureFile] = sf::Texture ();
			if (!_textures [pTextureFile].loadFromFile (pTextureFile))
			{
				std::cout<<"[ERROR]: could not load the texture '"<<pTextureFile<<"'!"<<std::endl;
			}
		}
		return &_textures [pTextureFile];
	}

}