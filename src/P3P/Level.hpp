#ifndef Level_Def
#define Level_Def


//Include files
#include <SFML\Graphics.hpp>
#include <mge/core/GameObject.hpp>
#include <mge/util/LevelImporter.hpp>
#include <P3P/Hud.hpp>
#include <P3P/Hint.hpp>
#include <P3P/ProgressTracker.hpp>

class Quest;
class Gate;
class Collectable;


class Level : public GameObject
{
	protected:
		int _levelNumber = -1;
		int _levelKey = 1;
		bool _isHub = false;
		int _nextLevel = -1;
		std::vector <Gate*> _gates;
		std::vector <GameObject*> _nonWalkables;
		std::vector <std::string> _inventoryCopy;
		std::vector<Quest*> _activeQuestsCopy;
		int _questTalksCopy = 0;
		bool _stop = false;
		bool _reloading = false;
		int _bossLevelNumber = 5;
		int _playerSkin = 1;
		std::vector<ProgressTracker*> bossPuzzlesTrackers;
	public:
		Level (int pPlayerSkin);
		~Level ();
		void increaseLevelKey ();
		int levelKey ();
		bool isHub ();
		bool levelCompleted = false;
		bool setMap (int pLevelNumber);
		void loadMap ();
		void clear ();
		void loadLevel (int pLevelNumber);
		void reloadLevel ();
		static const float TILESIZE;
		static LevelMap* map;
		static Level* singletonInstance;
		std::vector <sf::Drawable*>drawBuffer;
		std::vector <Collectable*>deleteBuffer;
		void update (float pStep, bool pUpdateWorldTransform = false) override;
		static void render (sf::RenderWindow* pWindow);
		Hud * hud = nullptr;
		std::vector<Hint*> hints;
};

#endif