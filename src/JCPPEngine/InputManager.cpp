#include "InputManager.hpp"
#include "mge/lua/LuaParser.hpp"


namespace JCPPEngine
{

	//Static fields
	InputManager* InputManager::singletonInstance = nullptr;
	//Define keystates
	const int InputManager::KEY_STILL_UP;
	const int InputManager::KEY_DOWN;
	const int InputManager::KEY_STILL_DOWN;
	const int InputManager::KEY_UP;
	static std::map <int, int> _keyStates;
	static std::map <int, int> _mouseStates;


//////////////////////////////|	INSTANCE-MANAGEMENT
	//Class constructor
	InputManager::InputManager ()
	{
		if (singletonInstance != nullptr)
		{
			delete singletonInstance;
		}
		singletonInstance = this;

		//Set initial mousebutton states
		_mouseStates [0] = (sf::Mouse::isButtonPressed (sf::Mouse::Button::Left)) ? KEY_DOWN : KEY_STILL_UP;
		_mouseStates [1] = (sf::Mouse::isButtonPressed (sf::Mouse::Button::Right)) ? KEY_DOWN : KEY_STILL_UP;

		//Set initial key states
		for (int i = 0, size = sf::Keyboard::Key::KeyCount; i < size; i ++)
		{
			_keyStates [i] = (sf::Keyboard::isKeyPressed ((sf::Keyboard::Key)i)) ? KEY_DOWN : KEY_STILL_UP;
		}
	}


//////////////////////////////|	UPDATING FUNCTIONS
	//Update the keystates
	void InputManager::Update ()
	{
		//Set mousebutton states
		for (int i = 0, size = 2; i < size; i ++)//We only use left and right mouse button.
		{
			int curState = _mouseStates [i];
			if (sf::Mouse::isButtonPressed ((sf::Mouse::Button)i))//If the mousebutton is currently down
			{
				if (curState == KEY_STILL_UP || curState == KEY_DOWN)
				{
					curState = (curState + 1) % 4;
					EventHandler::RaiseEvent (new MouseEvent ((sf::Mouse::Button)i, curState, sf::Mouse::getPosition (*LuaParser::_window)));
				}
				else if (curState == KEY_UP)
				{
					curState = (curState + 2) % 4;
					EventHandler::RaiseEvent (new MouseEvent ((sf::Mouse::Button)i, curState, sf::Mouse::getPosition (*LuaParser::_window)));
				}
			}
			else//If the mousebutton is currently up
			{
				if (curState == KEY_STILL_DOWN || curState == KEY_UP)
				{
					curState = (curState + 1) % 4;
					EventHandler::RaiseEvent (new MouseEvent ((sf::Mouse::Button)i, curState, sf::Mouse::getPosition (*LuaParser::_window)));
				}
				else if (curState == KEY_DOWN)
				{
					curState = (curState + 2) % 4;
					EventHandler::RaiseEvent (new MouseEvent ((sf::Mouse::Button)i, curState, sf::Mouse::getPosition (*LuaParser::_window)));
				}
			}
			_mouseStates [i] = curState;
		}

		//Set key states
		for (int i = 0, size = (int)sf::Keyboard::Key::KeyCount; i < size; i ++)
		{
			int curState = _keyStates [i];
			if (sf::Keyboard::isKeyPressed ((sf::Keyboard::Key)i))//If the key is currently down
			{
				if (curState == KEY_STILL_UP || curState == KEY_DOWN)
				{
					curState = (curState + 1) % 4;
					EventHandler::RaiseEvent (new KeyEvent ((sf::Keyboard::Key)i, curState));
				}
				else if (curState == KEY_UP)
				{
					curState = (curState + 2) % 4;
					EventHandler::RaiseEvent (new KeyEvent ((sf::Keyboard::Key)i, curState));
				}
			}
			else//If the key is currently up
			{
				if (curState == KEY_STILL_DOWN || curState == KEY_UP)
				{
					curState = (curState + 1) % 4;
					EventHandler::RaiseEvent (new KeyEvent ((sf::Keyboard::Key)i, curState));
				}
				else if (curState == KEY_DOWN)
				{
					curState = (curState + 2) % 4;
					EventHandler::RaiseEvent (new KeyEvent ((sf::Keyboard::Key)i, curState));
				}
			}
			_keyStates [i] = curState;
		}
	}


//////////////////////////////|	INPUT-RETRIEVAL
	//Get wether this key is currently pressed
	bool InputManager::GetKey (sf::Keyboard::Key pKey)
	{
		int state = _keyStates [(int)pKey];
		return (state == KEY_STILL_DOWN || state == KEY_DOWN);
	}
	//Get wether this key was pressed this frame (holding the key is ignored)
	bool InputManager::GetKeyDown (sf::Keyboard::Key pKey)
	{
		return (_keyStates [(int)pKey] == KEY_DOWN);
	}
	//Get wether this key was released this frame
	bool InputManager::GetKeyUp (sf::Keyboard::Key pKey)
	{
		return (_keyStates [(int)pKey] == KEY_UP);
	}
	//Get wether this mousebutton is currently pressed
	bool InputManager::GetMouse (sf::Mouse::Button pButton)
	{
		int state = _mouseStates [(int)pButton];
		return (state == KEY_STILL_DOWN || state == KEY_DOWN);
	}
	//Get wether this mousebutton was pressed this frame (holding the button is ignored)
	bool InputManager::GetMouseDown (sf::Mouse::Button pButton)
	{
		return (_mouseStates [(int)pButton] == KEY_DOWN);
	}
	//Get wether this mousebutton was released this frame
	bool InputManager::GetMouseUp (sf::Mouse::Button pButton)
	{
		return (_mouseStates [(int)pButton] == KEY_UP);
	}

}