#include "EventHandler.hpp"
#include "AbstractListener.hpp"
using namespace std;


namespace JCPPEngine
{

	//Static fields
	static map <sf::Event::EventType, vector <AbstractListener*>> _sfListeners;
	static map <JCPPEngine::Event::EventType, vector <AbstractListener*>> _JCPPEListeners;
	static vector <JCPPEngine::Event*> _eventQueue;
	static sf::RenderWindow* _window;



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
		vector <AbstractListener*>::iterator itr;
		vector <AbstractListener*>::iterator end;
		for (itr = _sfListeners [pEventType].begin (), end = _sfListeners [pEventType].end (); itr != end; itr ++)
		{
			_sfListeners [pEventType].erase (itr);
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
		vector <AbstractListener*>::iterator itr;
		vector <AbstractListener*>::iterator end;
		for (itr = _JCPPEListeners [pEventType].begin (), end = _JCPPEListeners [pEventType].end (); itr != end; itr ++)
		{
			_JCPPEListeners [pEventType].erase (itr);
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
		JCPPEngine::Event::EventType tempType;
		vector <JCPPEngine::Event*>::iterator begin = _eventQueue.begin ();
		for (int j = 0, size = _eventQueue.size (); j < size; j ++)//Loop through events in chronological order
		{
			tempType = _eventQueue [0]->eventType ();//Since we remove every event we processed, we're always at index zero.
			for (int i = 0, size = _JCPPEListeners [tempType].size (); i < size; i ++)
			{
				_JCPPEListeners [tempType] [i]->ProcessEvent (_eventQueue [0]);
			}
			delete _eventQueue [0];
			_eventQueue.erase (begin);//Remove processed events from the list
		}
	}

}