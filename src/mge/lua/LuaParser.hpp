#ifndef LuaParser_Def
#define LuaParser_Def


//Include files
#include <SFML/Graphics.hpp>
#include <mge/config.hpp>
#include <mge/lua/lua.hpp>
#include <JCPPEngine/TextureManager.hpp>
#include <JCPPEngine/FontManager.hpp>
#include <JCPPEngine/SoundManager.hpp>
#include <JCPPEngine/InputManager.hpp>
#include <JCPPEngine/AbstractListener.hpp>
#include <JCPPEngine/MouseEvent.hpp>

class GameObject;



//|	SINGLETON CLASS
class LuaParser : public JCPPEngine::AbstractListener
{
	public:
		LuaParser::LuaParser (sf::RenderWindow* pWindow);
		~LuaParser ();
		void Clean ();
		bool Update (float pStep);
		void Render ();
		void Refresh ();
		void SafeRefresh ();
		GameObject* LoadScene (std::string pFilename);
		static LuaParser* singletonInstance;
		static sf::RenderWindow* _window;
		std::string _luaFile;
		std::string _currentFunction;
		bool _errorRaised = false;
		bool _spacePress = false;
		JCPPEngine::MouseEvent* _mouseEvent = nullptr;
		std::vector <sf::Drawable*> _drawBuffer;
		GameObject* _curScene = nullptr;
		float _yieldTimer = -0.1f;
		void ProcessEvent (JCPPEngine::Event* pEvent) override;
	private:
		void cleanDrawBuffer ();
		bool _startFrame = false;
		lua_State* _luaMain = nullptr;
		lua_State* _lua = nullptr;
		int _originalStatePointerValue;//Needed, becuase somehow lua corrupts the pointer on exit.
};

#endif
