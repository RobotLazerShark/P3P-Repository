#ifndef LevelMap_Def
#define LevelMap_Def


//Includes
#include <vector>
#include <P3P/DoorObject.hpp>


class LevelMap
{
	public:
		LevelMap (int pWidth, int pHeight);
		const int width = 0;
		const int height = 0;
		std::vector <std::vector <int>> baseTiles;
		std::vector <std::vector <int>> objectTiles;
		std::vector <DoorObject*> doorObjects;
};

#endif