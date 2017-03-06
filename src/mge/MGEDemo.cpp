#include "mge/MGEDemo.hpp"
#include <P3P/Level.hpp>
#include "mge/core/Renderer.hpp"
#include "mge/core/World.hpp"
#include "mge/objects/Camera.hpp"
#include <mge/objects/Light.hpp>
#include <mge/core/Mesh.hpp>
//[TESTING]
#include <mge/behaviours/CameraOrbitBehaviour.hpp>
#include <JCPPEngine/Random.hpp>


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
	//Load large meshes not in hub
	Mesh::load(config::MGE_MODEL_PATH + "ServerRack.obj");
	Mesh::load(config::MGE_MODEL_PATH + "Fan.obj");
	Mesh::load(config::MGE_MODEL_PATH + "PowerBox.obj");
	Mesh::load(config::MGE_MODEL_PATH + "BoxSpot.obj");
	//Load large audio files
	JCPPEngine::SoundManager::GetBuffer ("sounds/BossHurt.wav");
	JCPPEngine::SoundManager::GetBuffer ("sounds/Death.wav");
	JCPPEngine::SoundManager::GetBuffer ("sounds/Unstable.wav");
	JCPPEngine::SoundManager::GetBuffer ("sounds/Fan.wav");
	JCPPEngine::SoundManager::GetBuffer ("sounds/NpcTalking.wav");

	//add camera first (it will be updated last)
	Camera* camera = new Camera ();
	camera->rotate (glm::radians (-45.0f), glm::vec3 (1, 0, 0));
	camera->translate (glm::vec3 (10, 0, 15));
	_world->add (camera);
	_world->setMainCamera (camera);

	//[TESTING]
	Light* dirLight = new Light (glm::vec3 (1, 1, 1), 0.45f);
	dirLight->rotate (glm::radians (25.0f), glm::vec3 (0, 1, 0));
	dirLight->rotate (glm::radians (45.0f), glm::vec3 (1, 0, 0));
	dirLight->setParent (_world);
//	Light* spotLight = new Light (glm::vec3 (1, 0.95f, 0.8f), 5.0f, glm::vec3 (1, 2.5f, 0), glm::radians (60.0f), glm::vec2 (0.25f, 0.75f));
//	spotLight->rotate (glm::radians (210.0f), glm::vec3 (0, 1, 0));
//	spotLight->rotate (glm::radians (35.0f), glm::vec3 (1, 0, 0));
//	spotLight->setParent (_world);
	ShaderDataUtil::SetAmbientLight (glm::vec3 (0.8f, 0.9f, 1), 0.1f);
	//camera->setBehaviour (new CameraOrbitBehaviour (10.0f, glm::radians (-60.0f), glm::radians (60.0f), 0.01f, _world));

	//Set up P3P hub (starts the game)
	_fader = new SceneFader (_window);
	_fader->setParent (_world);
	_game = new Level (JCPPEngine::Random::Range (1, 4), _window);
}

MGEDemo::~MGEDemo()
{
	_game->clear ();
	delete _game;
	delete _fader;
}
