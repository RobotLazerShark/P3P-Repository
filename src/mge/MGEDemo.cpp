#include "mge/MGEDemo.hpp"


//construct the game class into _window and _renderer (other parts are initialized by build)
MGEDemo::MGEDemo():AbstractGame ()
{
}

void MGEDemo::initialize() {
    //setup the core part
    AbstractGame::initialize();
}

//build the game _world
void MGEDemo::_initializeScene()
{
	_renderer->setClearColor (0,0,0);

	//add camera first (it will be updated last)
	Camera* camera = new Camera ("Camera", glm::vec3(0.5,0.5,10));
	_world->add (camera);
	_world->setMainCamera (camera);

	LevelImporter::ReadFile ("Level.tmx");
}

MGEDemo::~MGEDemo()
{
	//dtor
}
