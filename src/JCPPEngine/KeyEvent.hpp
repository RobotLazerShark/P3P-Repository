#ifndef JCPPE_KeyEvent_Def
#define JCPPE_KeyEvent_Def


//Include files
#include <SFML/Graphics.hpp>
#include <JCPPEngine/InputManager.hpp>


namespace JCPPEngine
{

	class KeyEvent : public Event
	{
		public:
			KeyEvent (sf::Keyboard::Key pKey, int pKeyState);
			KeyEvent (KeyEvent* pOriginal);
			sf::Keyboard::Key key ();
			int keyState ();
		private:
			sf::Keyboard::Key _key;
			int _keyState;
	};

}
#endif