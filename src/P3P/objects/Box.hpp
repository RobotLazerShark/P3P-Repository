<<<<<<< HEAD
#ifndef BOX_H
#define BOX_H


//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <P3P/objects/base objects/BoxSpot.hpp>

//Forward declarations
class LevelMap;


class Box : public GameObject
{
    public:
        Box (int pX, int pZ);
        void moveBox (int pX, int pZ);
    protected:
    private:
	glm::vec2 _currentTile;
	glm::vec2 _oldTile;
	GameObject* _model;
};

=======
#ifndef BOX_H
#define BOX_H


//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <P3P/objects/base objects/BoxSpot.hpp>

//Forward declarations
class LevelMap;


class Box : public GameObject
{
    public:
        Box (int pX, int pZ);
        void moveBox (int pX, int pZ);
    protected:
    private:
	glm::vec2 _currentTile;
	glm::vec2 _oldTile;
	GameObject* _model;
};

>>>>>>> 05a65edaed0d726361052543950587d121f9c233
#endif // BOX_H