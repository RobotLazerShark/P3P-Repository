#ifndef ButtonTarget_Def
#define ButtonTarget_Def


//Include files
#include <mge/core/GameObject.hpp>


class ButtonTarget : public GameObject
{
	public:
		virtual bool setActive (bool pActive) = 0;
};

#endif