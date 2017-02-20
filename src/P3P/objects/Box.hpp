#ifndef BOX_H
#define BOX_H


//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
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
	bool _powered = false;
	AnimationBehaviour* _animator = nullptr;
	GameObject* _model = nullptr;
    protected:
    private:
	int _oldTile [2] = { 0, 0 };
};

#endif // BOX_H