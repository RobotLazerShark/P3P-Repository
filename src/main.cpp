#include "mge/MGEDemo.hpp"


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