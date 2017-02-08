#ifndef JCPPE_Event_Def
#define JCPPE_Event_Def


//Include files
#include <SFML/Graphics.hpp>


namespace JCPPEngine
{

	class Event
	{
		public:
			enum EventType { KeyDown, KeyStillDown, KeyUp, KeyStillUp, MouseDown, MouseStillDown, MouseUp, MouseStillUp, Unknown };
			virtual ~Event ();
			EventType eventType ();
		protected:
			EventType _eventType = EventType::Unknown;
	};

}
#endif