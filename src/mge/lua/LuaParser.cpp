#include "LuaParser.hpp"
#ifndef LuaParser_CPP_Def
#define LuaParser_CPP_Def
#include <stdio.h>
#include <glm.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <mge/core/World.hpp>
#include <mge/core/Texture.hpp>
#include <mge/core/GameObject.hpp>
#include <mge/core/AbstractGame.hpp>
#include <mge/core/Mesh.hpp>
#include <mge/core/TerrainMesh.hpp>
#include <mge/util/ShaderDataUtil.hpp>
#include <mge/objects/Camera.hpp>
#include <mge/objects/Light.hpp>
#include <mge/objects/DoomSprite.hpp>
#include <mge/objects/DoomAnimation.hpp>
#include <mge/objects/DoomText.hpp>
#include <mge/materials/ColorMaterial.hpp>
#include <mge/materials/TextureMaterial.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <mge/materials/TerrainMaterial.hpp>
#include <mge/behaviours/RotatingBehaviour.hpp>
#include <mge/behaviours/KeysBehaviour.hpp>
#include <P3P/objects/Npc.hpp>
#include <P3P/objects/Player.hpp>
#include <P3P/Level.hpp>
using namespace std;
using namespace JCPPEngine;



//Static fields
LuaParser* LuaParser::singletonInstance = nullptr;
sf::RenderWindow* LuaParser::_window = nullptr;


//////////////////////////////| STATIC/LUA FUNCTIONS
//Throw an error
void throwError (string pErrorMessage)
{
	//Display the error message
	cout << "[ERROR]: " << pErrorMessage << endl;
	cout << "at function " << LuaParser::singletonInstance->_currentFunction << endl;
	//Stop the LuaParser
	LuaParser::singletonInstance->_errorRaised = true;
}
////////////////////|	DRAWABLES
//Put drawable in drawbuffer
int putInDrawBuffer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -1))
	{
		sf::Drawable* drawable = (sf::Drawable*)lua_tointeger (pLua, -1);
		if (drawable != nullptr)
		{
			LuaParser::singletonInstance->_drawBuffer.push_back (drawable);
		}
		else
		{
			throwError ("Function: draw\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: draw\nParameter was incorrect!\nIt should be: (int) pointer to drawable.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Put a table of drawables in the drawbuffer
int putTableInDrawBuffer (lua_State* pLua)
{
	if (lua_istable (pLua, -1))
	{
		lua_len (pLua, -1);//Get the amount of items in the lua table (value is added to the stack, table is now at -2)
		int tableSize = lua_tointeger (pLua, -1);
		for (int i = 0; i < tableSize; i ++)
		{
			lua_pushinteger (pLua, (i + 1));//Push index we want to access in the table onto the stack (table is now at -3)
			lua_gettable (pLua, -3);//Replaces the key with the sub-table
			lua_pushstring (pLua, "pointer");//Push key we want to access in the sub-table onto the stack (sub-table is now at -2, table is now at -4)
			lua_gettable (pLua, -2);//Replaces the key with its corresponding value
			LuaParser::singletonInstance->_drawBuffer.push_back ((sf::Drawable*)lua_tointeger (pLua, -1));
			lua_pop (pLua, 2);//Remove the pointer and sub-table from the stack (table is now at -3)
		}
	}
	else
	{
		throwError ("Function: drawTable\nParameter was incorrect!\nIt should be: (table <Image/Text>) drawables.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Delete a drawable
int deleteDrawable (lua_State* pLua)
{
	if (lua_isinteger (pLua, -1))
	{
		sf::Drawable* drawable = (sf::Drawable*)lua_tointeger (pLua, -1);
		if (drawable != nullptr)
		{
			delete drawable;
		}
	//	else
	//	{
	//		throwError ("Function: deleteDrawable\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
	//	}
	}
	else
	{
		throwError ("Function: deleteDrawable\nParameter was incorrect!\nIt should be: (int) pointer to drawable.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Get the position of a drawable
int getDrawablePosition (lua_State* pLua)
{
	if (lua_isinteger (pLua, -1))
	{
		int pointer = lua_tointeger (pLua,-1);
		sf::Sprite* image = (sf::Sprite*)pointer;
		sf::Text* text = (sf::Text*)pointer;
		lua_settop (pLua, 0);
		if (image != nullptr || text != nullptr)
		{
			sf::Vector2f position;
			if (image != nullptr)//Can't use the transformable functions directly, because it screws up the coördinates
			{
				position = image->getPosition ();
			}
			else
			{
				position = text->getPosition ();
			}
			lua_newtable (pLua);//Creating the lua table (table now at -1)
			lua_pushstring (pLua, "x");//Push the key onto the stack (table now at -2)
			lua_pushnumber (pLua, position.x);//Push the value onto the stack (table now at -3)
			lua_settable (pLua, -3);//Put the key-value pair in the table (table now at -1)
			lua_pushstring (pLua, "y");//Push the key onto the stack (table now at -2)
			lua_pushnumber (pLua, position.y);//Push the value onto the stack (table now at -3)
			lua_settable (pLua, -3);//Put the key-value pair in the table
		}
		else
		{
			throwError ("Function: getDrawablePosition\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
			lua_pushnil (pLua);
		}
	}
	else
	{
		throwError ("Function: getDrawablePosition\nParameter was incorrect!\nIt should be: (int) pointer to drawable.");
		lua_pushnil (pLua);
	}
	return 1;//Return a table of the drawable's position
}
//Set a drawable's position
int setDrawablePosition (lua_State* pLua)
{
	if (lua_isinteger (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		int pointer = lua_tointeger (pLua, -3);
		sf::Sprite* image = (sf::Sprite*)pointer;
		sf::Text* text = (sf::Text*)pointer;
		if (image != nullptr || text != nullptr)
		{
			sf::Vector2f position;
			if (image != nullptr)//Can't use the transformable functions directly, because it screws up the coördinates
			{
				image->setPosition (lua_tonumber (pLua, -2), lua_tonumber (pLua, -1));
			}
			else
			{
				text->setPosition (lua_tonumber (pLua, -2), lua_tonumber (pLua, -1));
			}
		}
		else
		{
			throwError ("Function: setDrawablePosition\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: setDrawablePosition\nParameter was incorrect!\nIt should be: (int) pointer to drawable, (float) new x, (float) new y.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Move a drawable
int moveDrawable (lua_State* pLua)
{
	if (lua_isinteger (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		int pointer = lua_tointeger (pLua,-3);
		sf::Sprite* image = (sf::Sprite*)pointer;
		sf::Text* text = (sf::Text*)pointer;
		if (image != nullptr || text != nullptr)
		{
			sf::Vector2f movement = sf::Vector2f (lua_tonumber (pLua, -2), lua_tonumber (pLua, -1));
			if (image != nullptr)//Can't use the transformable functions directly, because it screws up the coördinates
			{
				image->move (movement);
			}
			else
			{
				text->move (movement);
			}
		}
		else
		{
			throwError ("Function: moveDrawable\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: moveDrawable\nParameter was incorrect!\nIt should be: (int) pointer to drawable, (float) x movement, (float) y movement.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Rotate a drawable
int rotateDrawable (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		int pointer = lua_tointeger (pLua, -2);
		sf::Sprite* image = (sf::Sprite*)pointer;
		sf::Text* text = (sf::Text*)pointer;
		if (image != nullptr || text != nullptr)
		{
			if (image != nullptr)//Can't use the transformable functions directly, because it screws up the coördinates
			{
				image->rotate (lua_tonumber (pLua, -1));
			}
			else
			{
				text->rotate (lua_tonumber (pLua, -1));
			}
		}
		else
		{
			throwError ("Function: rotateDrawable\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: rotateDrawable\nParameter was incorrect!\nIt should be: (int) pointer to drawable, (float) rotation in degrees.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Scale a drawable
int scaleDrawable (lua_State* pLua)
{
	if (lua_isinteger (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		int pointer = lua_tointeger (pLua,-3);
		sf::Sprite* image = (sf::Sprite*)pointer;
		sf::Text* text = (sf::Text*)pointer;
		if (image != nullptr || text != nullptr)
		{
			sf::Vector2f scale = sf::Vector2f (lua_tonumber (pLua, -2), lua_tonumber (pLua, -1));
			if (image != nullptr)//Can't use the transformable functions directly, because it screws up the coördinates
			{
				image->scale (scale);
			}
			else
			{
				text->scale (scale);
			}
		}
		else
		{
			throwError ("Function: scaleDrawable\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else if (lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		int pointer = lua_tointeger (pLua, -2);
		sf::Sprite* image = (sf::Sprite*)pointer;
		sf::Text* text = (sf::Text*)pointer;
		if (image != nullptr || text != nullptr)
		{
			float scale = lua_tonumber (pLua, -1);
			if (image != nullptr)//Can't use the transformable functions directly, because it screws up the coördinates
			{
				image->scale (scale, scale);
			}
			else
			{
				text->scale (scale, scale);
			}
		}
		else
		{
			throwError ("Function: scaleDrawable\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: scaleDrawable\nParameter was incorrect!\nIt should be: (int) pointer to drawable, (float) scale OR\n(int) pointer to drawable, (float) x scale, (float) y scale.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//////////|	IMAGES
//Create an image
int createImage (lua_State* pLua)
{
	if (lua_isstring (pLua, -3) && lua_isinteger (pLua, -2) && lua_isinteger (pLua, -1))
	{
		string filePath = "images/" + (string)lua_tostring (pLua, -3);
		sf::Texture* texture = TextureManager::GetTexture (filePath);
		sf::Sprite* image = new sf::Sprite (*texture);
		sf::Vector2u size = texture->getSize ();
		image->setOrigin (size.x * 0.5f, size.y * 0.5f);
		image->setPosition ((float)lua_tointeger (pLua, -2), (float)lua_tointeger (pLua, -1));
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)image);
	}
	else
	{
		throwError ("Function: image\nParameter was incorrect!\nIt should be: (string) filename, (int) x, (int) y.\nNote that the position is relative to the upper left corner of the image.");
		lua_settop (pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;//Return pointer to created object
}
//Check if a given image was clicked
int checkButton (lua_State* pLua)
{
	if (lua_isinteger (pLua, -1))
	{
		sf::Sprite* image = (sf::Sprite*)lua_tointeger (pLua, -1);
		lua_settop (pLua, 0);
		if (image != nullptr)
		{
			//Check if the button was clicked
			JCPPEngine::MouseEvent* mouseEvent = LuaParser::singletonInstance->_mouseEvent;
			if (mouseEvent != nullptr)
			{
				sf::Vector2u halfSize = image->getTexture ()->getSize ();
				halfSize.x *= 0.5f;
				halfSize.y *= 0.5f;
				sf::Vector2f imagePosition = image->getPosition ();
				sf::Vector2i eventPosition = mouseEvent->position ();
				if
				(
					(eventPosition.x > imagePosition.x - halfSize.x && eventPosition.x < imagePosition.x + halfSize.x) &&
					(eventPosition.y > imagePosition.y - halfSize.x && eventPosition.y < imagePosition.y + halfSize.y)
				)
				{
					lua_pushboolean (pLua, true);
				}
				else
				{
					lua_pushboolean (pLua, false);
				}
			}
			else
			{
				lua_pushboolean (pLua, false);
			}
		}
		else
		{
			throwError ("Function: ifClicked\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
			lua_pushboolean (pLua, false);
		}
	}
	else
	{
		throwError ("Function: ifClicked\nParameter was incorrect!\nIt should be: (int) pointer to image.");
		lua_pushboolean (pLua, false);
	}
	return 1;//Return whether image was clicked
}
//////////|	TEXT
//Create a text
int createText (lua_State* pLua)
{
	if (lua_isstring (pLua, -8) && lua_isstring (pLua, -7) && lua_isinteger (pLua, -6) && lua_isinteger (pLua, -5) && lua_isinteger (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		string filePath = "fonts/" + (string)lua_tostring (pLua, -7);
		sf::Text* text = new sf::Text (lua_tostring (pLua, -8), *FontManager::GetFont (filePath), (unsigned int)lua_tointeger (pLua, -6));
		text->setPosition (lua_tointeger (pLua, -5), lua_tointeger (pLua, -4));
		text->setFillColor (sf::Color ((int)(lua_tonumber (pLua, -3) * 255), (int)(lua_tonumber (pLua, -2) * 255), (int)(lua_tonumber (pLua, -1) * 255)));
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)text);
	}
	else if (lua_isstring (pLua, -5) && lua_isstring (pLua, -4) && lua_isinteger (pLua, -3) && lua_isinteger (pLua, -2) && lua_isinteger (pLua, -1))
	{
		string filePath = "fonts/" + (string)lua_tostring (pLua, -4);
		sf::Text* text = new sf::Text (lua_tostring (pLua, -5), *FontManager::GetFont (filePath), (unsigned int)lua_tointeger (pLua, -3));
		text->setPosition (lua_tointeger (pLua, -2), lua_tointeger (pLua, -1));
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)text);
	}
	else
	{
		throwError ("Function: text\nParameter was incorrect!\nIt should be: (string) text, (string) font, (int) size, (int) x, (int) y. OR\n(string) text, (string) font, (int) size, (int) x, (int) y, (float) r, (float) g, (float) b.\nNote the rgb values should be in the range 0 - 1 and\nthat the position is relative to the upper left corner of the text.");
		lua_settop (pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;//Return pointer to created object
}
//////////|	SOUND
//Play a sound
int playSound (lua_State* pLua)
{
	if (lua_isstring (pLua, -1))
	{
		string filePath = "sounds/" + (string)lua_tostring (pLua, -1);
		sf::SoundBuffer* buffer = SoundManager::GetBuffer (filePath);
		if (buffer == nullptr)
		{
			throwError ("The file '" + filePath + "' could not be loaded!");
			lua_settop (pLua, 0);
			return 0;
		}
		sf::Sound* sound = new sf::Sound (*buffer);
		SoundManager::PlaySound (sound);//SoundManager takes care of deleting the sound when it's done playing
	}
	else
	{
		throwError ("Function: sound\nParameter was incorrect!\nIt should be: (string) filename.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Play a looping sound
int playLoopingSound (lua_State* pLua)
{
	if (lua_isstring (pLua, -1))
	{
		string filePath = "sounds/" + (string)lua_tostring (pLua, -1);
		int returnValue = SoundManager::PlayMusicLoop (filePath);
		if (returnValue == -1)
		{
			throwError ("The file '" + filePath + "' could not be loaded!");
			lua_settop (pLua, 0);
			lua_pushinteger (pLua, -1);
			return 1;
		}
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, returnValue);
	}
	else
	{
		throwError ("Function: loopSound\nParameter was incorrect!\nIt should be: (string) filename.");
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, -1);
	}
	return 1;//Return the sound's index (in the soundmanager)
}
//Stop a looping sound
int stopLoopingSound (lua_State* pLua)
{
	if (lua_isinteger (pLua, -1))
	{
		int index = lua_tointeger (pLua, -1);
		if (index < 0)
		{
			throwError ("Function: stopLoop\nParameter was incorrect!\nThe loop-index is invalid.\nMost likely there was a problem playing the sound.");
			lua_settop (pLua, 0);
			return 0;
		}
		SoundManager::StopMusicLoop (index);//SoundManager takes care of deleting the music when it's stopped
	}
	else
	{
		throwError ("Function: stopLoop\nParameter was incorrect!\nIt should be: (int) sound-loop index.");
	}
	lua_settop (pLua, 0);
	return 0;
}
////////////////////|	IMPORTING FUNCTIONS
//Import camera from Unity scene and add it to the world
int importUnityCamera (lua_State* pLua)
{
	if (lua_isnumber (pLua, -7) && lua_isnumber (pLua, -6) && lua_isnumber (pLua, -5) && lua_isnumber (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		float FOV = lua_tonumber (pLua, -1);
		float xRotation = glm::radians (lua_tonumber (pLua, -4));
		float yRotation = glm::radians (lua_tonumber (pLua, -3));
		float zRotation = glm::radians (lua_tonumber (pLua, -2));
		glm::mat4 transform = glm::translate (glm::vec3 (lua_tonumber (pLua, -7), lua_tonumber (pLua, -6), lua_tonumber (pLua, -5)));
		transform = transform * glm::rotate (yRotation, glm::vec3 (0, 1, 0)) * glm::rotate (xRotation, glm::vec3 (1, 0, 0)) * glm::rotate (zRotation, glm::vec3 (0, 0, 1));
		Camera* camera = new Camera (FOV);
		camera->setTransform (transform);
		if (LuaParser::singletonInstance->_curScene != nullptr)//We are importing the object as part of a scene
		{
			camera->setParent (LuaParser::singletonInstance->_curScene);
		}
		else
		{
			camera->setParent (World::singletonInstance);
		}
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)camera);//Pass (the pointer to) this object to lua
	}
	else
	{
		throwError ("[IMPORT]: Parameter was incorrect!\nIt should be: (float) position x, (float) position y, (float) position z,\n(float) rotation x, (float) rotation y, (float) rotation z,\n(float) field-of-view angle.");
		lua_settop (pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;//Return the camera 'object'
}
//Import light from Unity scene and add it to the world
int importUnityLight (lua_State* pLua)
{
	if (lua_isinteger (pLua, -1))
	{
		int lightType = lua_tointeger (pLua, -1);
		switch (lightType)
		{
			case 1://Directional light
				if
				(
					lua_isnumber (pLua, -11) && lua_isnumber (pLua, -10) && lua_isnumber (pLua, -9) && lua_isnumber (pLua, -8) && lua_isnumber (pLua, -7) && lua_isnumber (pLua, -6)
					&& lua_isnumber (pLua, -5) && lua_isnumber (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2)
				)
				{
					float intensity = lua_tonumber (pLua, -2);
					glm::vec3 color = glm::vec3 (lua_tonumber (pLua, -5), lua_tonumber (pLua, -4), lua_tonumber (pLua, -3));
					float xRotation = glm::radians (lua_tonumber (pLua, -8));
					float yRotation = glm::radians (lua_tonumber (pLua, -7));
					float zRotation = glm::radians (lua_tonumber (pLua, -6));
					glm::mat4 transform = glm::translate (glm::vec3 (lua_tonumber (pLua, -11), lua_tonumber (pLua, -10), lua_tonumber (pLua, -9)));
					transform = transform * glm::rotate (yRotation, glm::vec3 (0, 1, 0)) * glm::rotate (xRotation, glm::vec3 (1, 0, 0)) * glm::rotate (zRotation, glm::vec3 (0, 0, 1));
					Light* light = new Light (color, intensity);
					light->setTransform (transform);
					if (LuaParser::singletonInstance->_curScene != nullptr)//We are importing the object as part of a scene
					{
						light->setParent (LuaParser::singletonInstance->_curScene);
					}
					else
					{
						light->setParent (World::singletonInstance);
					}
					lua_settop(pLua, 0);
					lua_pushinteger (pLua, (int)light);//Pass (the pointer to) this object to lua
				}
				else
				{
					throwError ("[IMPORT]: Parameter was incorrect!\nIt should be: (float) position x, (float) position y, (float) position z,\n(float) rotation x, (float) rotation y, (float) rotation z,\n(float) red, (float) green, (float) blue, (float) intensity, (int) lighttype");
					lua_settop(pLua, 0);
					lua_pushnil (pLua);
				}
				break;
			case 2://Point light
				if
				(
					lua_isnumber (pLua, -13) && lua_isnumber (pLua, -12) && lua_isnumber (pLua, -11) && lua_isnumber (pLua, -10) && lua_isnumber (pLua, -9) && lua_isnumber (pLua, -8)
					&& lua_isnumber (pLua, -7) && lua_isnumber (pLua, -6) && lua_isnumber (pLua, -5) && lua_isnumber (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2)
				)
				{
					float intensity = lua_tonumber (pLua, -4);
					glm::vec3 color = glm::vec3 (lua_tonumber (pLua, -7), lua_tonumber (pLua, -6), lua_tonumber (pLua, -5));
					glm::vec2 falloff = glm::vec2 (lua_tonumber (pLua, -3), lua_tonumber (pLua, -2));
					float xRotation = glm::radians (lua_tonumber (pLua, -10));
					float yRotation = glm::radians (lua_tonumber (pLua, -9));
					float zRotation = glm::radians (lua_tonumber (pLua, -8));
					glm::vec3 position = glm::vec3 (lua_tonumber (pLua, -13), lua_tonumber (pLua, -12), lua_tonumber (pLua, -11));
					Light* light = new Light (color, intensity, position, falloff);
					glm::mat4 transform = light->getTransform () * glm::rotate (yRotation, glm::vec3 (0, 1, 0)) * glm::rotate (xRotation, glm::vec3 (1, 0, 0)) * glm::rotate (zRotation, glm::vec3 (0, 0, 1));
					light->setTransform (transform);
					if (LuaParser::singletonInstance->_curScene != nullptr)//We are importing the object as part of a scene
					{
						light->setParent (LuaParser::singletonInstance->_curScene);
					}
					else
					{
						light->setParent (World::singletonInstance);
					}
					lua_settop(pLua, 0);
					lua_pushinteger (pLua, (int)light);//Pass (the pointer to) this object to lua
				}
				else
				{
					throwError ("[IMPORT]: Parameter was incorrect!\nIt should be: (float) position x, (float) position y, (float) position z,\n(float) rotation x, (float) rotation y, (float) rotation z,\n(float) red, (float) green, (float) blue, (float) intensity,\n(float) linear falloff, (float) quadratic falloff, (int) lighttype");
					lua_settop(pLua, 0);
					lua_pushnil (pLua);
				}
				break;
			case 3://Spot light
				if
				(
					lua_isnumber (pLua, -14) && lua_isnumber (pLua, -13) && lua_isnumber (pLua, -12) && lua_isnumber (pLua, -11) && lua_isnumber (pLua, -10) && lua_isnumber (pLua, -9)
					&& lua_isnumber (pLua, -8) && lua_isnumber (pLua, -7) && lua_isnumber (pLua, -6) && lua_isnumber (pLua, -5) && lua_isnumber (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2)
				)
				{
					float intensity = lua_tonumber (pLua, -5);
					glm::vec3 color = glm::vec3 (lua_tonumber (pLua, -8), lua_tonumber (pLua, -7), lua_tonumber (pLua, -6));
					glm::vec2 falloff = glm::vec2 (lua_tonumber (pLua, -3), lua_tonumber (pLua, -2));
					float angle = glm::radians (lua_tonumber (pLua, -4));
					float xRotation = glm::radians (lua_tonumber (pLua, -11));
					float yRotation = glm::radians (lua_tonumber (pLua, -10));
					float zRotation = glm::radians (lua_tonumber (pLua, -9));
					glm::vec3 position = glm::vec3 (lua_tonumber (pLua, -14), lua_tonumber (pLua, -13), lua_tonumber (pLua, -12));
					Light* light = new Light (color, intensity, position, angle, falloff);
					glm::mat4 transform = light->getTransform () * glm::rotate (yRotation, glm::vec3 (0, 1, 0)) * glm::rotate (xRotation, glm::vec3 (1, 0, 0)) * glm::rotate (zRotation, glm::vec3 (0, 0, 1));
					light->setTransform (transform);
					if (LuaParser::singletonInstance->_curScene != nullptr)//We are importing the object as part of a scene
					{
						light->setParent (LuaParser::singletonInstance->_curScene);
					}
					else
					{
						light->setParent (World::singletonInstance);
					}
					lua_settop(pLua, 0);
					lua_pushinteger (pLua, (int)light);//Pass (the pointer to) this object to lua
				}
				else
				{
					throwError ("[IMPORT]: Parameter was incorrect!\nIt should be: (float) position x, (float) position y, (float) position z,\n(float) rotation x, (float) rotation y, (float) rotation z,\n(float) red, (float) green, (float) blue, (float) intensity,\n(float) angle, (float) linear falloff, (float) quadratic falloff,\n(int) lighttype.");
					lua_settop(pLua, 0);
					lua_pushnil (pLua);
				}
				break;
			default:
				throwError ("[IMPORT]: Parameter was incorrect!\nIt should (at least) be:\n(float) position x, (float) position y, (float) position z,\n(float) rotation x, (float) rotation y, (float) rotation z,\n(float) red, (float) green, (float) blue, (float) intensity, (int) lighttype.");
				lua_settop(pLua, 0);
				lua_pushnil (pLua);
				break;
		}
	}
	else
	{
		throwError ("[IMPORT]: Parameter was incorrect!\nThe lighttype value was incorrect.");
		lua_settop (pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;
}
//Import other object from Unity scene and add it to the world
int importUnityObject (lua_State* pLua)
{
	if (lua_isnumber (pLua, -9) && lua_isnumber (pLua, -8) && lua_isnumber (pLua, -7) && lua_isnumber (pLua, -6) && lua_isnumber (pLua, -5) && lua_isnumber (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		glm::vec3 scale = glm::vec3 (lua_tonumber (pLua, -3), lua_tonumber (pLua, -2), lua_tonumber (pLua, -1));
		float xRotation = glm::radians (lua_tonumber (pLua, -6));
		float yRotation = glm::radians (lua_tonumber (pLua, -5));
		float zRotation = glm::radians (lua_tonumber (pLua, -4));
		glm::mat4 transform = glm::translate (glm::vec3 (lua_tonumber (pLua, -9), lua_tonumber (pLua, -8), lua_tonumber (pLua, -7)));
		transform = transform * glm::rotate (yRotation, glm::vec3 (0, 1, 0)) * glm::rotate (xRotation, glm::vec3 (1, 0, 0)) * glm::rotate (zRotation, glm::vec3 (0, 0, 1));
		GameObject* object = new GameObject ();
		object->setTransform (transform);
		object->badScale (scale);
		if (LuaParser::singletonInstance->_curScene != nullptr)//We are importing the object as part of a scene
		{
			object->setParent (LuaParser::singletonInstance->_curScene);
		}
		else
		{
			object->setParent (World::singletonInstance);
		}
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)object);//Pass (the pointer to) this object to lua
	}
	else
	{
		throwError ("[IMPORT]: Parameter was incorrect!\nIt should be: (float) position x, (float) position y, (float) position z,\n(float) rotation x, (float) rotation y, (float) rotation z,\n(float) scale. Note that rotations should be in degrees.");
		lua_settop (pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;
}
////////////////////| OBJECT MANIPULATION
//Set an object's mesh
int setMeshByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isstring (pLua, -1))
	{
		GameObject* object = ((GameObject*)lua_tointeger (pLua, -2));
		if (object != nullptr)
		{
			object->setMesh (lua_tostring (pLua, -1));
		}
		else
		{
			throwError ("Function: setMesh\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else if (lua_isinteger (pLua, -2) && lua_isinteger (pLua, -1))
	{
		GameObject* object = ((GameObject*)lua_tointeger (pLua, -2));
		Mesh* mesh = (Mesh*)lua_tointeger (pLua, -1);
		if (object != nullptr && mesh != nullptr)
		{
			object->setMesh (mesh);
		}
		else
		{
			std::string wrongPointer = (object == nullptr) ? "object" : "mesh";
			throwError ("Function: setMesh\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted "+wrongPointer+"!");
		}
	}
	else
	{
		throwError ("Function: setMesh\nParameter was incorrect!\nIt should be: (int) pointer to object, (string) mesh-filename. OR:\n(int) pointer to object, (int) pointer to mesh.\nThe pointer is returned when you create the object.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Set an object's material
int setMaterialByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isinteger (pLua, -1))
	{
		GameObject* object = ((GameObject*)lua_tointeger (pLua, -2));
		AbstractMaterial* material = ((AbstractMaterial*)lua_tointeger (pLua, -1));
		if (object != nullptr && material != nullptr)
		{
			object->setMaterial (material);
		}
		else
		{
			throwError ("Function: setMaterial\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: setMaterial\nParameter was incorrect!\nIt should be: (int) pointer to object, (int) pointer to material.\nThe pointer is returned when you create the object/material.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Set an object's behaviour
int setBehaviourByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isinteger (pLua, -1))
	{
		GameObject* object = ((GameObject*)lua_tointeger (pLua, -2));
		AbstractBehaviour* behaviour = ((AbstractBehaviour*)lua_tointeger (pLua, -1));
		if (object != nullptr && behaviour != nullptr)
		{
			object->setBehaviour (behaviour);
		}
		else
		{
			throwError ("Function: setBehaviour\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: setBehaviour\nParameter was incorrect!\nIt should be: (int) pointer to object, (int) pointer to behaviour.\nThe pointer is returned when you create the object/behaviour.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//////////|	MESHES
//Create mesh
int createMesh (lua_State* pLua)
{
	if (lua_isstring (pLua, -1))
	{
		Mesh* mesh = Mesh::load (config::MGE_MODEL_PATH + (string)lua_tostring (pLua, -1));
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)mesh);
	}
	else
	{
		throwError ("Function: mesh\nParameter was incorrect!\nIt should be: (string) mesh file.");
		lua_settop (pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;//Return pointer to created mesh
}
//Create terrainmesh
int createTerrainMesh (lua_State* pLua)
{
	if (lua_isstring (pLua, -3) && lua_isnumber (pLua, -2) && lua_isinteger (pLua, -1))
	{
		TerrainMesh* mesh = TerrainMesh::BuildTerrain (lua_tostring (pLua, -3), lua_tonumber (pLua, -2), lua_tointeger (pLua, -1));
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)mesh);
	}
	else if (lua_isstring (pLua, -1))
	{
		TerrainMesh* mesh = TerrainMesh::loadTerrain (lua_tostring (pLua, -1));
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)mesh);
	}
	else
	{
		throwError ("Function: terrainMesh\nParameter was incorrect!\nIt should be: (string) mesh file. OR:\n(string) heightmap, (float) max height, (int) terrain density/detail.");
		lua_settop (pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;//Return pointer to created mesh
}
//////////|	MATERIALS
//Create color material
int createColorMaterial (lua_State* pLua)
{
	if (lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		ColorMaterial* material = new ColorMaterial (glm::vec3 (lua_tonumber (pLua, -3), lua_tonumber (pLua, -2), lua_tonumber (pLua, -1)));
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)material);
	}
	else
	{
		throwError ("Function: colorMaterial\nParameter was incorrect!\nIt should be: (float) red, (float) green, (float) blue.\nNote that the rgb values should be in the range 0 - 1.");
		lua_settop (pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;//Return pointer to created material
}
//Change color material's color
int setColorMaterialColor (lua_State* pLua)
{
	if (lua_isinteger (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		ColorMaterial* material = (ColorMaterial*)lua_tointeger (pLua, -4);
		if (material != nullptr)
		{
			material->setDiffuseColor (glm::vec3 (lua_tonumber (pLua, -3), lua_tonumber (pLua, -2), lua_tonumber (pLua, -1)));
		}
		else
		{
			throwError ("Function: setColorMaterialColor\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setColorMaterialColor\nParameter was incorrect!\nIt should be: (int) pointer to ColorMaterial, (float) red, (float) green, (float) blue.\nNote that the rgb values should be in the range 0 - 1.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Create lit material using a color
int createColorLitMaterial (lua_State* pLua)
{
	if (lua_isnumber (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		LitMaterial* material = new LitMaterial (glm::vec3 (lua_tonumber (pLua, -4), lua_tonumber (pLua, -3), lua_tonumber (pLua, -2)), lua_tonumber (pLua, -1));
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)material);
	}
	else
	{
		throwError ("Function: litColorMaterial\nParameter was incorrect!\nIt should be: (float) red, (float) green, (float) blue, (float) shininess.\nNote that the rgb values should be in the range 0 - 1.\nShininess should be in the range 0 - 250.");
		lua_settop (pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;//Return pointer to created material
}
//Change lit material's color
int setLitMaterialColor (lua_State* pLua)
{
	if (lua_isinteger (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		LitMaterial* material = (LitMaterial*)lua_tointeger (pLua, -4);
		if (material != nullptr)
		{
			material->SetColor (glm::vec3 (lua_tonumber (pLua, -3), lua_tonumber (pLua, -2), lua_tonumber (pLua, -1)));
		}
		else
		{
			throwError ("Function: setLitMaterialColor\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setLitMaterialColor\nParameter was incorrect!\nIt should be: (int) pointer to LitMaterial, (float) red, (float) green, (float) blue.\nNote that the rgb values should be in the range 0 - 1.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Change lit materíal's shininess
int setLitMaterialShininess (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		LitMaterial* material = (LitMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->SetShininess (lua_tonumber (pLua, -1));
		}
		else
		{
			throwError ("Function: setLitMaterialShininess\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setLitMaterialShininess\nParameter was incorrect!\nIt should be: (int) pointer to LitMaterial, (float) shininess.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Change lit material's specular color
int setLitMaterialSpecularColor (lua_State* pLua)
{
	if (lua_isinteger (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		LitMaterial* material = (LitMaterial*)lua_tointeger (pLua, -4);
		if (material != nullptr)
		{
			material->SetSpecularColor (glm::vec3 (lua_tonumber (pLua, -3), lua_tonumber (pLua, -2), lua_tonumber (pLua, -1)));
		}
		else
		{
			throwError ("Function: setLitMaterialSpecularColor\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setLitMaterialSpecularColor\nParameter was incorrect!\nIt should be: (int) pointer to LitMaterial, (float) red, (float) green, (float) blue.\nNote that the rgb values should be in the range 0 - 1.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Create texture material
int createTextureMaterial (lua_State* pLua)
{
	if (lua_isstring (pLua, -1))
	{
		TextureMaterial* material = new TextureMaterial (lua_tostring(pLua, -1));
		lua_settop(pLua, 0);
		lua_pushinteger (pLua, (int)material);
	}
	else
	{
		throwError ("Function: textureMaterial\nParameter was incorrect!\nIt should be: (string) texture filename.");
		lua_settop (pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;//Return pointer to created material
}
//Change texture material's texture
int setTextureMaterialTexture (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isstring (pLua, -1))
	{
		TextureMaterial* material = (TextureMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->setDiffuseTexture (Texture::load (config::MGE_TEXTURE_PATH+(string)lua_tostring (pLua, -1)));
		}
		else
		{
			throwError ("Function: setTextureMaterialTexture\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTextureMaterialTexture\nParameter was incorrect!\nIt should be: (int) pointer to TextureMaterial, (string) texture filename.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Change texture material's texture scale
int setTextureMaterialScale (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		TextureMaterial* material = (TextureMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->setTextureScale (lua_tonumber (pLua, -1));
		}
		else
		{
			throwError ("Function: setTextureMaterialScale\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTextureMaterialScale\nParameter was incorrect!\nIt should be: (int) pointer to TextureMaterial, (float) texture scale.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Create lit material using a texture
int createTextureLitMaterial (lua_State* pLua)
{
	if (lua_isstring (pLua, -2) && lua_isnumber (pLua, -1))
	{
		LitMaterial* material = new LitMaterial (lua_tostring (pLua, -2), lua_tonumber (pLua, -1));
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)material);
	}
	else
	{
		throwError ("Function: litTextureMaterial\nParameter was incorrect!\nIt should be: (string) texture filename, (float) shininess.\nShininess should be in the range 0 - 250.");
		lua_settop (pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;//Return pointer to created material
}
//Change lit material's texture
int setLitMaterialTexture (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isstring (pLua, -1))
	{
		LitMaterial* material = (LitMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->SetTexture ((string)lua_tostring (pLua, -1));
		}
		else
		{
			throwError ("Function: setLitMaterialTexture\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setLitMaterialTexture\nParameter was incorrect!\nIt should be: (int) pointer to LitMaterial, (string) texture filename.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Change lit material's texture scale
int setLitMaterialScale (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		LitMaterial* material = (LitMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->SetTextureScale (lua_tonumber (pLua, -1));
		}
		else
		{
			throwError ("Function: setLitMaterialScale\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setLitMaterialScale\nParameter was incorrect!\nIt should be: (int) pointer to LitMaterial, (float) texture scale.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Create a terrain material
int createTerrainMaterial (lua_State* pLua)
{
	if (lua_isstring (pLua, -3) && lua_istable (pLua, -2) && lua_isboolean (pLua, -1))
	{
		string splatMapFile = lua_tostring (pLua, -3);
		bool lit = lua_tonumber (pLua, -1);
		vector <string> luaStrings;
		lua_len (pLua, -2);//Get the amount of items in the lua table (value is added to the stack, table is now at -3)
		int tableSize = lua_tointeger (pLua, -1);
		for (int i = 0; i < tableSize; i ++)
		{
			lua_pushinteger (pLua, (i + 1));//Push index we want to access in the table onto the stack (table is now at -(4 + i))
			lua_gettable (pLua, -(4 + i));//Replaces the key on top of the stack with its corresponding value
			luaStrings.push_back (lua_tostring (pLua, -1));//Add the value from the table to the vector
		}
		TerrainMaterial* material = new TerrainMaterial (splatMapFile, luaStrings, lit);
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)material);
	}
	else
	{
		throwError ("Function: terrainMaterial\nParameter was incorrect!\nIt should be: (string) splatmap filename, (table <string>) texture filenames, (bool) lit.");
		lua_settop (pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;//Return pointer to created material
}
//Change terrain material's splat texR
int setTerrainMaterialTexR (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isstring (pLua, -1))
	{
		TerrainMaterial* material = (TerrainMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->SetSplatTexR ((string)lua_tostring (pLua, -1));
		}
		else
		{
			throwError ("Function: setTerrainMaterialTexR\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTerrainMaterialTexR\nParameter was incorrect!\nIt should be: (int) pointer to TerrainMaterial, (string) texture filename.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Change terrain material's texG
int setTerrainMaterialTexG (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isstring (pLua, -1))
	{
		TerrainMaterial* material = (TerrainMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->SetSplatTexG ((string)lua_tostring (pLua, -1));
		}
		else
		{
			throwError ("Function: setTerrainMaterialTexG\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTerrainMaterialTexG\nParameter was incorrect!\nIt should be: (int) pointer to TerrainMaterial, (string) texture filename.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Change terrain material's texB
int setTerrainMaterialTexB (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isstring (pLua, -1))
	{
		TerrainMaterial* material = (TerrainMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->SetSplatTexB ((string)lua_tostring (pLua, -1));
		}
		else
		{
			throwError ("Function: setTerrainMaterialTexB\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTerrainMaterialTexB\nParameter was incorrect!\nIt should be: (int) pointer to TerrainMaterial, (string) texture filename.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Change terrain material's texA
int setTerrainMaterialTexA (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isstring (pLua, -1))
	{
		TerrainMaterial* material = (TerrainMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->SetSplatTexA ((string)lua_tostring (pLua, -1));
		}
		else
		{
			throwError ("Function: setTerrainMaterialTexA\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTerrainMaterialTexA\nParameter was incorrect!\nIt should be: (int) pointer to TerrainMaterial, (string) texture filename.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Change terrain material's texR scale
int setTerrainMaterialTexRScale (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		TerrainMaterial* material = (TerrainMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->SetTexRScale (lua_tonumber (pLua, -1));
		}
		else
		{
			throwError ("Function: setTerrainMaterialTexRScale\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTerrainMaterialTexRScale\nParameter was incorrect!\nIt should be: (int) pointer to TerrainMaterial, (float) texture scale.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Change terrain material's texG scale
int setTerrainMaterialTexGScale (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		TerrainMaterial* material = (TerrainMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->SetTexGScale (lua_tonumber (pLua, -1));
		}
		else
		{
			throwError ("Function: setTerrainMaterialTexGScale\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTerrainMaterialTexGScale\nParameter was incorrect!\nIt should be: (int) pointer to TerrainMaterial, (float) texture scale.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Change terrain material's texB scale
int setTerrainMaterialTexBScale (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		TerrainMaterial* material = (TerrainMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->SetTexBScale (lua_tonumber (pLua, -1));
		}
		else
		{
			throwError ("Function: setTerrainMaterialTexBScale\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTerrainMaterialTexBScale\nParameter was incorrect!\nIt should be: (int) pointer to TerrainMaterial, (float) texture scale.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Change terrain material's texA scale
int setTerrainMaterialTexAScale (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		TerrainMaterial* material = (TerrainMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->SetTexAScale (lua_tonumber (pLua, -1));
		}
		else
		{
			throwError ("Function: setTerrainMaterialTexAScale\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTerrainMaterialTexAScale\nParameter was incorrect!\nIt should be: (int) pointer to TerrainMaterial, (float) texture scale.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Change terrain material's texture scales
int setTerrainMaterialTexScales (lua_State* pLua)
{
	if (lua_isinteger (pLua, -5) && lua_isnumber (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		TerrainMaterial* material = (TerrainMaterial*)lua_tointeger (pLua, -5);
		if (material != nullptr)
		{
			material->SetUVScales (lua_tonumber (pLua, -4), lua_tonumber (pLua, -3), lua_tonumber (pLua, -2), lua_tonumber (pLua, -1));
		}
		else
		{
			throwError ("Function: setTerrainMaterialTexScales\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTerrainMaterialTexScales\nParameter was incorrect!\nIt should be: (int) pointer to TerrainMaterial, (float) texR scale, (float) texG scale, (float) texB scale, (float) texA scale.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Change whether terrain material is lit
int setTerrainMaterialLit (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isboolean (pLua, -1))
	{
		TerrainMaterial* material = (TerrainMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->SetLit (lua_toboolean (pLua, -1));
		}
		else
		{
			throwError ("Function: setTerrainMaterialLit\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTerrainMaterialLit\nParameter was incorrect!\nIt should be: (int) pointer to TerrainMaterial, (bool) lit.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Change terrain material's shininess
int setTerrainMaterialShininess (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		TerrainMaterial* material = (TerrainMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->SetShininess (lua_tonumber (pLua, -1));
		}
		else
		{
			throwError ("Function: setTerrainMaterialShininess\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTerrainMaterialShininess\nParameter was incorrect!\nIt should be: (int) pointer to TerrainMaterial, (float) shininess.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Change terrain material's specular color
int setTerrainMaterialSpecularColor (lua_State* pLua)
{
	if (lua_isinteger (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		TerrainMaterial* material = (TerrainMaterial*)lua_tointeger (pLua, -4);
		if (material != nullptr)
		{
			material->SetSpecularColor (glm::vec3 (lua_tonumber (pLua, -3), lua_tonumber (pLua, -2), lua_tonumber (pLua, -1)));
		}
		else
		{
			throwError ("Function: setTerrainMaterialSpecularColor\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTerrainMaterialSpecularColor\nParameter was incorrect!\nIt should be: (int) pointer to TerrainMaterial, (float) red, (float) green, (float) blue.\nNote that the rgb values should be in the range 0 - 1.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Change terrain material's water index
int setTerrainMaterialWaterIndex (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isinteger (pLua, -1))
	{
		TerrainMaterial* material = (TerrainMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->SetWaterIndex (lua_tointeger (pLua, -1));
		}
		else
		{
			throwError ("Function: setTerrainMaterialWaterIndex\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTerrainMaterialWaterIndex\nParameter was incorrect!\nIt should be: (int) pointer to TerrainMaterial, (int) water index.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Change terrain material's wave height
int setTerrainMaterialWaveHeight (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		TerrainMaterial* material = (TerrainMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->SetWaveHeight (lua_tonumber (pLua, -1));
		}
		else
		{
			throwError ("Function: setTerrainMaterialWaveHeight\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTerrainMaterialWaveHeight\nParameter was incorrect!\nIt should be: (int) pointer to TerrainMaterial, (float) wave height.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Change terrain material's wave count
int setTerrainMaterialWaveCount (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		TerrainMaterial* material = (TerrainMaterial*)lua_tointeger (pLua, -2);
		if (material != nullptr)
		{
			material->SetWaveCount (lua_tonumber (pLua, -1));
		}
		else
		{
			throwError ("Function: setTerrainMaterialWaveCount\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted material!");
		}
	}
	else
	{
		throwError ("Function: setTerrainMaterialWaveCount\nParameter was incorrect!\nIt should be: (int) pointer to TerrainMaterial, (float) wave count.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//////////|	BEHAVIOURS
//Create rotating behaviour
int createRotatingBehaviour (lua_State* pLua)
{
	if (lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		RotatingBehaviour* behaviour = new RotatingBehaviour (glm::radians (lua_tonumber (pLua, -3)), glm::radians (lua_tonumber (pLua, -2)), glm::radians (lua_tonumber (pLua, -1)));
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)behaviour);
	}
	else
	{
		throwError ("Function: rotatingBehaviour\nParameter was incorrect!\nIt should be: (float) x angle, (float) y angle, (float) z angle.");
		lua_settop (pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;//Return pointer to created behaviour
}
//Create keysbehaviour
int createKeysBehaviour (lua_State* pLua)
{
	KeysBehaviour* behaviour = new KeysBehaviour ();
	lua_pushinteger (pLua, (int)behaviour);
	return 1;//Return pointer to created behaviour
}
//////////|	DOOM OBJECTS
//Create doom object
int createDoomObject (lua_State* pLua)
{
	//If the object should be a DoomText
	if (lua_isstring (pLua, -6) && lua_isstring (pLua, -5) && lua_isinteger (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		DoomText* text = new DoomText ((string)lua_tostring (pLua, -6), (string)lua_tostring (pLua, -5), (unsigned int)lua_tointeger (pLua, -4), glm::vec3 (lua_tonumber (pLua, -3), lua_tonumber (pLua, -2), lua_tonumber (pLua, -1)));
		text->setParent (World::singletonInstance);
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)text);
	}
	//		non-colored DoomText
	else if (lua_isstring (pLua, -3) && lua_isstring (pLua, -2) && lua_isinteger (pLua, -1))
	{
		DoomText* text = new DoomText ((string)lua_tostring (pLua, -3), (string)lua_tostring (pLua, -2), (unsigned int)lua_tointeger (pLua, -1), glm::vec3 (1, 1, 1));
		text->setParent (World::singletonInstance);
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)text);
	}
	//If the object should be a DoomAnimation
	else if (lua_isstring (pLua, -4) && lua_isinteger (pLua, -3) && lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		DoomAnimation* anim = new DoomAnimation ((string)lua_tostring (pLua, -4), lua_tointeger (pLua, -3), lua_tointeger (pLua, -2), lua_tonumber (pLua, -1));
		anim->setParent (World::singletonInstance);
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)anim);
	}
	//If the object should be a DoomSprite
	else if (lua_isstring (pLua, -1))
	{
		DoomSprite* image = new DoomSprite ((string)lua_tostring (pLua, -1));
		image->setParent (World::singletonInstance);
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)image);
	}
	else
	{
		throwError ("Function: doomObject\nParameter was incorrect!\nIt should be: (string) filename OR:\n(string) filename, (int) framewidth, (int) frameheight, (float) frame time in seconds/frame.\nNote that the position is relative to the bottom center of the image.");
		lua_settop (pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;//Return the created object
}
//Set a given doom object's click-hitbox
int setClickHitboxByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -5) && lua_isnumber (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		//Get the sprite/animation
		int pointer = lua_tointeger (pLua, -5);
		DoomObject* doom = (DoomObject*)pointer;
		if (doom != nullptr)
		{
			//Change the hitbox
			doom->SetClickHitbox (glm::vec2 (lua_tonumber (pLua, -4), lua_tonumber (pLua, -3)), glm::vec2 (lua_tonumber (pLua, -2), lua_tonumber (pLua, -1)));
		}
		else
		{
			throwError ("Function: setClickHitbox\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: setClickHitbox\nParameter was incorrect!\nIt should be: (int) pointer to doomobject, (float) x offset, (float) y offset, (float) hitbox width, (float) hitbox height.\nNote that these measurements are in pixels.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Check if a given doom object was clicked
int checkDoomButton (lua_State* pLua)
{
	if (lua_isinteger (pLua, -1))
	{
		//Get the sprite/animation
		int pointer = lua_tointeger (pLua, -1);
		lua_settop (pLua, 0);
		DoomObject* doom = (DoomObject*)pointer;
		if (doom != nullptr)
		{
			//Check if the button was clicked
			JCPPEngine::MouseEvent* mouseEvent = LuaParser::singletonInstance->_mouseEvent;
			if (mouseEvent != nullptr && doom->CheckIfClicked (mouseEvent->position ()))
			{
				lua_pushboolean (pLua, true);
			}
			else
			{
				lua_pushboolean (pLua, false);
			}
		}
		else
		{
			throwError ("Function: ifDoomClicked\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
			lua_pushboolean (pLua, false);
		}
	}
	else
	{
		throwError ("Function: ifDoomClicked\nParameter was incorrect!\nIt should be: (int) pointer to the doomsprite.\nThe pointer is returned when you create the object.");
		lua_settop (pLua, 0);
		lua_pushboolean (pLua, false);
	}
	return 1;
}

//////////|	SCENES
//Load a scene from a export-file
int loadScene (lua_State* pLua)
{
	if (lua_isstring (pLua, -1))
	{
		string filepath = "scenes/" + (string)lua_tostring (pLua, -1);
		LuaParser::singletonInstance->_curScene = new GameObject ("Scene", glm::vec3 (0, 0, 0));//All loaded objects will be a child of this one
		LuaParser::singletonInstance->_curScene->setParent (World::singletonInstance);//Scene should be a part of world

		//Load the lua file, and run it once (if there are no errors)
		luaL_loadfile (pLua, filepath.c_str ());
		int result = lua_pcall (pLua, 0, 0, 0);

		//Check for errors
		if (result != LUA_OK)
		{
			if (lua_isstring (pLua, -1))//Write error to command prompt
			{
				cout << "[REFRESH] [ERROR]: " << lua_tostring (pLua, -1) << endl;
				LuaParser::singletonInstance->_curScene = nullptr;
				lua_settop(pLua, 0);
				lua_pushnil (pLua);
				return 1;
			}
			else//If there is no error message, say so
			{
				cout << "[REFRESH] [ERROR]: unknown; there was no error message available." << endl;
				LuaParser::singletonInstance->_curScene = nullptr;
				lua_settop(pLua, 0);
				lua_pushnil (pLua);
				return 1;
			}
		}

		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)LuaParser::singletonInstance->_curScene);
		LuaParser::singletonInstance->_curScene = nullptr;//Stop adding imported objects to this scene
	}
	else
	{
		throwError ("Function: loadScene\nParameter was incorrect!\nIt should be: (string) exported-scene filename.");
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, -1);
	}
	return 1;//Return the id of the scene
}

//Create a gameobject and add it to the world
int createObject (lua_State* pLua)
{
	if (lua_isstring (pLua, -1))
	{
		GameObject* object = new GameObject (lua_tostring (pLua, -1));
		World::singletonInstance->add (object);
		lua_settop (pLua, 0);
		lua_pushinteger (pLua, (int)object);
	}
	else
	{
		throwError ("Function: object\nParameter was incorrect!\nIt should be: (string) mesh-filename.");
		lua_settop (pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;//Return the object created
}
//Remove a gameobject from the world and delete it
int deleteObjectByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -1))
	{
		GameObject* object = ((GameObject*)lua_tointeger (pLua, -1));
		if (object != nullptr)
		{
			object->setParent (nullptr);
			delete object;
		}
	//	else
	//	{
	//		throwError ("Function: delete\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
	//	}
	}
	else
	{
		throwError ("Function: delete\nParameter was incorrect!\nIt should be: (int) pointer to object.\nThe pointer is returned when you create the object.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Set a gameobject's parent
int setParentByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isinteger (pLua, -1))
	{
		GameObject* child = ((GameObject*)lua_tointeger (pLua, -2));
		GameObject* parent = ((GameObject*)lua_tointeger (pLua, -1));
		if (child != nullptr && parent != nullptr)
		{
			child->setParent (parent);
		}
		else
		{
			throwError ("Function: parent\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: parent\nParameter was incorrect!\nIt should be: (int) pointer to the child object, (int) pointer to the parent object.\nThe pointer is returned when you create the object.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Remove a gameobject's parent
int removeParentByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -1))
	{
		GameObject* child = ((GameObject*)lua_tointeger (pLua, -1));
		if (child != nullptr)
		{
			child->setParent (nullptr);
		}
		else
		{
			throwError ("Function: unparent\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: unparent\nParameter was incorrect!\nIt should be: (int) pointer to the child object.\nThe pointer is returned when you create the object.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Translate a gameobject by its pointer
int translateObjectByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		GameObject* object = ((GameObject*)lua_tointeger (pLua, -4));
		if (object != nullptr)
		{
			object->translate (glm::vec3 (lua_tonumber (pLua, -3), lua_tonumber (pLua, -2), lua_tonumber (pLua, -1)));
		}
		else
		{
			throwError ("Function: move\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: move\nParameter was incorrect!\nIt should be: (int) pointer to object, (float) x movement, (float) y movement, (float) z movement.\nThe pointer is returned when you create the object.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Rotate a gameobject by its pointer
int rotateObjectByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		GameObject* object = ((GameObject*)lua_tointeger (pLua, -4));
		if (object != nullptr)
		{
			object->rotate (glm::radians (lua_tonumber (pLua, -2)), glm::vec3 (0, 1, 0));
			object->rotate (glm::radians (lua_tonumber (pLua, -3)), glm::vec3 (1, 0, 0));
			object->rotate (glm::radians (lua_tonumber (pLua, -1)), glm::vec3 (0, 0, 1));
		}
		else
		{
			throwError ("Function: rotate\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: rotate\nParameter was incorrect!\nIt should be: (int) pointer to object, (float) x rotation, (float) y rotation, (float) z rotation.\nThe pointer is returned when you create the object.\nAll rotations are in degrees.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Scale a gameobject by its pointer (uniform)
int scaleObjectByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		GameObject* object = ((GameObject*)lua_tointeger (pLua, -2));
		if (object != nullptr)
		{
			object->scale (lua_tonumber (pLua, -1));
		}
		else
		{
			throwError ("Function: scale\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: scale\nParameter was incorrect!\nIt should be: (int) pointer to object, (float) scale.\nThe pointer is returned when you create the object.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Scale a gameobject by its pointer (non-uniform)
int badScaleObjectByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		GameObject* object = ((GameObject*)lua_tointeger (pLua, -4));
		if (object != nullptr)
		{
			object->badScale (glm::vec3 (lua_tonumber (pLua, -3), lua_tonumber (pLua, -2), lua_tonumber (pLua, -1)));
		}
		else
		{
			throwError ("Function: badScale\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: badScale\nParameter was incorrect!\nIt should be: (int) pointer to object, (float) scale x, (float) scale y, (float) scale z.\nThe pointer is returned when you create the object.");
	}
	lua_settop(pLua, 0);
	return 0;
}
////////////////////|	COLLISIONS
//Add a collider to an object
int addColliderByPointer (lua_State* pLua)
{
	//Check for axis-aligned box collider parameters
	if (lua_isinteger (pLua, -9) && lua_isnumber (pLua, -8) && lua_isnumber (pLua, -7) && lua_isnumber (pLua, -6) && lua_isnumber (pLua, -5) && lua_isnumber (pLua, -4) && lua_isnumber (pLua, -3) && lua_isboolean (pLua, -2) && lua_isboolean (pLua, -1))
	{
		GameObject* object = (GameObject*)lua_tointeger (pLua, -9);
		if (object != nullptr)
		{
			object->addCollider (glm::vec3 (lua_tonumber (pLua, -8), lua_tonumber (pLua, -7), lua_tonumber (pLua, -6)) * 0.5f, glm::vec3 (lua_tonumber (pLua, -5), lua_tonumber (pLua, -4), lua_tonumber (pLua, -3)), lua_toboolean (pLua, -2), lua_toboolean (pLua, -1));
		}
		else
		{
			throwError ("Function: addCollider\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	//Check for sphere collider parameters
	else if (lua_isinteger (pLua, -6) && lua_isnumber (pLua, -5) && lua_isnumber (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isboolean (pLua, -1))
	{
		GameObject* object = (GameObject*)lua_tointeger (pLua, -6);
		if (object != nullptr)
		{
			object->addCollider (lua_tonumber (pLua, -5), glm::vec3 (lua_tonumber (pLua, -4), lua_tonumber (pLua, -3), lua_tonumber (pLua, -2)), lua_toboolean (pLua, -1));
		}
		else
		{
			throwError ("Function: addCollider\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: addCollider\nParameter was incorrect!\nIt should be:\n(int) pointer to object, (float) width, (float) height, (float) depth,\n(float) offset x, (float) offset y, (float) offset z,\n(bool) use outerbox, (bool) mark as static.\nor:\n(int) pointer to object, (float) radius,\n(float) offset x, (float) offset y, (float) offset z,\n(bool) mark as static.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Remove an object's collider
int removeColliderByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -1))
	{
		GameObject* object = (GameObject*)lua_tointeger (pLua, -1);
		if (object != nullptr)
		{
			object->removeCollider ();
		}
		else
		{
			throwError ("Function: removeCollider\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: removeCollider\nParameter was incorrect!\nIt should be: (int) pointer to object.\nThe pointer is returned when you create the object.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Check if two objects collided
int checkCollisionByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -3) && lua_isinteger (pLua, -2) && lua_isboolean (pLua, -1))
	{
		GameObject* object1 = (GameObject*)lua_tointeger (pLua, -3);
		GameObject* object2 = (GameObject*)lua_tointeger (pLua, -2);
		//Check if pointers are valid
		if (object1 != nullptr && object2 != nullptr)
		{
			//Check if both objects have a collider
			Collider* collider1 = object1->getCollider ();
			Collider* collider2 = object2->getCollider ();
			if (collider1 != nullptr && collider2 != nullptr)
			{
				if (collider1->colliderType () == 1 && collider2->colliderType () == 1)
				{
					//AAB | AAB collision
					glm::vec3 coll1Pos = collider1->worldCenterOffset ();
					glm::vec3 coll2Pos = collider2->worldCenterOffset ();
					glm::vec3 coll1Ext = collider1->worldHalfExtents ();
					glm::vec3 coll2Ext = collider2->worldHalfExtents ();
					glm::vec2 coll1Limits [3] =
					{
						glm::vec2 (coll1Pos.x - coll1Ext.x, coll1Pos.x + coll1Ext.x),//x min, x max
						glm::vec2 (coll1Pos.y - coll1Ext.y, coll1Pos.y + coll1Ext.y),//y min, y max
						glm::vec2 (coll1Pos.z - coll1Ext.z, coll1Pos.z + coll1Ext.z)//z min, z max
					};
					glm::vec2 coll2Limits [3] =
					{
						glm::vec2 (coll2Pos.x - coll2Ext.x, coll2Pos.x + coll2Ext.x),//x min, x max
						glm::vec2 (coll2Pos.y - coll2Ext.y, coll2Pos.y + coll2Ext.y),//y min, y max
						glm::vec2 (coll2Pos.z - coll2Ext.z, coll2Pos.z + coll2Ext.z)//z min, z max
					};
					if
					(
						//Check if x locations overlap
						((coll2Limits [0] [0] > coll1Limits [0] [0] && coll2Limits [0] [0] < coll1Limits [0] [1]) || (coll2Limits [0] [1] > coll1Limits [0] [0] && coll2Limits [0] [1] < coll1Limits [0] [1]) || (coll2Limits [0] [0] <= coll1Limits [0] [0] && coll2Limits [0] [1] >= coll1Limits [0] [1])) &&
						//Check if y locations overlap
						((coll2Limits [1] [0] > coll1Limits [1] [0] && coll2Limits [1] [0] < coll1Limits [1] [1]) || (coll2Limits [1] [1] > coll1Limits [1] [0] && coll2Limits [1] [1] < coll1Limits [1] [1]) || (coll2Limits [1] [0] <= coll1Limits [1] [0] && coll2Limits [1] [1] >= coll1Limits [1] [1])) &&
						//Check if z locations overlap
						((coll2Limits [2] [0] > coll1Limits [2] [0] && coll2Limits [2] [0] < coll1Limits [2] [1]) || (coll2Limits [2] [1] > coll1Limits [2] [0] && coll2Limits [2] [1] < coll1Limits [2] [1]) || (coll2Limits [2] [0] <= coll1Limits [2] [0] && coll2Limits [2] [1] >= coll1Limits [2] [1]))
					)
					{
						if (lua_toboolean (pLua, -1) && !(collider1->isStatic () && collider2->isStatic ()))
						{
							//Resolve collision
							glm::vec3 delta = (coll2Pos - coll1Pos);//delta goes from object1 to object2
							if (glm::length (delta) < 0.0001f)
							{
								delta = glm::vec3 (0, 1, 0);
							}
							glm::vec3 maxDelta = glm::vec3 ((coll1Ext.x + coll2Ext.x) * ((delta.x < 0) ? -1 : 1), (coll1Ext.y + coll2Ext.y) * ((delta.y < 0) ? -1 : 1), (coll1Ext.z + coll2Ext.z) * ((delta.z < 0) ? -1 : 1));
							glm::vec3 absDelta = glm::vec3 (abs (delta.x), abs (delta.y), abs (delta.z));
							//Calculate the contribution of each of the boxes surfaces to the collision. Since it's a box, it can't be a gradual transition.
							glm::vec3 contribFactor;
							contribFactor.x = (absDelta.x < absDelta.y || absDelta.x < absDelta.z) ? 0 : 1;
							contribFactor.y = (absDelta.y < absDelta.x || absDelta.y < absDelta.z) ? 0 : 1;
							contribFactor.z = (absDelta.z < absDelta.x || absDelta.z < absDelta.y) ? 0 : 1;
							glm::vec3 idealDelta = maxDelta * contribFactor;
							delta *= contribFactor;
							if (!collider1->isStatic () && !collider2->isStatic ())
							{
								//Move both objects back
								glm::vec3 adjustment = (idealDelta - delta) * 0.5f;
								object1->translate (-adjustment);
								object2->translate (adjustment);
							}
							else
							{
								//Move the non-static object back
								if (!collider1->isStatic ())
								{
									object1->translate (-(idealDelta - delta));
								}
								else
								{
									object2->translate (idealDelta - delta);
								}
							}
							object1->updateCollider ();
							object2->updateCollider ();
						}
						lua_settop(pLua, 0);
						lua_pushboolean (pLua, true);
					}
					else
					{
						lua_settop(pLua, 0);
						lua_pushboolean (pLua, false);
					}
				}
				else if (collider1->colliderType () == 2 && collider2->colliderType () == 2)
				{
					//S | S collision
					glm::vec3 coll1Pos = collider1->worldCenterOffset ();
					glm::vec3 coll2Pos = collider2->worldCenterOffset ();
					float idealDist = collider1->worldRadius () + collider2->worldRadius ();
					//Check the distance between the spheres
					glm::vec3 delta = (coll2Pos - coll1Pos);//delta goes from object1 to object2
					if (glm::length (delta) < 0.00001f)
					{
						delta = glm::vec3 (0, 1, 0);
					}
					if (glm::length (delta) < idealDist)
					{
						if (lua_toboolean (pLua, -1) && !(collider1->isStatic () && collider2->isStatic ()))
						{
							//Resolve collision
							glm::vec3 idealDelta = glm::normalize (delta) * idealDist;
							if (!collider1->isStatic () && !collider2->isStatic ())
							{
								//Move both objects back
								glm::vec3 adjustment = (idealDelta - delta) * 0.5f;
								object1->translate (-adjustment);
								object2->translate (adjustment);
							}
							else
							{
								//Move the non-static object back
								if (!collider1->isStatic ())
								{
									object1->translate (-(idealDelta - delta));
								}
								else
								{
									object2->translate (idealDelta - delta);
								}
							}
							object1->updateCollider ();
							object2->updateCollider ();
						}
						lua_settop(pLua, 0);
						lua_pushboolean (pLua, true);
					}
					else
					{
						lua_settop(pLua, 0);
						lua_pushboolean (pLua, false);
					}
				}
				else
				{
					if (collider1->colliderType () == 1)
					{
						Collider* coll = collider2;
						GameObject* obj = object2;
						collider2 = collider1;
						object2 = object1;
						collider1 = coll;
						object1 = obj;
					}
					//S | AAB collision
					glm::vec3 coll1Pos = collider1->worldCenterOffset ();
					glm::vec3 coll2Pos = collider2->worldCenterOffset ();
					float coll1Rad = collider1->worldRadius ();
					glm::vec3 coll2Ext = collider2->worldHalfExtents ();
					glm::vec2 coll2Limits [3] =
					{
						glm::vec2 (coll2Pos.x - coll2Ext.x, coll2Pos.x + coll2Ext.x),//x min, x max
						glm::vec2 (coll2Pos.y - coll2Ext.y, coll2Pos.y + coll2Ext.y),//y min, y max
						glm::vec2 (coll2Pos.z - coll2Ext.z, coll2Pos.z + coll2Ext.z)//z min, z max
					};
					//Get the point on the sphere closest to the center of the box
					glm::vec3 delta = (coll2Pos - coll1Pos);//delta goes from object1 to object2
					if (glm::length (delta) < 0.00001f)
					{
						delta = glm::vec3 (0, 1, 0);
					}
					if (glm::length (delta) <= coll1Rad)
					{
						if (lua_toboolean (pLua, -1) && !(collider1->isStatic () && collider2->isStatic ()))
						{
							//Resolve collision
							glm::vec3 idealDelta = glm::vec3 (coll1Rad + coll2Ext.x, coll1Rad + coll2Ext.y, coll1Rad + coll2Ext.z) * glm::normalize (delta);
							if (!collider1->isStatic () && !collider2->isStatic ())
							{
								//Move both objects back
								glm::vec3 adjustment = (idealDelta - delta) * 0.5f;
								object1->translate (-adjustment);
								object2->translate (adjustment);
							}
							else
							{
								//Move the non-static object back
								if (!collider1->isStatic ())
								{
									object1->translate (-(idealDelta - delta));
								}
								else
								{
									object2->translate (idealDelta - delta);
								}
							}
							object1->updateCollider ();
							object2->updateCollider ();
						}
						lua_settop(pLua, 0);
						lua_pushboolean (pLua, true);
						return 1;
					}
					glm::vec3 coll1Closest = coll1Pos + (glm::normalize (delta) * coll1Rad);
					//Check if the point is within the box
					if
					(
						//Check if x locations overlap
						(coll1Closest.x > coll2Limits [0] [0] && coll1Closest.x < coll2Limits [0] [1]) &&
						//Check if y locations overlap
						(coll1Closest.y > coll2Limits [1] [0] && coll1Closest.y < coll2Limits [1] [1]) &&
						//Check if z locations overlap
						(coll1Closest.z > coll2Limits [2] [0] && coll1Closest.z < coll2Limits [2] [1])
					)
					{
						if (lua_toboolean (pLua, -1) && !(collider1->isStatic () && collider2->isStatic ()))
						{
							//Resolve collision
							glm::vec3 idealDelta = glm::vec3 (coll1Rad + coll2Ext.x, coll1Rad + coll2Ext.y, coll1Rad + coll2Ext.z) * glm::normalize (delta);
							if (!collider1->isStatic () && !collider2->isStatic ())
							{
								//Move both objects back
								glm::vec3 adjustment = (idealDelta - delta) * 0.5f;
								object1->translate (-adjustment);
								object2->translate (adjustment);
							}
							else
							{
								//Move the non-static object back
								if (!collider1->isStatic ())
								{
									object1->translate (-(idealDelta - delta));
								}
								else
								{
									object2->translate (idealDelta - delta);
								}
							}
							object1->updateCollider ();
							object2->updateCollider ();
						}
						lua_settop(pLua, 0);
						lua_pushboolean (pLua, true);
					}
					else
					{
						lua_settop(pLua, 0);
						lua_pushboolean (pLua, false);
					}
				}
			}
			else
			{
				throwError ("Function: ifCollide\nParameter was incorrect!\nOne or both objects did not have a collider!");
				lua_settop(pLua, 0);
				lua_pushboolean (pLua, false);
			}
		}
		else
		{
			throwError ("Function: ifCollide\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
			lua_settop(pLua, 0);
			lua_pushboolean (pLua, false);
		}
	}
	else
	{
		throwError ("Function: ifCollide\nParameter was incorrect!\nIt should be: (int) pointer to object 1, (int) pointer to object 2, (bool) resolve collision.");
		lua_settop(pLua, 0);
		lua_pushboolean (pLua, false);
	}
	return 1;//Return whether a collision happened.
}
////////////////////|	GETTERS/SETTERS
//Getting an object's local position
int getLocalPositionByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -1))
	{
		GameObject* object = (GameObject*)(lua_tointeger (pLua, -1));
		lua_settop(pLua, 0);
		if (object != nullptr)
		{
			glm::vec3 localPos = object->getLocalPosition ();
			lua_newtable (pLua);//Creating the lua table (table now at -1)
			lua_pushstring (pLua, "x");//Push the key onto the stack (table now at -2)
			lua_pushnumber (pLua, localPos.x);//Push the value onto the stack (table now at -3)
			lua_settable (pLua, -3);//Put the key-value pair in the table (table now at -1)
			lua_pushstring (pLua, "y");//Push the key onto the stack (table now at -2)
			lua_pushnumber (pLua, localPos.y);//Push the value onto the stack (table now at -3)
			lua_settable (pLua, -3);//Put the key-value pair in the table (table now at -1)
			lua_pushstring (pLua, "z");//Push the key onto the stack (table now at -2)
			lua_pushnumber (pLua, localPos.z);//Push the value onto the stack (table now at -3)
			lua_settable (pLua, -3);//Put the key-value pair in the table
		}
		else
		{
			throwError ("Function: getLocalPosition\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
			lua_pushnil (pLua);
		}
	}
	else
	{
		throwError ("Function: getLocalPosition\nParameter was incorrect!\nIt should be: (int) pointer to object.");
		lua_pushnil (pLua);
	}
	return 1;//return the position as a lua table
}
//Setting an object's local position
int setLocalPositionByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		GameObject* object = (GameObject*)(lua_tointeger (pLua, -1));
		if (object != nullptr)
		{
			object->setLocalPosition (glm::vec3 (lua_tonumber (pLua, -3), lua_tonumber (pLua, -2), lua_tonumber (pLua, -1)));
		}
		else
		{
			throwError ("Function: setLocalPosition\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: setLocalPosition\nParameter was incorrect!\nIt should be: (int) pointer to object, (float) x, (float) y, (float) z.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Getting an object's world position
int getWorldPositionByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -1))
	{
		GameObject* object = (GameObject*)(lua_tointeger (pLua, -1));
		lua_settop(pLua, 0);
		if (object != nullptr)
		{
			glm::vec3 worldPos = object->getWorldPosition ();
			lua_newtable (pLua);//Creating the lua table (table now at -1)
			lua_pushstring (pLua, "x");//Push the key onto the stack (table now at -2)
			lua_pushnumber (pLua, worldPos.x);//Push the value onto the stack (table now at -3)
			lua_settable (pLua, -3);//Put the key-value pair in the table (table now at -1)
			lua_pushstring (pLua, "y");//Push the key onto the stack (table now at -2)
			lua_pushnumber (pLua, worldPos.y);//Push the value onto the stack (table now at -3)
			lua_settable (pLua, -3);//Put the key-value pair in the table (table now at -1)
			lua_pushstring (pLua, "z");//Push the key onto the stack (table now at -2)
			lua_pushnumber (pLua, worldPos.z);//Push the value onto the stack (table now at -3)
			lua_settable (pLua, -3);//Put the key-value pair in the table
		}
		else
		{
			throwError ("Function: getWorldPosition\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
			lua_pushnil (pLua);
		}
	}
	else
	{
		throwError ("Function: getWorldPosition\nParameter was incorrect!\nIt should be: (int) pointer to object.");
		lua_pushnil (pLua);
	}
	return 1;//return the position as a lua table
}
//Setting an object's world position
int setWorldPositionByPointer (lua_State* pLua)
{
	if (lua_isinteger (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		GameObject* object = (GameObject*)(lua_tointeger (pLua, -1));
		if (object != nullptr)
		{
			object->setWorldPosition (glm::vec3 (lua_tonumber (pLua, -3), lua_tonumber (pLua, -2), lua_tonumber (pLua, -1)));
		}
		else
		{
			throwError ("Function: setWorldPosition\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted object!");
		}
	}
	else
	{
		throwError ("Function: setWorldPosition\nParameter was incorrect!\nIt should be: (int) pointer to object, (float) x, (float) y, (float) z.");
	}
	lua_settop(pLua, 0);
	return 0;
}

////////////////////|	LIGHTS
//Set the ambient light
int setAmbientLight (lua_State* pLua)
{
	if (lua_istable (pLua, -2) && lua_isnumber (pLua, -1))
	{
		float intensity = lua_tonumber (pLua, -1);
		glm::vec3 luaColor;
		//Read color values from table
		lua_pushstring (pLua, "r");
		lua_gettable (pLua, -3);
		luaColor.x = lua_tonumber (pLua, -1);
		lua_pushstring (pLua, "g");
		lua_gettable (pLua, -4);
		luaColor.y = lua_tonumber (pLua, -1);
		lua_pushstring (pLua, "b");
		lua_gettable (pLua, -5);
		luaColor.z = lua_tonumber (pLua, -1);

		ShaderDataUtil::SetAmbientLight (luaColor, intensity);
	}
	else if (lua_isnumber (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		ShaderDataUtil::SetAmbientLight (glm::vec3 (lua_tonumber (pLua, -4), lua_tonumber (pLua, -3), lua_tonumber (pLua, -2)), lua_tonumber (pLua, -1));
	}
	else
	{
		throwError ("Function: ambientLight\nParameter was incorrect!\nIt should be: (float) red, (float) green, (float) blue, (float) intensity.\nNote that the rgb values should be in the range 0 - 1.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Create directional light, and add it to the world
int createDirectionalLight (lua_State* pLua)
{
	if (lua_isnumber (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		Light* light = new Light (glm::vec3 (lua_tonumber (pLua, -4), lua_tonumber (pLua, -3), lua_tonumber (pLua, -2)), lua_tonumber (pLua, -1));
		World::singletonInstance->add (light);
		lua_settop(pLua, 0);
		lua_pushinteger (pLua, (int)light);
	}
	else
	{
		throwError ("Function: directionalLight\nParameter was incorrect!\nIt should be: (float) red, (float) green, (float) blue, (float) intensity.\nNote that the rgb values should be in the range 0 - 1.");
		lua_settop(pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;//Return the object created
}
//Create point light, and add it to the world
int createPointLight (lua_State* pLua)
{
	if (lua_isnumber (pLua, -5) && lua_isnumber (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		float linearFalloff = lua_tonumber (pLua, -1);
		float quadraticFalloff = 1 - linearFalloff;
		Light* light = new Light (glm::vec3 (lua_tonumber (pLua, -5), lua_tonumber (pLua, -4), lua_tonumber (pLua, -3)), lua_tonumber (pLua, -2), glm::vec2 (linearFalloff, quadraticFalloff));
		World::singletonInstance->add (light);
		lua_settop(pLua, 0);
		lua_pushinteger (pLua, (int)light);
	}
	else
	{
		throwError ("Function: pointLight\nParameter was incorrect!\nIt should be: (float) red, (float) green, (float) blue, (float) intensity, (float) linearFalloff.\nNote that the rgb and falloff values should be in the range 0 - 1.");
		lua_settop(pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;//Return the object created
}
//Create spot light, and add it to the world
int createSpotLight (lua_State* pLua)
{
	if (lua_isnumber (pLua, -6) && lua_isnumber (pLua, -5) && lua_isnumber (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		float linearFalloff = lua_tonumber (pLua, -1);
		float quadraticFalloff = 1 - linearFalloff;
		Light* light = new Light (glm::vec3 (lua_tonumber (pLua, -6), lua_tonumber (pLua, -5), lua_tonumber (pLua, -4)), lua_tonumber (pLua, -3), glm::radians (lua_tonumber (pLua, -2)), glm::vec2 (linearFalloff, quadraticFalloff));
		World::singletonInstance->add (light);
		lua_settop(pLua, 0);
		lua_pushinteger (pLua, (int)light);
	}
	else
	{
		throwError ("Function: spotLight\nParameter was incorrect!\nIt should be: (float) red, (float) green, (float) blue, (float) intensity, (float) angle, (float) linear falloff.\nNote that the rgb and falloff values should be in the range 0 - 1.");
		lua_settop(pLua, 0);
		lua_pushnil (pLua);
	}
	return 1;//Return the object created
}
//Set light color
int setLightColor (lua_State* pLua)
{
	if (lua_isinteger (pLua, -4) && lua_isnumber (pLua, -3) && lua_isnumber (pLua, -2) && lua_isnumber (pLua, -1))
	{
		Light* light = (Light*)lua_tointeger (pLua, -4);
		if (light != nullptr)
		{
			light->SetColor (glm::vec3 (lua_tonumber (pLua, -3), lua_tonumber (pLua, -2), lua_tonumber (pLua, -1)));
		}
		else
		{
			throwError ("Function: setLightColor\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted light!");
		}
	}
	else
	{
		throwError ("Function: setLightColor\nParameter was incorrect!\nIt should be: (int) pointer to light, (float) red, (float) green, (float) blue.\nNote that the rgb values should be in the range 0 - 1.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Set light intensity
int setLightIntensity (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		Light* light = (Light*)lua_tointeger (pLua, -2);
		if (light != nullptr)
		{
			light->SetIntensity (lua_tonumber (pLua, -1));
		}
		else
		{
			throwError ("Function: setLightIntensity\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted light!");
		}
	}
	else
	{
		throwError ("Function: setLightIntensity\nParameter was incorrect!\nIt should be: (int) pointer to light, (float) intensity.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Set light falloff
int setLightFalloff (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		Light* light = (Light*)lua_tointeger (pLua, -2);
		if (light != nullptr)
		{
			float linearFalloff = lua_tonumber (pLua, -1);
			float quadraticFalloff = 1 - linearFalloff;
			light->SetFalloff (glm::vec2 (linearFalloff, quadraticFalloff));
		}
		else
		{
			throwError ("Function: setLightFalloff\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted light!");
		}
	}
	else
	{
		throwError ("Function: setLightFalloff\nParameter was incorrect!\nIt should be: (int) pointer to light, (float) linear falloff. Note that linear falloff should be in the range 0 - 1.");
	}
	lua_settop (pLua, 0);
	return 0;
}
//Set light angle
int setLightAngle (lua_State* pLua)
{
	if (lua_isinteger (pLua, -2) && lua_isnumber (pLua, -1))
	{
		Light* light = (Light*)lua_tointeger (pLua, -2);
		if (light != nullptr)
		{
			light->SetAngle (glm::radians (lua_tonumber (pLua, -1)));
		}
		else
		{
			throwError ("Function: setLightAngle\nParameter was incorrect!\nPlease make sure you don't modify the pointer, or attempt to use a deleted light!");
		}
	}
	else
	{
		throwError ("Function: setLightAngle\nParameter was incorrect!\nIt should be: (int) pointer to light, (float) angle.");
	}
	lua_settop (pLua, 0);
	return 0;
}


////////////////////|	DIALOG
//Get whether the npc is talking with the player
int isTalking (lua_State* pLua)
{
	if (Npc::singletonInstance != nullptr)
	{
		lua_pushboolean (pLua, Npc::singletonInstance->talking);
	}
	else
	{
		lua_pushboolean (pLua, false);
	}
	return 1;
}
//Get the number of times the player has talked to the npc about the current quest
int questTalks (lua_State* pLua)
{
	if (Npc::singletonInstance != nullptr)
	{
		lua_pushinteger (pLua, Npc::singletonInstance->questTalks);
	}
	else
	{
		lua_pushinteger (pLua, -1);
	}
	return 1;
}
//Get the number of completed quests
int completedQuests (lua_State* pLua)
{
	if (Npc::singletonInstance != nullptr)
	{
		lua_pushinteger (pLua, Npc::singletonInstance->completedQuests);
	}
	else
	{
		lua_pushinteger (pLua, -1);
	}
	return 1;
}
//Get wheter the talk key was pressed during this frame
int waitForKeyPress (lua_State* pLua)
{
	LuaParser::singletonInstance->_yieldTimer = -1;
	lua_settop (pLua, 0);
	return lua_yieldk (pLua, 0, 0, 0);
}
//Lock/unlock player movement
int lockPlayer (lua_State* pLua)
{
	if (lua_isboolean (pLua, -1) && Player::singletonInstance != nullptr)
	{
		Player::singletonInstance->blockMovement = lua_toboolean (pLua, -1);
	}
	return 0;
}
//Get whether the player has a certain Item
int checkInventory (lua_State* pLua)
{
	if (lua_isstring (pLua, -1) && Player::singletonInstance != nullptr)
	{
		bool itemFound = Player::singletonInstance->hasItem (lua_tostring (pLua, -1));
		lua_settop (pLua, 0);
		lua_pushboolean (pLua, itemFound);
	}
	else
	{
		lua_settop (pLua, 0);
		lua_pushboolean (pLua, false);
	}
	return 1;
}
//Give the player a quest. Before this quest can be completed, all previous quests have to be completed first.
int giveQuest (lua_State* pLua)
{
	if (Npc::singletonInstance != nullptr && lua_isstring (pLua, -2) && lua_isnumber (pLua, -1))
	{
		Npc::singletonInstance->addQuest (new Quest ("", lua_tostring (pLua, -2), lua_tonumber (pLua, -1)));
	}
	lua_settop (pLua, 0);
	return 0;
}
//Show dialog (even though player may not be near)
int showDialog (lua_State* pLua)
{
	if (lua_isstring (pLua, -1) && Npc::singletonInstance != nullptr)
	{
		if (!Npc::singletonInstance->talking)
		{
			Npc::singletonInstance->displayDialog (lua_tostring (pLua, -1));
		}
		else
		{
			Npc::singletonInstance->updateDialog (lua_tostring (pLua, -1));
		}
	}
	lua_settop (pLua, 0);
	return 0;
}
//Update the diplayed dialog
int updateDialog (lua_State* pLua)
{
	if (lua_isstring (pLua, -1) && Npc::singletonInstance != nullptr)
	{
		if (Npc::singletonInstance->talking)
		{
			Npc::singletonInstance->updateDialog (lua_tostring (pLua, -1));
		}
	}
	lua_settop (pLua, 0);
	return 0;
}
//Stop displaying dialog
int stopDialog (lua_State* pLua)
{
	if (Npc::singletonInstance != nullptr)
	{
		Npc::singletonInstance->stopDialog ();
	}
	return 0;
}
//Check if the current level is the hub
int isHub (lua_State* pLua)
{
	lua_settop (pLua, 0);
	lua_pushboolean (pLua, Level::singletonInstance->isHub ());
	return 1;
}
//Change the level key
int setLevelKey (lua_State* pLua)
{
	if (lua_isinteger (pLua, -1))
	{
		Level::singletonInstance->levelKey = lua_tointeger (pLua, -1);
	}
	return 0;
}
//Load the boss level
int loadBossLevel (lua_State* pLua)
{
	Level::singletonInstance->loadLevel (Level::singletonInstance->_bossLevelNumber);
	return 0;
}


////////////////////|	FLOW CONTROL
//Set which lua function will be updated
int setUpdatingFunction (lua_State* pLua)
{
	if (lua_isstring (pLua, -1))
	{
		LuaParser::singletonInstance->_currentFunction = (string)lua_tostring (pLua, -1);
	}
	else
	{
		throwError ("Function: update\nParameter was incorrect!\nIt should be: (string) functionname.\nPlease check if the function actually exists or if you made a typo.");
	}
	lua_settop(pLua, 0);
	return 0;
}
//Get the time since this program started (in seconds)
int getTimeSinceProgramStart (lua_State* pLua)
{
	lua_pushnumber (pLua, AbstractGame::Time ());
	return 1;
}
//Pause the lua program
int pauseLua (lua_State* pLua)
{
	if (lua_isnumber (pLua, -1))
	{
		LuaParser::singletonInstance->_yieldTimer = lua_tonumber (pLua, -1);
		lua_settop (pLua, 0);
		return lua_yieldk (pLua, 0, 0, 0);
	}
	else
	{
		lua_settop (pLua, 0);
		throwError ("Function: wait\nParameter was incorrect!\nIt should be: (float) seconds to wait.");
		return 0;
	}
}
////////////////////|	DEBUGGING
//Print a string
int printString (lua_State* pLua)
{
	cout << lua_tostring (pLua, -1) << endl;
	lua_settop(pLua, 0);
	return 0;
}


//////////////////////////////|	INSTANCE CONTROL
////////////////////|	Create custom 'libraries'
//Game lib
int open_gamelib (lua_State* pLua)
{
	lua_settop(pLua, 0);
	lua_newtable (pLua);//Creating the lua table (table now at -1)
	lua_pushstring (pLua, "time");//Push the key onto the stack (table now at -2)
	lua_pushcfunction (pLua, &getTimeSinceProgramStart);//Push the value onto the stack (table now at -3)
	lua_settable (pLua, -3);//Put the key-value pair in the table (table now at -1)
	lua_pushstring (pLua, "setAmbientLight");//Push the key onto the stack (table now at -2)
	lua_pushcfunction (pLua, &setAmbientLight);//Push the value onto the stack (table now at -3)
	lua_settable (pLua, -3);//Put the key-value pair in the table (table now at -1)
	lua_setglobal (pLua, "Game");
	return 1;
}
//Lua lib
int open_lualib (lua_State* pLua)
{
	lua_settop(pLua, 0);
	lua_newtable (pLua);
	lua_pushstring (pLua, "wait");
	lua_pushcfunction (pLua, &pauseLua);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "update");
	lua_pushcfunction (pLua, &setUpdatingFunction);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "image");
	lua_pushcfunction (pLua, &createImage);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "ifClicked");
	lua_pushcfunction (pLua, &checkButton);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "text");
	lua_pushcfunction (pLua, &createText);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "drawTable");
	lua_pushcfunction (pLua, &putTableInDrawBuffer);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "sound");
	lua_pushcfunction (pLua, &playSound);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "soundLoop");
	lua_pushcfunction (pLua, &playLoopingSound);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "stopLoop");
	lua_pushcfunction (pLua, &stopLoopingSound);
	lua_settable (pLua, -3);
	lua_setglobal (pLua, "Lua");//Name the newly made library
	return 1;
}
//Hidden lib
int open_hiddenlib (lua_State* pLua)
{
	lua_settop(pLua, 0);
	lua_newtable (pLua);
	lua_pushstring (pLua, "draw");
	lua_pushcfunction (pLua, &putInDrawBuffer);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "deleteDrawable");
	lua_pushcfunction (pLua, &deleteDrawable);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "loadScene");
	lua_pushcfunction (pLua, &loadScene);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "object");
	lua_pushcfunction (pLua, &createObject);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "delete");
	lua_pushcfunction (pLua, &deleteObjectByPointer);
	lua_settable (pLua, -3);
	//Getters
	lua_pushstring (pLua, "getDrawablePosition");
	lua_pushcfunction (pLua, &getDrawablePosition);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "getLocalPosition");
	lua_pushcfunction (pLua, &getLocalPositionByPointer);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "getWorldPosition");
	lua_pushcfunction (pLua, &getWorldPositionByPointer);
	lua_settable (pLua, -3);
	//Setters
	lua_pushstring (pLua, "setDrawablePosition");
	lua_pushcfunction (pLua, &setDrawablePosition);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setLocalPosition");
	lua_pushcfunction (pLua, &setLocalPositionByPointer);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setWorldPosition");
	lua_pushcfunction (pLua, &setWorldPositionByPointer);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setMesh");
	lua_pushcfunction (pLua, &setMeshByPointer);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setMaterial");
	lua_pushcfunction (pLua, &setMaterialByPointer);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setBehaviour");
	lua_pushcfunction (pLua, &setBehaviourByPointer);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "parent");
	lua_pushcfunction (pLua, &setParentByPointer);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "unparent");
	lua_pushcfunction (pLua, &removeParentByPointer);
	lua_settable (pLua, -3);
	//Transformations
	lua_pushstring (pLua, "moveDrawable");
	lua_pushcfunction (pLua, &moveDrawable);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "rotateDrawable");
	lua_pushcfunction (pLua, &rotateDrawable);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "scaleDrawable");
	lua_pushcfunction (pLua, &scaleDrawable);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "move");
	lua_pushcfunction (pLua, &translateObjectByPointer);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "rotate");
	lua_pushcfunction (pLua, &rotateObjectByPointer);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "scale");
	lua_pushcfunction (pLua, &scaleObjectByPointer);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "badScale");
	lua_pushcfunction (pLua, &badScaleObjectByPointer);
	lua_settable (pLua, -3);
	//Collisions
	lua_pushstring (pLua, "addCollider");
	lua_pushcfunction (pLua, &addColliderByPointer);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "removeCollider");
	lua_pushcfunction (pLua, &removeColliderByPointer);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "ifCollide");
	lua_pushcfunction (pLua, &checkCollisionByPointer);
	lua_settable (pLua, -3);
	//Light
	lua_pushstring (pLua, "directionalLight");
	lua_pushcfunction (pLua, &createDirectionalLight);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "pointLight");
	lua_pushcfunction (pLua, &createPointLight);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "spotLight");
	lua_pushcfunction (pLua, &createSpotLight);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setLightColor");
	lua_pushcfunction (pLua, &setLightColor);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setLightIntensity");
	lua_pushcfunction (pLua, &setLightIntensity);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setLightFalloff");
	lua_pushcfunction (pLua, &setLightFalloff);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setLightAngle");
	lua_pushcfunction (pLua, &setLightAngle);
	lua_settable (pLua, -3);
	//Meshes
	lua_pushstring (pLua, "mesh");
	lua_pushcfunction (pLua, &createMesh);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "terrainmesh");
	lua_pushcfunction (pLua, &createTerrainMesh);
	lua_settable (pLua, -3);
	//Materials
	lua_pushstring (pLua, "colorMaterial");
	lua_pushcfunction (pLua, &createColorMaterial);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setColorMaterialColor");
	lua_pushcfunction (pLua, &setColorMaterialColor);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "litColorMaterial");
	lua_pushcfunction (pLua, &createColorLitMaterial);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setLitMaterialColor");
	lua_pushcfunction (pLua, &setLitMaterialColor);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setLitMaterialShininess");
	lua_pushcfunction (pLua, &setLitMaterialShininess);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setLitMaterialSpecularColor");
	lua_pushcfunction (pLua, &setLitMaterialSpecularColor);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "textureMaterial");
	lua_pushcfunction (pLua, &createTextureMaterial);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTextureMaterialTexture");
	lua_pushcfunction (pLua, &setTextureMaterialTexture);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTextureMaterialScale");
	lua_pushcfunction (pLua, &setTextureMaterialScale);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "litTextureMaterial");
	lua_pushcfunction (pLua, &createTextureLitMaterial);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setLitMaterialTexture");
	lua_pushcfunction (pLua, &setLitMaterialTexture);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setLitMaterialScale");
	lua_pushcfunction (pLua, &setLitMaterialScale);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "terrainMaterial");
	lua_pushcfunction (pLua, &createTerrainMaterial);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTerrainMaterialTexR");
	lua_pushcfunction (pLua, &setTerrainMaterialTexR);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTerrainMaterialTexRScale");
	lua_pushcfunction (pLua, &setTerrainMaterialTexRScale);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTerrainMaterialTexG");
	lua_pushcfunction (pLua, &setTerrainMaterialTexG);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTerrainMaterialTexGScale");
	lua_pushcfunction (pLua, &setTerrainMaterialTexGScale);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTerrainMaterialTexB");
	lua_pushcfunction (pLua, &setTerrainMaterialTexB);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTerrainMaterialTexBScale");
	lua_pushcfunction (pLua, &setTerrainMaterialTexBScale);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTerrainMaterialTexA");
	lua_pushcfunction (pLua, &setTerrainMaterialTexA);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTerrainMaterialTexAScale");
	lua_pushcfunction (pLua, &setTerrainMaterialTexAScale);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTerrainMaterialTexScales");
	lua_pushcfunction (pLua, &setTerrainMaterialTexScales);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTerrainMaterialLit");
	lua_pushcfunction (pLua, &setTerrainMaterialLit);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTerrainMaterialShininess");
	lua_pushcfunction (pLua, &setTerrainMaterialShininess);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTerrainMaterialSpecularColor");
	lua_pushcfunction (pLua, &setTerrainMaterialSpecularColor);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTerrainMaterialWaterIndex");
	lua_pushcfunction (pLua, &setTerrainMaterialWaterIndex);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTerrainMaterialWaveHeight");
	lua_pushcfunction (pLua, &setTerrainMaterialWaveHeight);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setTerrainMaterialWaveCount");
	lua_pushcfunction (pLua, &setTerrainMaterialWaveCount);
	lua_settable (pLua, -3);
	//Behaviours
	lua_pushstring (pLua, "rotatingBehaviour");
	lua_pushcfunction (pLua, &createRotatingBehaviour);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "keysBehaviour");
	lua_pushcfunction (pLua, &createKeysBehaviour);
	lua_settable (pLua, -3);
	//Doom objects
	lua_pushstring (pLua, "doomObject");
	lua_pushcfunction (pLua, &createDoomObject);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setClickHitbox");
	lua_pushcfunction (pLua, &setClickHitboxByPointer);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "ifDoomClicked");
	lua_pushcfunction (pLua, &checkDoomButton);
	lua_settable (pLua, -3);
	lua_setglobal (pLua, "Hidden");//Name the newly made library
	return 1;
}
//Import lib
int open_importlib (lua_State* pLua)
{
	lua_settop(pLua, 0);
	lua_newtable (pLua);
	lua_pushstring (pLua, "UnityCamera");
	lua_pushcfunction (pLua, &importUnityCamera);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "UnityLight");
	lua_pushcfunction (pLua, &importUnityLight);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "UnityObject");
	lua_pushcfunction (pLua, &importUnityObject);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "RotatingBehaviour");
	lua_pushcfunction (pLua, &createRotatingBehaviour);
	lua_settable (pLua, -3);
	lua_setglobal (pLua, "Import");//Name the newly made library
	return 1;
}
//Npc lib
int open_npclib (lua_State* pLua)
{
	lua_settop (pLua, 0);
	lua_newtable (pLua);
	lua_pushstring (pLua, "isTalking");
	lua_pushcfunction (pLua, &isTalking);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "showDialog");
	lua_pushcfunction (pLua, &showDialog);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "updateDialog");
	lua_pushcfunction (pLua, &updateDialog);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "stopDialog");
	lua_pushcfunction (pLua, &stopDialog);
	lua_settable (pLua, -3);
	lua_setglobal (pLua, "Npc");//Name the newly made library
	return 1;
}
//Player lib
int open_playerlib (lua_State* pLua)
{
	lua_settop (pLua, 0);
	lua_newtable (pLua);
	lua_pushstring (pLua, "questTalks");
	lua_pushcfunction (pLua, &questTalks);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "completedQuests");
	lua_pushcfunction (pLua, &completedQuests);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "waitForKey");
	lua_pushcfunction (pLua, &waitForKeyPress);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "lock");
	lua_pushcfunction (pLua, &lockPlayer);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "hasItem");
	lua_pushcfunction (pLua, &checkInventory);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "giveQuest");
	lua_pushcfunction (pLua, &giveQuest);
	lua_settable (pLua, -3);
	lua_setglobal (pLua, "Player");//Name the newly made library
	return 1;
}
//Level lib
int open_levellib (lua_State* pLua)
{
	lua_settop (pLua, 0);
	lua_newtable (pLua);
	lua_pushstring (pLua, "isHub");
	lua_pushcfunction (pLua, &isHub);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "setKey");
	lua_pushcfunction (pLua, &setLevelKey);
	lua_settable (pLua, -3);
	lua_pushstring (pLua, "loadBossLevel");
	lua_pushcfunction (pLua, &loadBossLevel);
	lua_settable (pLua, -3);
	lua_setglobal (pLua, "Level");//Name newly made library
	return 1;
}


//Set up a LuaParser
LuaParser::LuaParser (sf::RenderWindow* pWindow)
{
	//Maintain singleton
	if (singletonInstance != nullptr)
	{
		singletonInstance->Clean ();
		delete singletonInstance;
	}
	singletonInstance = this;
	_errorRaised = false;
	_window = pWindow;
	registerForEvent (JCPPEngine::Event::EventType::MouseDown);
	registerForEvent (JCPPEngine::Event::EventType::KeyDown);

	_luaMain = luaL_newstate ();
	if (_luaMain == nullptr)
	{
		_errorRaised = true;
		cout << "[ERROR]: Invalid lua state created!" << endl;
		return;
	}
	luaL_openlibs (_luaMain);//Open all default lua libraries
	//Open all custom 'libraries'
	open_gamelib (_luaMain);
	open_lualib (_luaMain);
	open_hiddenlib (_luaMain);
	open_importlib (_luaMain);
	open_npclib (_luaMain);
	open_playerlib (_luaMain);
	open_levellib (_luaMain);

	lua_pushcfunction (_luaMain, &printString);
	lua_setglobal (_luaMain, "print");

	_lua = lua_newthread (_luaMain);
	_originalStatePointerValue = ((int)_lua) + 1;//This calculation prevents anything from adjusting the value stored in _originalStatePointerValue

	_luaFile = "main.lua";
	SafeRefresh ();//Load lua file and start at main function
	_currentFunction = "start";//Call start function the first frame
	_startFrame = true;
}
LuaParser::~LuaParser ()
{
	unregisterForEvent (JCPPEngine::Event::EventType::MouseDown);
	unregisterForEvent (JCPPEngine::Event::EventType::KeyDown);
	if (_mouseEvent != nullptr)
	{
		delete _mouseEvent;
		_mouseEvent = nullptr;
	}
	if (_curScene != nullptr)
	{
		delete _curScene;
		_curScene = nullptr;
	}
	if (_drawBuffer.size () > 0)
	{
		for (int i = 0, size = _drawBuffer.size (); i < size; i ++)
		{
			delete _drawBuffer [i];
		}
		_drawBuffer.clear ();
	}
	if (_lua != nullptr)
	{
		lua_close (_lua);
		_lua = nullptr;
	}
	singletonInstance = nullptr;
	_window = nullptr;
}
//Clean out the LuaParser. This does not delete this object itself.
void LuaParser::Clean ()
{
	//The window may still be needed for another part of the program, so just clear it.
	_window->clear ();
	_window->display ();
	//NOTE:
	//	the texturemanager's clean function is not called here, since we may want to use the textures outside of the luaparser.
	//	If we were to delete these textures, that may cause problems. We can, however, delete the sprites.
	int size = _drawBuffer.size ();
	if (size > 0)
	{
		for (int i = size - 1; i >= 0; i --)
		{
			delete _drawBuffer [i];
		}
	}
	_drawBuffer.clear ();
	SoundManager::Clean ();
	//The lua state pointers are cleared when luaparser is deleted
}
//Reload the lua file to implement changes at runtime
void LuaParser::Refresh ()
{
	cout << "<warning> reloading the lua file." << endl;
	_drawBuffer.clear ();

	//Load the lua file, and run it once (if there are no errors)
	luaL_loadfile (_lua, _luaFile.c_str ());
	int result = lua_pcall (_lua, 0, 0, 0);

	//Check for errors
	if (result != LUA_OK)
	{
		if (lua_isstring (_lua, -1))//Write error to command prompt
		{
			cout << "[REFRESH] [ERROR]: " << lua_tostring (_lua, -1) << endl;
		}
		else//If there is no error message, say so
		{
			cout << "[REFRESH] [ERROR]: unknown; there was no error message available." << endl;
		}
	}
}
//Identical to the Refresh () function, but resets the currentfunction to main. This is useful for removing/renaming functions at runtime.
void LuaParser::SafeRefresh ()
{
	_currentFunction = "main";
	Refresh ();
}
//Load a lua file containing an exported unity scene
GameObject* LuaParser::LoadScene (string pFilename)
{
	string filepath = "scenes/" + pFilename;
	_curScene = new GameObject ("Scene", glm::vec3 (0, 0, 0));//All loaded objects will be a child of this one
	_curScene->setParent (World::singletonInstance);//Scene should be a part of world

	//Load the lua file, and run it once (if there are no errors)
	luaL_loadfile (_lua, filepath.c_str ());
	int result = lua_pcall (_lua, 0, 0, 0);

	//Check for errors
	if (result != LUA_OK)
	{
		if (lua_isstring (_lua, -1))//Write error to command prompt
		{
			cout << "[REFRESH] [ERROR]: " << lua_tostring (_lua, -1) << endl;
		}
		else//If there is no error message, say so
		{
			cout << "[REFRESH] [ERROR]: unknown; there was no error message available." << endl;
		}
	}
	GameObject* temp = _curScene;
	_curScene = nullptr;//Stop adding imported objects to this scene
	return temp;
}


//////////////////////////////|	UPDATING FUNCTIONS
//Update the lua program: return whether there were any errors
bool LuaParser::Update (float pStep)
{
	_drawBuffer.clear ();
	if (_errorRaised)
	{
		//Stop executing the lua code
		cout << "Stopping lua program to prevent further problems..." << endl;
		Clean ();
		return true;
	}

	//Update currently selected function
	if ((int)_lua != (_originalStatePointerValue - 1))
	{
		cout << "[LUASTATE] [ERROR]: The lua state was corrupted.\nThis is really bad..." << endl;
		return true;
	}

	if (_yieldTimer > 0)
	{
		_yieldTimer -= pStep;
		if (_yieldTimer <= 0)
		{
			_yieldTimer = -0.1f;//Make sure floating-point errors won't result in a false positive
			lua_settop (_lua, 0);//We were yielding, so we have to continue from where lua yielded
		}
	}
	else if (_yieldTimer < -0.9f)
	{
		_yieldTimer -= pStep;
		//We're waiting for input
		if (_spacePress || _yieldTimer < -300)//Make sure we don't wait infinitely
		{
			_yieldTimer = -0.1f;//Make sure floating-point errors won't result in a false positive
			lua_settop (_lua, 0);//We were yielding, so we have to continue from where lua yielded
		}
	}
	else
	{
		lua_getglobal (_lua, _currentFunction.c_str ());//We weren't yielding, so the selected function can be called
	}
	if (_yieldTimer < 0 && _yieldTimer > -1)
	{
		int result = lua_resume (_lua, 0, 0);
		if (result != LUA_OK && result != LUA_YIELD)
		{
			if (lua_isstring (_lua, -1))//Write error to command prompt
			{
				cout << "[LUACALL] [ERROR]: " << lua_tostring (_lua, -1) << endl;
			}
			else//If there is no error message, say so
			{
				cout << "[LUACALL] [ERROR]: unknown; there was no error message available." << endl;
			}
			//Stop executing the lua code
			cout << "Stopping lua program to prevent further problems..." << endl;
			Clean ();
			return true;
		}
	}

	if (_mouseEvent != nullptr)
	{
		_mouseEvent = nullptr;
	}
	_spacePress = false;
	if (_startFrame)
	{
		_currentFunction = "main";
		_startFrame = false;
	}
	return false;
}
//Render all images currently on-screen
void LuaParser::Render ()
{
	int size = _drawBuffer.size ();
	if (size > 0)
	{
		//Draw all images in the buffer
		for (int i = 0; i < size; i ++)
		{
			if (_drawBuffer [i] != nullptr)
			{
				_window->pushGLStates ();
				_window->draw (*_drawBuffer [i]);
				_window->popGLStates ();
			}
		}
		_drawBuffer.clear ();//Clear buffer
	}
}
//Store any mouse clicks this frame
void LuaParser::ProcessEvent (JCPPEngine::Event* pEvent)
{
	JCPPEngine::MouseEvent* mouseEvent = dynamic_cast <MouseEvent*> (pEvent);
	JCPPEngine::KeyEvent* keyEvent = dynamic_cast <KeyEvent*> (pEvent);
	if (mouseEvent != nullptr)
	{
		if (mouseEvent->button () == sf::Mouse::Button::Left)
		{
			if (_mouseEvent != nullptr)
			{
				delete _mouseEvent;
			}
			_mouseEvent = new MouseEvent (mouseEvent);//Creating a copy, eventhandler deletes its events after they are processed.
		}
	}
	else if (keyEvent != nullptr)
	{
		if (keyEvent->key () == sf::Keyboard::Key::Space)
		{
			_spacePress = true;
		}
	}
}
#endif