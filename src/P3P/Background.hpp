#ifndef Background_Def
#define Background_Def


//Include files
#include <SFML/Graphics.hpp>
#include <string>


class Background
{
	private:
		sf::Text* _text = nullptr;
		static const int CHARCOUNT_X = 173;
		static const int CHARCOUNT_Y = 46;
		std::string _matrix [CHARCOUNT_X] [CHARCOUNT_Y];
		std::string _chars [20] =
		{
			"0","1","2","3","4","5","6","7","8","9",
			"M","J","X","Y","Z","|","/","?","!","~"
		};
		void randomize ();
		void moveLines ();
	public:
		Background ();
		~Background ();
		void render (sf::RenderWindow* pWindow);
};

#endif