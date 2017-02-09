#ifndef LuaParser_Def
#define LuaParser_Def


//Include files
#include <stdio.h>
#include <glm.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <mge/config.hpp>
#include <mge/lua/lua.hpp>
#include <mge/core/Texture.hpp>
#include <mge/core/World.hpp>
#include <mge/core/GameObject.hpp>
#include <mge/core/AbstractGame.hpp>
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
#include <JCPPEngine/TextureManager.hpp>
#include <JCPPEngine/FontManager.hpp>
#include <JCPPEngine/SoundManager.hpp>
#include <JCPPEngine/InputManager.hpp>
#include <JCPPEngine/AbstractListener.hpp>
#include <JCPPEngine/MouseEvent.hpp>



//|	SINGLETON CLASS
class LuaParser : public JCPPEngine::AbstractListener
{
	public:
		LuaParser::LuaParser (sf::RenderWindow* pWindow);
		void Clean ();
		bool Update (float pStep);
		void Render ();
		void Refresh ();
		void SafeRefresh ();
		GameObject* LoadScene (std::string pFilename);
		static LuaParser* singletonInstance;
		static sf::RenderWindow* _window;
		std::string _luaFile = "main.lua";
		std::string _currentFunction = "main";
		bool _errorRaised = false;
		JCPPEngine::MouseEvent* _mouseEvent = nullptr;
		std::vector <sf::Drawable*> _drawBuffer;
		GameObject* _curScene = nullptr;
		float _yieldTimer = -1;
		void ProcessEvent (JCPPEngine::Event* pEvent) override;
	private:
		void cleanDrawBuffer ();
		bool _startFrame;
		lua_State* _luaMain;
		lua_State* _lua;
		int _originalStatePointerValue;//Needed, becuase somehow lua corrupts the pointer on exit.
};

#endif
