#ifndef LevelMap_Def
#define LevelMap_Def


//Includes
#include <vector>


class LevelMap
{
	public:
		LevelMap (int pWidth, int pHeight);
		const int width = 0;
		const int height = 0;
		std::vector <std::vector <int>> baseTiles;
		std::vector <std::vector <int>> objectTiles;
};
#endif