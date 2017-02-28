#ifndef HUD_Def
#define HUD_Def

#include <mge/core/GameObject.hpp>
#include <SFML/Graphics.hpp>
#include <P3P/HudButton.hpp>
#include <P3P/Stats.hpp>
#include <JCPPEngine/AbstractListener.hpp>

class Hud : public GameObject, public JCPPEngine::AbstractListener
{
	public:
		Hud();
		~Hud();
		void update(float pStep, bool pUpdateWorldTransform = false) override;
		void setState(int state);
		void ProcessEvent(JCPPEngine::Event* pEvent) override;
		std::vector<sf::Drawable*> getAllDrawables();

	private:
		std::vector<HudButton*> buttons;
};

#endif