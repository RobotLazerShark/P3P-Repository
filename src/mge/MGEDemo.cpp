#include "mge/MGEDemo.hpp"
//[TESTING]
#include <mge/behaviours/CameraOrbitBehaviour.hpp>


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
	Camera* camera = new Camera ();
	camera->rotate (glm::radians (-45.0f), glm::vec3 (1, 0, 0));
	camera->translate (glm::vec3 (10, 0, 15));
	_world->add (camera);
	_world->setMainCamera (camera);

	//[TESTING]
	Light* dirLight = new Light (glm::vec3 (1, 1, 1), 0.4f);
	dirLight->rotate (glm::radians (25.0f), glm::vec3 (0, 1, 0));
	dirLight->rotate (glm::radians (45.0f), glm::vec3 (1, 0, 0));
	dirLight->setParent (_world);
	ShaderDataUtil::SetAmbientLight (glm::vec3 (1, 1, 1), 0.15f);
	//camera->setBehaviour (new CameraOrbitBehaviour (10.0f, glm::radians (-60.0f), glm::radians (60.0f), 0.01f, _world));

	//Set up P3P hub (starts the game)
	_game = new Level (0);
}

MGEDemo::~MGEDemo()
{
	_game->clear ();
	delete _game;
}
