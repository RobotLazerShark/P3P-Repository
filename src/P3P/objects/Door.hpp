#ifndef DOOR_H
#define DOOR_H


//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <mge/materials/TextureMaterial.hpp>
#include <mge/behaviours/AnimationBehaviour.hpp>

class Door : public GameObject
{
    public:
        Door (int pX, int pZ, int pNextLevelNumber, int pOrientation);
	~Door ();
        bool enter ();
	void open ();
        int levelNumber ();
	void update (float pStep, bool pUpdateWorldTransform = false) override;
    protected:
    private:
	    int _nextLevelNumber;
	    GameObject* _model = nullptr;
		AnimationBehaviour* _doorLeftAnimator = nullptr;
		AnimationBehaviour* _doorRightAnimator = nullptr;
		AnimationBehaviour* _barsLeftAnimator = nullptr;
		AnimationBehaviour* _barsRightAnimator = nullptr;
		TextureMaterial* _portal = nullptr;
		int _orientation = 0;
		int _position [2] = { 0, 0 };
		bool _open = false;
		float _timer = 0;
};

#endif // DOOR_H
