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
	int _currentTile [2] = { 0, 0 };
	int _oldTile [2] = { 0, 0 };
	GameObject* _model;
};

#endif // BOX_H