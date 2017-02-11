#ifndef SPIKES_Def
#define SPIKES_Def


//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/ColorMaterial.hpp>
#include <SFML/Graphics.hpp>

class Spikes : public GameObject
{
public:
	Spikes(int pX, int pZ);
	void update(float pStep, bool pUpdateWorldTransform = false) override;
private:
	GameObject* _model;

	glm::vec2 arrayPosition;
	sf::Clock clock;
	sf::Time time;

	bool isOn = false;
	float timeSinceLastChange = 0;
	const float SWITCH_TIME = 2;

	float oldTime = 0;
};

#endif