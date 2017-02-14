#ifndef Level_Def
#define Level_Def


//Include files
#include <string>
#include <mge/util/LevelImporter.hpp>
#include <mge/core/GameObject.hpp>
#include <mge/core/World.hpp>
#include <P3P/ProgressTracker.hpp>
#include <P3P/Quest.hpp>
#include <P3P/objects/base objects/Floor.hpp>
#include <P3P/objects/base objects/BoxSpot.hpp>
#include <P3P/objects/Player.hpp>
#include <P3P/objects/Box.hpp>
#include <P3P/objects/Door.hpp>
#include <P3P/objects/Gate.hpp>
#include <P3P/objects/Npc.hpp>
#include <P3P/objects/Fan.hpp>
#include <P3P/objects/Collectable.hpp>
#include <P3P/objects/base objects/Button.hpp>
#include <P3P/objects/base objects/Spikes.hpp>



class Level : public GameObject
{
	protected:
		int _levelNumber = 0;
		int _levelKey = 1;
		bool _isHub = false;
		int _nextLevel = -1;
		std::vector <Gate*> _gates;
		std::vector <Collectable*> _inventoryCopy;
		std::vector<Quest*> _activeQuestsCopy;
	public:
		Level (int pLevelNumber);
		~Level ();
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
		void update (float pStep, bool pUpdateWorldTransform = false) override;
};

#endif