#ifndef AbstractListener_Def
#define AbstractListener_Def

//Include files
#include <SFML/Graphics.hpp>
#include <vector>
#include "EventHandler.hpp"
#include "JCPPEngine/Event.hpp"


namespace JCPPEngine
{

	class AbstractListener
	{
		protected:
			void registerForEvent (sf::Event::EventType pEventType);
			void unregisterForEvent (sf::Event::EventType pEventType);
			void registerForEvent (JCPPEngine::Event::EventType pEventType);
			void unregisterForEvent (JCPPEngine::Event::EventType pEventType);
		public:
			virtual void ProcessEvent (sf::Event pEvent);
			virtual void ProcessEvent (JCPPEngine::Event* pEvent);
	};

}
#endif