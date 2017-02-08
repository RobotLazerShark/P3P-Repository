#ifndef BOXSPOT_H
#define BOXSPOT_H


//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>


class BoxSpot : public GameObject
{
    public:
        BoxSpot (int pX, int pZ);
        bool taken = false;
    protected:
    private:
	    GameObject* _model;
};

#endif // BOXSPOT_H