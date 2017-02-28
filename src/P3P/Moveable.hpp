#ifndef Moveable_Def
#define Moveable_Def


//Forward declarations
class GameObject;

class Moveable
{
public:
	virtual bool move (int pX, int pZ, bool pAnimate = true) = 0;
	virtual bool move (int pX, int pZ, bool pAnimate, void (*pFuncPtr) (int, GameObject*), GameObject* pFuncOwner) = 0;
	virtual void stopAnimation () = 0;
	//Preferably these variables would be private, but they are needed in a globalspace function
	//They use the private name conventions because they should be used as such
	GameObject* _funcOwner = nullptr;
	void (*_stopFunc) (int, GameObject*) = nullptr;
};

#endif