#include <P3P/DoorObject.hpp>
#include <iostream>

//Constructor
DoorObject::DoorObject (int pX, int pZ, int pNextLevelNumber)
{
	_x = pX;
	_z = pZ;
	_nextLevelNumber = pNextLevelNumber;
}


//////////////////////////////|	GETTERS
//Get this object's x position (int tiles)
int DoorObject::x ()
{
	return _x;
}
//Get this object's z position (int tiles)
int DoorObject::z ()
{
	return _z;
}
//Get this object's nextLevelNumber
int DoorObject::nextLevelNumber ()
{
	return _nextLevelNumber;
}