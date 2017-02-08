#include "JCPPEngine/Event.hpp"


namespace JCPPEngine
{	

	//////////////////////////////|	INSTANCE MANAGEMENT
	//Destructor
	Event::~Event () {}
	//Get the event type
	Event::EventType Event::eventType ()
	{
		return _eventType;
	}

}