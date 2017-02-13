#ifndef SPIKES_Def
#define SPIKES_Def


//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <SFML/Graphics.hpp>

class Spikes : public GameObject
{
public:
	Spikes(int pX, int pZ);
	void update(float pStep, bool pUpdateWorldTransform = false) override;
private:
	GameObject* _model;

	int _position [2] = { 0, 0 };
	float _timer;

	bool _spikesUp = false;
	const float SWITCH_TIME = 2;
};

#endif