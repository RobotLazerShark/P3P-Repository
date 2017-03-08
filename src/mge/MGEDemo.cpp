#include "mge/MGEDemo.hpp"
#include <P3P/Level.hpp>
#include <P3P/SceneFader.hpp>
#include "mge/core/Renderer.hpp"
#include "mge/core/World.hpp"
#include "mge/objects/Camera.hpp"
#include <mge/objects/Light.hpp>
#include <mge/core/Mesh.hpp>
#include <P3P/Menu.hpp>


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
	//Load coomon/large meshes not in hub
	Mesh::load(config::MGE_MODEL_PATH + "ServerRack.obj");
	Mesh::load(config::MGE_MODEL_PATH + "FanBase.obj");
	Mesh::load(config::MGE_MODEL_PATH + "FanBlades.obj");
	Mesh::load(config::MGE_MODEL_PATH + "PowerBox.obj");
	Mesh::load(config::MGE_MODEL_PATH + "BoxSpot.obj");
	Mesh::load(config::MGE_MODEL_PATH + "DoorBarsLeft.obj");
	Mesh::load(config::MGE_MODEL_PATH + "DoorBarsRight.obj");
	Mesh::load(config::MGE_MODEL_PATH + "DoorLeft.obj");
	Mesh::load(config::MGE_MODEL_PATH + "DoorRight.obj");
	Mesh::load(config::MGE_MODEL_PATH + "DoorSupport.obj");
	//Load common/large audio files
	JCPPEngine::SoundManager::GetBuffer ("sounds/BossHurt.wav");
	JCPPEngine::SoundManager::GetBuffer ("sounds/Death.wav");
	JCPPEngine::SoundManager::GetBuffer ("sounds/Spawn.wav");
	JCPPEngine::SoundManager::GetBuffer ("sounds/DoorOpening.wav");
	JCPPEngine::SoundManager::GetBuffer ("sounds/Unstable.wav");
	JCPPEngine::SoundManager::GetBuffer ("sounds/Fan.wav");
	JCPPEngine::SoundManager::GetBuffer ("sounds/NpcTalking.wav");
	JCPPEngine::SoundManager::GetBuffer ("sounds/PlayerMoving.wav");
	JCPPEngine::SoundManager::GetBuffer ("sounds/PowerBoxMoving.wav");
	JCPPEngine::SoundManager::GetBuffer ("sounds/BoxMoving.wav");
	JCPPEngine::SoundManager::GetBuffer ("sounds/PlugMoving.wav");

	//add camera first (it will be updated last)
	Camera* camera = new Camera ();
	camera->rotate (glm::radians (-45.0f), glm::vec3 (1, 0, 0));
	camera->translate (glm::vec3 (10, 0, 15));
	_world->add (camera);
	_world->setMainCamera (camera);

	_window->display ();

//	_menu = new Menu (camera, _window, _world);
//	_menu->setParent (_world);//Menu crashes game on escape exit (is world deleted?) Menu's skinindex is not updated.

	//Set up P3P hub (starts the game)
	_fader = new SceneFader (_window);
	_fader->setParent (_world);
	_game = new Level (1, _window);
}

MGEDemo::~MGEDemo()
{
	_game->clear ();
	delete _game;
	delete _fader;
//	delete _menu;
}
