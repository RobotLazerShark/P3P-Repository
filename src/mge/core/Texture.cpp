#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include "mge/core/Texture.hpp"

std::map<std::string, Texture*> Texture::_textures; // for static texturs var

Texture::Texture(): _id() {
    glGenTextures (1, &_id);
}

Texture::~Texture()
{
	glDeleteTextures(1, &_id);
}

void Texture::ClearCache ()
{
	std::map<std::string,Texture*>::iterator itr;
	std::map<std::string,Texture*>::iterator end = _textures.end ();
	for (itr = _textures.begin (); itr != end; itr++)
	{
		delete itr->second;
	}
	_textures.clear ();
}

GLuint Texture::getId() {
	return _id;
}

// importer for textures
Texture* Texture::load(std::string pName)
{
    Texture* texture = nullptr;

    //try to locate texture
    std::map<std::string, Texture*>::iterator textureIterator = _textures.find(pName);

    if (textureIterator == _textures.end())
    {
	sf::Image image;
	if (image.loadFromFile (pName))
	{
	        texture = new Texture ();
	        glBindTexture (GL_TEXTURE_2D, texture->getId());
	        image.flipVertically ();
	        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	        glBindTexture(GL_TEXTURE_2D, 0);
	        _textures[pName] = texture;
	}
	else
	{
		return nullptr;
	}
//	std::cout << "Texture " << pName << " with id " << texture->getId() << " loaded." << std::endl;
//	std::cout << "Caching texture." << std::endl;
    } else {
//	std::cout << "Returning cached texture " << pName << std::endl;
        texture = textureIterator->second;
    }

    return texture;
}

Texture* Texture::_loadFromFile(std::string pName) {
    // load from file and store in cache
    sf::Image image;
    if (image.loadFromFile(pName)) {
        //normal image 0,0 is top left, but opengl uv 0,0 is bottom left, so we flip the image internally
        image.flipVertically();
        //create a wrapper for the id (texture is nothing more than that) and
        //load corresponding data into opengl using this id
        Texture * texture = new Texture();
        glBindTexture (GL_TEXTURE_2D, texture->getId());
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        return texture;
    } else {
        return nullptr;
    }
}










