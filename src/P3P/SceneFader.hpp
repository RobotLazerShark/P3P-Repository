#ifndef SCENEFADER_Def
#define SCENEFADER_Def

#include <mge/core/GameObject.hpp>
#include <SFML/Graphics.hpp>

class SceneFader : public GameObject
{
	public:
		static SceneFader* singletonInstance;
		sf::Sprite * sprite = nullptr;
		SceneFader(sf::RenderWindow * pWindow);
		~SceneFader();
		void update(float pStep, bool pUpdateWorldTransform = false) override;
		void fade(bool state);

		bool levelIsAllowedToReload = true;

	private:
		const float FADE_DURATION_TIME = 0.5;
		const float STAY_BLACK_DURATION_TIME = 0.0f;
		const float MAX_COLOR_VALUE = 255;

		float _currentAlpha = 0.1f;
		float _blackFor = 0;

		bool _increaseAlphaFlag = false;
		bool _decreaseAlphaFlag = false;
		bool _stayMaxAlphaFlag = false;

		sf::Clock clock; // starts the clock
		sf::Time  * time;

		float lastTime = 0;

		sf::RenderWindow * _window = nullptr;
};

#endif
