#ifndef EventHandler_Def
#define EventHandler_Def

//Include files
#include <SFML/Graphics.hpp>
#include <JCPPEngine/Event.hpp>

//Forward declarations
namespace JCPPEngine
{
	class AbstractListener;
}


namespace JCPPEngine
{

	class EventHandler
	{
		public:
			static void SetEventWindow (sf::RenderWindow* pWindow);
			static void RegisterForEvent (AbstractListener* pListener, sf::Event::EventType pEventType);
			static void UnregisterForEvent (AbstractListener* pListener, sf::Event::EventType pEventType);
			static void RegisterForEvent (AbstractListener* pListener, JCPPEngine::Event::EventType pEventType);
			static void UnregisterForEvent (AbstractListener* pListener, JCPPEngine::Event::EventType pEventType);
			static void RaiseEvent (JCPPEngine::Event* pEvent);
			static void ProcessEvents ();
	};

}
#endif