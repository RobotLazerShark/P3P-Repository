#include "JCPPEngine/KeyEvent.hpp"


namespace JCPPEngine
{
	

	//////////////////////////////|	INSTANCE MANAGEMENT
	//Constructor for input events
	KeyEvent::KeyEvent (sf::Keyboard::Key pKey, int pKeyState)
	{
		_key = pKey;
		_keyState = pKeyState;
		switch (pKeyState)
		{
			case InputManager::KEY_STILL_UP:
				_eventType = EventType::KeyStillUp;
				break;
			case InputManager::KEY_DOWN:
				_eventType = EventType::KeyDown;
				break;
			case InputManager::KEY_STILL_DOWN:
				_eventType = EventType::KeyStillDown;
				break;
			case InputManager::KEY_UP:
				_eventType = EventType::KeyUp;
				break;
			default:
				_eventType = EventType::Unknown;
				break;
		}
	}
	//Copy constructor
	KeyEvent::KeyEvent (KeyEvent* pOriginal)
	{
		_key = pOriginal->key ();
		_keyState = pOriginal->keyState ();
		switch (_keyState)
		{
			case InputManager::KEY_STILL_UP:
				_eventType = EventType::KeyStillUp;
				break;
			case InputManager::KEY_DOWN:
				_eventType = EventType::KeyDown;
				break;
			case InputManager::KEY_STILL_DOWN:
				_eventType = EventType::KeyStillDown;
				break;
			case InputManager::KEY_UP:
				_eventType = EventType::KeyUp;
				break;
			default:
				_eventType = EventType::Unknown;
				break;
		}
	}


	//////////////////////////////|	GETTERS
	//Get the key
	sf::Keyboard::Key KeyEvent::key ()
	{
		return _key;
	}
	//Get the key's new state
	int KeyEvent::keyState ()
	{
		return _keyState;
	}

}