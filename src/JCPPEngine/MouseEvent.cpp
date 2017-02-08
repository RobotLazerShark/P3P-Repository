#include "JCPPEngine/MouseEvent.hpp"


namespace JCPPEngine
{
	

	//////////////////////////////|	INSTANCE MANAGEMENT
	//Constructor for input events
	MouseEvent::MouseEvent (sf::Mouse::Button pButton, int pButtonState, sf::Vector2i pPosition)
	{
		_button = pButton;
		_buttonState = pButtonState;
		_position = pPosition;
		switch (pButtonState)
		{
			case InputManager::KEY_STILL_UP:
				_eventType = EventType::MouseStillUp;
				break;
			case InputManager::KEY_DOWN:
				_eventType = EventType::MouseDown;
				break;
			case InputManager::KEY_STILL_DOWN:
				_eventType = EventType::MouseStillDown;
				break;
			case InputManager::KEY_UP:
				_eventType = EventType::MouseUp;
				break;
			default:
				_eventType = EventType::Unknown;
				break;
		}
	}
	//Copy constructor
	MouseEvent::MouseEvent (MouseEvent* pOriginal)
	{
		_button = pOriginal->button ();
		_buttonState = pOriginal->buttonState ();
		_position = pOriginal->position ();
		switch (_buttonState)
		{
			case InputManager::KEY_STILL_UP:
				_eventType = EventType::MouseStillUp;
				break;
			case InputManager::KEY_DOWN:
				_eventType = EventType::MouseDown;
				break;
			case InputManager::KEY_STILL_DOWN:
				_eventType = EventType::MouseStillDown;
				break;
			case InputManager::KEY_UP:
				_eventType = EventType::MouseUp;
				break;
			default:
				_eventType = EventType::Unknown;
				break;
		}
	}


	//////////////////////////////|	GETTERS
	//Get the button
	sf::Mouse::Button MouseEvent::button ()
	{
		return _button;
	}
	//Get the button's new state
	int MouseEvent::buttonState ()
	{
		return _buttonState;
	}
	//Get the mouse position at the time of this event
	sf::Vector2i MouseEvent::position ()
	{
		return _position;
	}

}