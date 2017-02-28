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
	int var1;
	int var2;
	int var3;
	int var4;
	int var5;
}; 

class Stats
{
public:
	Stats();
	~Stats();
	void setActive(bool active);
	void saveToFile();
	std::vector<sf::Drawable*> getAllDrawables();

	static Stats* singletonInstance;
	sf::Sprite * _sprite = nullptr;
	Data data;
private:
	std::vector<sf::Text *> _texts;
	void loadFromFile();
};

#endif