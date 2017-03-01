#include <P3P/Background.hpp>
#include <JCPPEngine/Random.hpp>
#include <JCPPEngine/FontManager.hpp>


//Constructor
Background::Background ()
{
	_text = new sf::Text ("", *JCPPEngine::FontManager::GetFont ("fonts/Font1.ttf"), 15);
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
	std::string temp;
	int x = JCPPEngine::Random::Range (0, CHARCOUNT_X - 1);
	for (int y = CHARCOUNT_Y - 1; y >= 0; y--)
	{
		if (y == 0)
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
		}
		else
		{
			temp = _matrix [x] [y-1];
		}
		_matrix [x] [y] = temp;
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
	moveLines ();

	pWindow->pushGLStates ();
	pWindow->draw (*_text);
	pWindow->popGLStates ();
}