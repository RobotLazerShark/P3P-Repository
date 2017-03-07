#include <P3P/Stats.hpp>
#include <sstream>
#include <JCPPEngine/FontManager.hpp>
#include <JCPPEngine/TextureManager.hpp>

//Static variables
Stats* Stats::singletonInstance = nullptr;

Stats::Stats()
{
	singletonInstance = this;

	//set sprite
	_sprite = new sf::Sprite(*JCPPEngine::TextureManager::GetTexture ("mge/textures/Stats.png"));
	_sprite->setPosition(sf::Vector2f(190, 4 * 95));

	//set all texts
	loadFromFile();
	_texts.push_back(new sf::Text("deathCount", *JCPPEngine::FontManager::GetFont("fonts/Font1.ttf")));
	_texts.push_back(new sf::Text("metersWalked", *JCPPEngine::FontManager::GetFont("fonts/Font1.ttf")));
	_texts.push_back(new sf::Text("itemsCollected", *JCPPEngine::FontManager::GetFont("fonts/Font1.ttf")));
	_texts.push_back(new sf::Text("hintsUsed", *JCPPEngine::FontManager::GetFont("fonts/Font1.ttf")));
	_texts.push_back(new sf::Text("platformsBroke", *JCPPEngine::FontManager::GetFont("fonts/Font1.ttf")));
	_texts.push_back(new sf::Text("socketsActivated", *JCPPEngine::FontManager::GetFont("fonts/Font1.ttf")));
	_texts.push_back(new sf::Text("questsCompleted", *JCPPEngine::FontManager::GetFont("fonts/Font1.ttf")));
	for (int i = 0; i < _texts.size(); i++)
	{
		_texts[i]->setPosition(sf::Vector2f(190, 4 * 95 + 30 * i));
	}
	data.itemsCollected = 0;
	data.questsCompleted = 0;
	refreshText();

	setActive(false);
}

Stats::~Stats()
{
	singletonInstance = nullptr;
	for (sf::Text * text : _texts)
	{
		delete text;
	}
	_texts.clear ();
	delete _sprite;
}

void Stats::setActive(bool active) //hide/show drawable stuff
{
	int alpha;
	if (active)
	{
		alpha = 255;
	}
	else
	{
		alpha = 0;
	}

	_sprite->setColor(sf::Color(255, 255, 255, alpha));
	for (sf::Text * text : _texts)
	{
		text->setColor(sf::Color(255, 255, 255, alpha));
	}
}

std::vector<sf::Drawable*> Stats::getAllDrawables() //return all sf::Drawable objects;
{
	std::vector<sf::Drawable*> drawables;
	drawables.push_back(_sprite);
	for (sf::Text * text : _texts)
	{
		drawables.push_back(text);
	}
	return drawables;
}

void Stats::loadFromFile()
{
	std::vector<int> fileData;

	std::string filepath = "gameData/data.txt";
	std::ifstream file(filepath, std::ios::in);

	if (file.is_open())
	{
		std::string line;
		std::string readString;
		bool reading = true;
		int strPos;
		while (reading)
		{
			getline(file, line);
			if (line.find("*") != std::string::npos)
			{
				int stringSize = line.length();
				strPos = (int)line.find("*") + 1;
				readString = "";
				while (line[strPos] != '*' && strPos < stringSize)
				{
					if (line[strPos] != '/')
					{
						readString += line[strPos];
					}
					else //save number, reset readString
					{
						fileData.push_back(atoi(readString.c_str()));
						readString = "";
					}
					strPos++;
				}
				reading = false;
			}
		}

	}
	data.deathCount = fileData[0];
	data.metersWalked = fileData[1];
	data.itemsCollected = fileData[2];
	data.hintsUsed = fileData[3];
	data.platformsBroke = fileData[4];
	data.socketsActivated = fileData[5];
	data.questsCompleted = fileData[6];
}

void Stats::saveToFile()
{
	std::string fileData = "*";
	fileData += to_string(data.deathCount);
	fileData += "/";
	fileData += to_string(data.metersWalked);
	fileData += "/";
	fileData += to_string(data.itemsCollected);
	fileData += "/";
	fileData += to_string(data.hintsUsed);
	fileData += "/";
	fileData += to_string(data.platformsBroke);
	fileData += "/";
	fileData += to_string(data.socketsActivated);
	fileData += "/";
	fileData += to_string(data.questsCompleted);
	fileData += "/";
	fileData += "*";

	ofstream outputFile;
	outputFile.open("gameData/data.txt");
	outputFile << fileData << endl;
	outputFile.close();
}

void Stats::refreshText()
{
	_texts[0]->setString("deathcount: " + to_string(data.deathCount));
	_texts[1]->setString("meters walked: " + to_string(data.metersWalked));
	_texts[2]->setString("items colleced: " + to_string(data.itemsCollected) + "/10");
	_texts[3]->setString("hints used: " + to_string(data.hintsUsed));
	_texts[4]->setString("platforms broke: " + to_string(data.platformsBroke));
	_texts[5]->setString("sockets activated:" +to_string(data.socketsActivated));
	_texts[6]->setString("quests completed:" + to_string(data.questsCompleted) + "/5");

	saveToFile();
}

template <typename T>
std::string to_string(T value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}