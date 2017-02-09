#ifndef Level_Def
#define Level_Def


//Include files
#include <string>
#include <mge/util/LevelImporter.hpp>
#include <mge/core/GameObject.hpp>
#include <mge/core/World.hpp>
#include <P3P/ProgressTracker.hpp>
#include <P3P/objects/base objects/Floor.hpp>
#include <P3P/objects/base objects/BoxSpot.hpp>
#include <P3P/objects/Player.hpp>
#include <P3P/objects/Box.hpp>
#include <P3P/objects/Door.hpp>


class Level : public GameObject
{
	private:
		int _levelNumber = 0;
	public:
		Level (int pLevelNumber);
		void setMap (int pLevelNumber);
		void loadMap ();
		void clear ();
		static const float TILESIZE;
		static LevelMap* map;
		static Level* singletonInstance;
};

#endif