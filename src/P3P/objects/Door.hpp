#ifndef DOOR_H
#define DOOR_H


//Include files
#include <mge/core/GameObject.hpp>
#include <mge/core/World.hpp>
#include <mge/materials/LitMaterial.hpp>

class Door : public GameObject
{
    public:
        Door (int pX, int pZ, int pNextLevelNumber);
        void enter ();
    protected:
    private:
	    int _nextLevelNumber;
	    GameObject* _model;
};

#endif // DOOR_H
