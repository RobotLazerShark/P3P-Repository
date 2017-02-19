#ifndef Level_Def
#define Level_Def


//Include files
#include <SFML\Graphics.hpp>
#include <mge/core/GameObject.hpp>
#include <mge/util/LevelImporter.hpp>

class Quest;
class Gate;
class Collectable;



class Level : public GameObject
{
	protected:
		int _levelNumber = 0;
		int _levelKey = 1;
		bool _isHub = false;
		int _nextLevel = -1;
		std::vector <Gate*> _gates;
		std::vector <GameObject*> _nonWalkables;
		std::vector <std::string> _inventoryCopy;
		std::vector<Quest*> _activeQuestsCopy;
		bool _stop = false;
	public:
		Level (int pLevelNumber);
		~Level ();
		void increaseLevelKey ();
		int levelKey ();
		bool levelCompleted = false;
		bool setMap (int pLevelNumber);
		void loadMap ();
		void clear (bool pEndGame = false);
		void loadLevel (int pLevelNumber);
		void reloadLevel ();
		static const float TILESIZE;
		static LevelMap* map;
		static Level* singletonInstance;
		std::vector <sf::Drawable*>drawBuffer;
		void update (float pStep, bool pUpdateWorldTransform = false) override;
		static void render (sf::RenderWindow* pWindow);
};

#endif