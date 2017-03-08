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
class Background;


class Level : public GameObject
{
	protected:
		int _levelNumber = -1;
		bool _isHub = false;
		int _nextLevel = -1;
		std::vector <Gate*> _gates;
		std::vector <GameObject*> _nonWalkables;
		std::vector <std::string> _inventoryCopy;
		std::vector<Quest*> _activeQuestsCopy;
		int _questTalksCopy = 0;
		bool _stop = false;
		bool _reloading = false;
		int _playerSkin = 1;
		std::vector<ProgressTracker*> bossPuzzlesTrackers;
		sf::Sprite* _hudOverlay = nullptr;
		float _fadeMin = 1.75f;
		float _fadeMax = 3;
		float _distribution = 1;
		sf::Sprite* _endScreen = nullptr;
		bool _fading = false;
		float _endTimer = 0;
	public:
		int levelKey = 0;
		int _bossLevelNumber = 3;
		Background* _background = nullptr;
		Level (int pPlayerSkin, sf::RenderWindow* pWindow);
		~Level ();
		bool isHub ();
		bool levelCompleted = false;
		bool setMap (int pLevelNumber);
		void loadMap ();
		void clear ();
		void loadLevel (int pLevelNumber);
		void reloadLevel ();
		void startFade ();
		static const float TILESIZE;
		static LevelMap* map;
		static Level* singletonInstance;
		std::vector <sf::Drawable*>drawBuffer;
		std::vector <Collectable*>deleteBuffer;
		void update (float pStep, bool pUpdateWorldTransform = false) override;
		static void renderBackground (sf::RenderWindow* pWindow);
		static void render (sf::RenderWindow* pWindow);
		Hud * hud = nullptr;
		std::vector<Hint*> hints;
		GameObject* transparencyLayer1 = nullptr;
		GameObject* transparencyLayer2 = nullptr;
		GameObject* transparencyLayer3 = nullptr;
};

#endif