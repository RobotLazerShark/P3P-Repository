#ifndef BOX_H
#define BOX_H


//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <P3P/objects/base objects/BoxSpot.hpp>
#include <mge/behaviours/AnimationBehaviour.hpp>

//Forward declarations
class LevelMap;


class Box : public GameObject
{
    public:
        Box (int pX, int pZ, bool pPowered);
        void moveBox (int pX, int pZ, bool pAnimate = true);
	void stopAnimation ();
        //Preferably these variables would be private, but they are needed in a globalspace function
        //They use the private name conventions because they should be used as such
	int _currentTile [2] = { 0, 0 };
    protected:
    private:
	    bool _powered = false;
	int _oldTile [2] = { 0, 0 };
	GameObject* _model = nullptr;
	AnimationBehaviour* _animator = nullptr;
};

#endif // BOX_H