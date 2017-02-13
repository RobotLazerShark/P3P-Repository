#ifndef XmlObject_Def
#define XmlObject_Def


//Include files
#include <vector>


class XmlObject
{
	public:
		XmlObject ();
		int type = 0;
		int x = 0;
		int z = 0;
		std::vector <std::string> properties;
};

#endif