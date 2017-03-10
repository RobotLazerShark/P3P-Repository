#ifndef HINT_Def
#define HINT_Def


//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <mge/behaviours/AnimationBehaviour.hpp>
#include <SFML/Graphics.hpp>
#include <mge/objects/DoomSprite.hpp>

class Hint : public GameObject
{
	public:
		Hint(int pX, int pZ, std::string pInfo);
		~Hint();
		//void update(float pStep, bool pUpdateWorldTransform = false) override;
		void setActive(bool active);

		sf::Text * _text;

	private:
		DoomSprite* _model = nullptr;
		AnimationBehaviour* _animator = nullptr;
};

#endif
