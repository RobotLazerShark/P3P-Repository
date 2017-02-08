#ifndef InputManager_Def
#define InputManager_Def

//Includes
#include <vector>
#include <SFML\Graphics.hpp>
#include <JCPPEngine/EventHandler.hpp>
#include <JCPPEngine/MouseEvent.hpp>
#include <JCPPEngine/KeyEvent.hpp>


namespace JCPPEngine
{

	class InputManager
	{
		public:
			InputManager ();
			static void Update ();
			static bool GetKey (sf::Keyboard::Key pKey);
			static bool GetKeyDown (sf::Keyboard::Key pKey);
			static bool GetKeyUp (sf::Keyboard::Key pKey);
			static bool GetMouse (sf::Mouse::Button pButton);
			static bool GetMouseDown (sf::Mouse::Button pButton);
			static bool GetMouseUp (sf::Mouse::Button pButton);
			static const int InputManager::KEY_STILL_UP = 0;
			static const int InputManager::KEY_DOWN = 1;
			static const int InputManager::KEY_STILL_DOWN = 2;
			static const int InputManager::KEY_UP = 3;
	};

}
#endif