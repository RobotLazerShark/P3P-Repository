#include <P3P/Stats.hpp>
#include <sstream>

//Static variables
Stats* Stats::singletonInstance = nullptr;

template <typename T>
std::string to_string(T value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

Stats::Stats()
{
	singletonInstance = this;

	//set sprite
	sf::Texture * texture = new sf::Texture();
	texture->loadFromFile("mge/textures/Stats.png");
	_sprite = new sf::Sprite();
	_sprite->setTexture(*texture);
	_sprite->setPosition(sf::Vector2f(190, 4 * 95));

	//set all texts
	loadFromFile();
	sf::Font * font = new sf::Font();
	font->loadFromFile("fonts/Font1.ttf");
	_texts.push_back(new sf::Text(to_string(data.var1), *font));
	_texts.push_back(new sf::Text(to_string(data.var2), *font));
	_texts.push_back(new sf::Text(to_string(data.var3), *font));
	_texts.push_back(new sf::Text(to_string(data.var4), *font));
	_texts.push_back(new sf::Text(to_string(data.var5), *font));
	for (int i = 0; i < _texts.size(); i++)
	{
		_texts[i]->setPosition(sf::Vector2f(190, 4 * 95 + 30*i));
	}

	setActive(false);
}

Stats::~Stats()
{
	singletonInstance = nullptr;
	for (sf::Text * text : _texts)
	{
		delete text;
	}
	delete _sprite;
}

void Stats::setActive(bool active) //hide/show drawable stuff
{
	int alpha;
	if (active)
	{
		alpha = 255;
		_texts[0]->setString(to_string(data.var1));
		_texts[1]->setString(to_string(data.var2));
		_texts[2]->setString(to_string(data.var3));
		_texts[3]->setString(to_string(data.var4));
		_texts[4]->setString(to_string(data.var5));

		saveToFile();
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
				strPos = (int)line.find("*")+1;
				readString = "";
				while (line[strPos] != '*' && strPos < stringSize)
				{
					if (line[strPos] != '/')
					{
						readString += line[strPos];
					}
					else //save number, reset readString
					{
						std::cout << readString << std::endl;
						fileData.push_back(atoi(readString.c_str()));
						readString = "";
					}
					strPos++;
				}
				reading = false;
			}
		}
		
	}
	data.var1 = fileData[0];
	data.var2 = fileData[1];
	data.var3 = fileData[2];
	data.var4 = fileData[3];
	data.var5 = fileData[4];
}

void Stats::saveToFile()
{
	std::string fileData = "*";
	fileData += to_string(data.var1);
	fileData += "/";
	fileData += to_string(data.var2);
	fileData += "/";
	fileData += to_string(data.var3);
	fileData += "/";
	fileData += to_string(data.var4);
	fileData += "/";
	fileData += to_string(data.var5);
	fileData += "/";
	fileData += "*";

	ofstream outputFile;
	outputFile.open("gameData/data.txt");
	outputFile << fileData << endl;
	outputFile.close();
}