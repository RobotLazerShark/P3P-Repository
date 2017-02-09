#ifndef DoorObject_Def
#define DoorObject_Def


class DoorObject
{
	public:
		DoorObject (int pX, int pZ, int pNextLevelNumber);
		int x ();
		int z ();
		int nextLevelNumber ();
	private:
		int _x;
		int _z;
		int _nextLevelNumber;
};

#endif