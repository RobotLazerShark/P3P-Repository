<<<<<<< HEAD
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

=======
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

>>>>>>> 05a65edaed0d726361052543950587d121f9c233
#endif // BOXSPOT_H