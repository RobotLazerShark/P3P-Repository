#include "AbstractListener.hpp"


namespace JCPPEngine
{

	//////////////////////////////|	REGISTRATION
	//Register to receive the given type of sf::event
	void AbstractListener::registerForEvent (sf::Event::EventType pEventType)
	{
		EventHandler::RegisterForEvent (this, pEventType);
	}
	//Unregister to no longer receive the given type of sf::event
	void AbstractListener::unregisterForEvent (sf::Event::EventType pEventType)
	{
		EventHandler::UnregisterForEvent (this, pEventType);
	}
	//Register to receive the given type of JCPPEngine::event
	void AbstractListener::registerForEvent (JCPPEngine::Event::EventType pEventType)
	{
		EventHandler::RegisterForEvent (this, pEventType);
	}
	//Unregister to no longer receive the given type of JCPPEngine::event
	void AbstractListener::unregisterForEvent (JCPPEngine::Event::EventType pEventType)
	{
		EventHandler::UnregisterForEvent (this, pEventType);
	}


	//////////////////////////////|	PROCESSING EVENTS
	//Process an sf::event
	void AbstractListener::ProcessEvent (sf::Event pEvent) {}
	//Process a JCPPEngine::event
	void AbstractListener::ProcessEvent (JCPPEngine::Event* pEvent) {}

}