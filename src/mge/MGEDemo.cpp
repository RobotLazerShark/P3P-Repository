#include "mge/MGEDemo.hpp"
#include <mge/behaviours/CameraOrbitBehaviour.hpp>//[TESTING]


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
	_world->add (camera);
	_world->setMainCamera (camera);

	//[TESTING]
	GameObject* plane = new GameObject ("plane.obj");
	plane->setMaterial (new LitMaterial ("Default.png"));
	plane->setParent (_world);
	Light* dirLight = new Light (glm::vec3 (1, 1, 1), 0.6f);
	dirLight->rotate (glm::radians (45.0f), glm::vec3 (0, 1, 0));
	dirLight->rotate (glm::radians (45.0f), glm::vec3 (1, 0, 0));
	dirLight->setParent (_world);
	ShaderDataUtil::SetAmbientLight (glm::vec3 (1, 1, 1), 0.2f);
	camera->setBehaviour (new CameraOrbitBehaviour (10.0f, glm::radians (-60.0f), glm::radians (60.0f), 0.01f, plane));

	Level* level = new Level (1);
//	LevelMap* map = LevelImporter::ReadFile ("Level.tmx");
//	map->loadLevel ();
}

MGEDemo::~MGEDemo()
{
	//dtor
}
