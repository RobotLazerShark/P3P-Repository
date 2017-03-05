#ifndef SPIKES_Def
#define SPIKES_Def


//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <mge/behaviours/AnimationBehaviour.hpp>
#include <P3P/ButtonTarget.hpp>

class Spikes : public ButtonTarget
{
public:
	Spikes(int pX, int pZ);
	void update(float pStep, bool pUpdateWorldTransform = false) override;
	bool setActive (bool pActive);
private:
	GameObject* _model = nullptr;
	AnimationBehaviour* _animator = nullptr;
	int _position [2] = { 0, 0 };
	float _timer = 0;
	bool _stopped = false;
	bool _spikesUp = false;
	const float SWITCH_TIME = 2.25f;
};

#endif