#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cmath>
#include "ShaderUtil.h"
#include "mge/core/AbstractGame.hpp"
#include "mge/MGEDemo.hpp"
using namespace std;
using namespace sf;


int main ()
{
	//Run the (modified) demo
	AbstractGame* game = new MGEDemo ();
	game->initialize ();
	game->run ();
	delete game;

	//Assume program ran succesfully and exit
	return 0;
}