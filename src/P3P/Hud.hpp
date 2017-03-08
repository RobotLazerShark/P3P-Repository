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
		Hud(sf::RenderWindow* pWindow);
		~Hud();
		void update(float pStep, bool pUpdateWorldTransform = false) override;
		void setState(int state);

		void ProcessEvent(JCPPEngine::Event* pEvent) override;
		void ProcessEvent(sf::Event pEvent) override;

		std::vector<sf::Drawable*> getAllDrawables();
		void disable();
		void enable();
		void pressButton(int button);

		static Hud* singletonInstance;

		int state;

		sf::Sprite * _levelPauseSprite = nullptr;
		sf::Sprite * _hubPauseSprite = nullptr;
		sf::Sprite * _statsSprite = nullptr;

		bool showPauseMenu = false;

		bool blockStatsReturn = false;
		
	private:
		std::vector<HudButton*> buttons;
		bool _active = true;
		bool _debugMode = false;

		float _statsReturnBlockedFor = 0;
		float _statsBlockingTime = 0.5f;
		
};

#endif