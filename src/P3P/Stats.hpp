#ifndef STATS_Def
#define  STATS_Def

#include <mge/core/GameObject.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

struct Data
{
	int deathCount;
	int metersWalked;
	int itemsCollected;
	int hintsUsed;
	int platformsBroke;
	int socketsActivated;
	int questsCompleted;
};

class Stats
{
public:
	Stats(sf::Vector2f statsPos);
	~Stats();
	void setActive(bool active);
	void refreshText();

	std::vector<sf::Drawable*> getAllDrawables();
	static Stats* singletonInstance;
	Data data;

private:
	std::vector<sf::Text *> _texts;
	void loadFromFile();
	void saveToFile();
};

#endif