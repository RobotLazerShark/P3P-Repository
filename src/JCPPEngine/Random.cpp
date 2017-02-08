#include "Random.hpp"


namespace JCPPEngine
{

	//Static fields
	static bool _seeded = false;

	//Returns a random value between pMin and pMax.
	int Random::Range (int pMin, int pMax)
	{
		if (!_seeded)
		{
			srand (time (NULL));
			_seeded = true;
		}
		if (pMin > pMax)
		{
			int temp = pMin;
			pMin = pMax;
			pMax = temp;
		}
		return (rand () % (pMax - pMin + 1)) + pMin;
	}
	//Returns a random value between pMin and pMax. The valuesteps are a little larger so the values 'overlap' less.
	float Random::Range (int pMin, float pMax)
	{
		if (!_seeded)
		{
			srand (time (NULL));
			_seeded = true;
		}
		int min = pMin * 1000;
		int max = (int)round (pMax * 1000);
		if (min > max)
		{
			int temp = min;
			min = max;
			max = temp;
		}
		return ((rand () % (max - min + 1)) + min) * 0.001f;
	}
	float Random::Range (float pMin, int pMax)
	{
		float fMax = pMax;
		if (!_seeded)
		{
			srand (time (NULL));
			_seeded = true;
		}
		int min = (int)round (pMin * 1000);
		int max = pMax * 1000;
		if (min > max)
		{
			int temp = min;
			min = max;
			max = temp;
		}
		return ((rand () % (max - min + 1)) + min) * 0.001f;
	}
	float Random::Range (float pMin, float pMax)
	{
		if (!_seeded)
		{
			srand (time (NULL));
			_seeded = true;
		}
		if (pMin > pMax)
		{
			float temp = pMin;
			pMin = pMax;
			pMax = temp;
		}
		int min = (int)round (pMin * 1000);
		int max = (int)round (pMax * 1000);
		return ((rand () % (max - min + 1)) + min) * 0.001f;
	}
	//Return float between 0 and 1, with the smallest possible valuestep.
	float Random::Value ()
	{
		if (!_seeded)
		{
			srand (time (NULL));
			_seeded = true;
		}
		return (rand ()/(float)(RAND_MAX));
	}

}