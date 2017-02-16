#include "EventHandler.hpp"
#include "AbstractListener.hpp"
using namespace std;
#include <iostream>

namespace JCPPEngine
{

	//Static fields
	static map <sf::Event::EventType, vector <AbstractListener*>> _sfListeners;
	static map <JCPPEngine::Event::EventType, vector <AbstractListener*>> _JCPPEListeners;
	static vector <JCPPEngine::Event*> _eventQueue;
	static sf::RenderWindow* _window = nullptr;



	//////////////////////////////|	INITIALISATION
	//Set the window we will be using for events
	void EventHandler::SetEventWindow (sf::RenderWindow* pWindow)
	{
		_window = pWindow;
	}

	//////////////////////////////|	REGISTRATION
	//Register to receive the given sf::eventtype
	void EventHandler::RegisterForEvent (AbstractListener* pListener, sf::Event::EventType pEventType)
	{
		_sfListeners [pEventType].push_back (pListener);
	}
	//Unregister to no longer receive the given sf::eventtype
	void EventHandler::UnregisterForEvent (AbstractListener* pListener, sf::Event::EventType pEventType)
	{
		if (_sfListeners [pEventType].size () == 0)
		{
			return;
		}

		vector <AbstractListener*>::iterator begin = _sfListeners [pEventType].begin ();
		for (int i = 0, size = _sfListeners [pEventType].size (); i < size; i ++)
		{
			if (_sfListeners [pEventType] [i] == pListener)
			{
				_sfListeners [pEventType].erase (begin + i);
				return;
			}
		}
	}
	//Register to receive the given JCPPEngine::eventtype
	void EventHandler::RegisterForEvent (AbstractListener* pListener, JCPPEngine::Event::EventType pEventType)
	{
		_JCPPEListeners [pEventType].push_back (pListener);
	}
	//Unregister to no longer receive the given JCPPEngine::eventtype
	void EventHandler::UnregisterForEvent (AbstractListener* pListener, JCPPEngine::Event::EventType pEventType)
	{
		if (_JCPPEListeners [pEventType].size () == 0)
		{
			return;
		}

		vector <AbstractListener*>::iterator begin = _JCPPEListeners [pEventType].begin ();
		for (int i = 0, size = _JCPPEListeners [pEventType].size (); i < size; i ++)
		{
			if (_JCPPEListeners [pEventType] [i] == pListener)
			{
				_JCPPEListeners [pEventType].erase (begin + i);
				return;
			}
		}
	}


	//////////////////////////////|	EVENT PROCESSING
	//Put events in the queue (events are raised out-of-sync with the game FPS)
	void EventHandler::RaiseEvent (JCPPEngine::Event* pEvent)
	{
		_eventQueue.push_back (pEvent);
	}
	//Send events to registered listeners
	void EventHandler::ProcessEvents ()
	{
		sf::Event tempEvent;
		while (_window->pollEvent (tempEvent))
		{
			for (int i = 0, size = _sfListeners [tempEvent.type].size (); i < size; i ++)
			{
				_sfListeners [tempEvent.type] [i]->ProcessEvent (tempEvent);
			}
		}
		if (_eventQueue.size () > 0)
		{
			JCPPEngine::Event::EventType tempType;
			vector <JCPPEngine::Event*>::iterator begin = _eventQueue.begin ();
			for (int j = 0, size = _eventQueue.size (); j < size; j ++)//Loop through events in chronological order
			{
				tempType = _eventQueue [j]->eventType ();//Since we remove every event we processed, we're always at index zero.
				if (_JCPPEListeners [tempType].size () > 0)
				{
					for (int i = 0, size = _JCPPEListeners [tempType].size (); i < size; i ++)
					{
						_JCPPEListeners [tempType] [i]->ProcessEvent (_eventQueue [j]);
					}
				}
				delete _eventQueue [j];
			}
			_eventQueue.clear ();
		}
	}

}