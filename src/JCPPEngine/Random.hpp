#ifndef Random_Def
#define Random_Def

//Includes
#include <cstdlib>
#include <ctime>
#include <cmath>


namespace JCPPEngine
{
	class Random
	{
		public:
			static int Range (int pMin, int pMax);
			static float Range (int pMin, float pMax);
			static float Range (float pMin, int pMax);
			static float Range (float pMin, float pMax);
			static float Value ();
	};
}

#endif