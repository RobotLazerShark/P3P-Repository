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
	protected:
		int _levelNumber = 0;
		int _levelKey = 1;
		bool _isHub = false;
	public:
		Level (int pLevelNumber);
		void increaseLevelKey ();
		int levelKey ();
		bool levelCompleted = false;
		void setMap (int pLevelNumber);
		void loadMap ();
		void clear ();
		void loadLevel (int pLevelNumber);
		void reloadLevel ();
		static const float TILESIZE;
		static LevelMap* map;
		static Level* singletonInstance;
};

#endif