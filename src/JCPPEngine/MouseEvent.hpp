#ifndef JCPPE_MouseEvent_Def
#define JCPPE_MouseEvent_Def


//Include files
#include <SFML/Graphics.hpp>
#include <JCPPEngine/InputManager.hpp>


namespace JCPPEngine
{

	class MouseEvent : public Event
	{
		public:
			MouseEvent (sf::Mouse::Button pButton, int pButtonState, sf::Vector2i pPosition);
			MouseEvent (MouseEvent* pMouseEvent);
			sf::Mouse::Button button ();
			int buttonState ();
			sf::Vector2i position ();
		private:
			sf::Mouse::Button _button;
			int _buttonState;
			sf::Vector2i _position;
	};

}
#endif