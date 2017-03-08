#include <P3P/Background.hpp>
#include <JCPPEngine/Random.hpp>
#include <JCPPEngine/FontManager.hpp>

#include <iostream>
//Constructor
Background::Background ()
{
	_text = new sf::Text ("", *JCPPEngine::FontManager::GetFont ("fonts/Font1.ttf"), 15);
	_text->setPosition (10, 15);
	_text->setFillColor (sf::Color (0, 255, 0, 60));
	randomize ();
}
Background::~Background ()
{
	delete _text;
}

//Randomize matrix
void Background::randomize ()
{
	std::string temp;
	for (int y = 0; y < CHARCOUNT_Y; y++)
	{
		for (int x = 0; x < CHARCOUNT_X; x ++)
		{
			int rnd = JCPPEngine::Random::Range(0, 49);
			if (rnd > 19)
			{
				temp = " ";
			}
			else
			{
				temp = _chars [rnd];
			}
			_matrix [x] [y] = temp;
		}
	}
}
//Move everything one line down
void Background::moveLines ()
{
	std::string temp1;
	std::string temp2;
	int x1 = JCPPEngine::Random::Range (0, CHARCOUNT_X - 1);
	int x2 = JCPPEngine::Random::Range (0, CHARCOUNT_X - 1);
	for (int y = CHARCOUNT_Y - 1; y >= 0; y--)
	{
		if (y == 0)
		{
			int rnd1 = JCPPEngine::Random::Range(0, 49);
			int rnd2 = JCPPEngine::Random::Range (0,49);
			temp1 = (rnd1 > 19) ? " " : _chars [rnd1];
			temp2 = (rnd2 > 19) ? " " : _chars [rnd2];
		}
		else
		{
			temp1 = _matrix [x1] [y-1];
			temp2 = _matrix [x2] [y-1];
		}
		_matrix [x1] [y] = temp1;
		_matrix [x2] [y] = temp2;
	}
}

//Render background
void Background::render (sf::RenderWindow* pWindow)
{
	std::string text = "";
	for (int y = 0; y < CHARCOUNT_Y; y++)
	{
		for (int x = 0; x < CHARCOUNT_X; x ++)
		{
			text += _matrix [x] [y];
		}
		text += "\n";
	}
	_text->setString (text);
	moveLines ();

	pWindow->pushGLStates ();
	pWindow->draw (*_text);
	pWindow->popGLStates ();
}