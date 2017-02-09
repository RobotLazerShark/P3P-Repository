#ifndef LevelImporter_Def
#define LevelImporter_Def


//Include files
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <mge/util/LevelMap.hpp>


class LevelImporter
{
	private:
		static const int TILED_TILESIZE;
	public:
		static LevelMap* ReadFile (std::string pFilename, bool pExtraLayer = false);
};
#endif