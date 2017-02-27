#ifndef Moveable_Def
#define Moveable_Def


class Moveable
{
public:
	virtual bool move (int pX, int pZ, bool pAnimate = true) = 0;
	virtual void stopAnimation () = 0;
};

#endif