#include "AbstractGame.hpp"
//#include <vld.h>
#include <iostream>
using namespace std;
#include <mge/core/Mesh.hpp>
#include <mge/core/Texture.hpp>
#include <mge/materials/ColorMaterial.hpp>
#include <mge/materials/TextureMaterial.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <mge/materials/TerrainMaterial.hpp>
#include <mge/materials/WobbleMaterial.hpp>
#include <mge/materials/GlitchMaterial.hpp>
#include <JCPPEngine/TextureManager.hpp>
#include <JCPPEngine/FontManager.hpp>
#include <JCPPEngine/InputManager.hpp>
#include "mge/core/World.hpp"
#include <P3P/Level.hpp>
#include <P3P/Menu.hpp>
#include <P3P/SceneFader.hpp>

AbstractGame* AbstractGame::singletonInstance = nullptr;
static float timeSinceProgramStart;
int AbstractGame::windowWidth = 1920;
int AbstractGame::windowHeight = 1080;
int AbstractGame::windowHalfWidth = AbstractGame::windowWidth * 0.5f;
int AbstractGame::windowHalfHeight = AbstractGame::windowHeight * 0.5f;

float AbstractGame::Time ()
{
	return timeSinceProgramStart;
}
void AbstractGame::showCursor (bool pShowCursor)
{
	if (AbstractGame::singletonInstance->_window != nullptr)
	{
		AbstractGame::singletonInstance->_window->setMouseCursorVisible (pShowCursor);
	}
}

void AbstractGame::Stop ()
{
	unregisterForEvent (sf::Event::Closed);
	unregisterForEvent (sf::Event::Resized);
	unregisterForEvent (sf::Event::KeyPressed);
	if (_luaParser != nullptr)
	{
		_luaParser->Clean ();
		delete _luaParser;
		_luaParser = nullptr;
	}
	JCPPEngine::SoundManager::Clean ();
	if (_window != nullptr)
	{
		_window->close ();
	}
 }

AbstractGame::AbstractGame():_window(NULL),_renderer(NULL),_world(NULL), _fps(0)
{
	if (singletonInstance != nullptr)
	{
		delete singletonInstance;
	}
	singletonInstance = this;
	JCPPEngine::InputManager ();
	windowHalfWidth = windowWidth * 0.5f;
	windowHalfHeight = windowHeight * 0.5f;
    //ctor
}

AbstractGame::~AbstractGame()
{
    //dtor
    if (Level::singletonInstance != nullptr)
    {
	delete Level::singletonInstance;
    }
    if (SceneFader::singletonInstance != nullptr)
    {
	delete SceneFader::singletonInstance;
    }
	singletonInstance = nullptr;
    if (_luaParser != nullptr)
    {
	_luaParser->Clean ();
	delete _luaParser;
	_luaParser = nullptr;
    }
    JCPPEngine::SoundManager::Clean ();
    if (_window != nullptr)
    {
	delete _window;
	_window = nullptr;
    }
    delete _renderer;
    delete _world;
    if (Menu::singletonInstance != nullptr)
    {
	delete Menu::singletonInstance;
    }
    Mesh::ClearCache ();
    Texture::ClearCache ();
    ColorMaterial::clearShaderProgram ();
    TextureMaterial::clearShaderProgram();
    LitMaterial::clearShaderProgram();
    TerrainMaterial::clearShaderProgram();
    WobbleMaterial::clearShaderProgram();
    GlitchMaterial::clearShaderProgram ();
}

void AbstractGame::initialize() {
    cout << "Initializing engine..." << endl << endl;

    _initializeWindow();
//    _printVersionInfo();
    _initializeGlew();
    _initializeRenderer();
    _initializeWorld();
    _initializeScene();

	//Set up the event handler
	JCPPEngine::EventHandler::SetEventWindow (_window);
	//Register for events
	registerForEvent (sf::Event::Closed);
	registerForEvent (sf::Event::Resized);
	registerForEvent (sf::Event::KeyPressed);
    cout << endl << "Engine initialized." << endl << endl;
}

///SETUP

void AbstractGame::_initializeWindow() {//FADE TOP ROW OF WALLS ALPHA TO 0
	sf::VideoMode videoMode = sf::VideoMode::getFullscreenModes () [0];
		//sf::VideoMode(1400,800);//[TESTING]
	windowWidth = videoMode.width;
	windowHeight = videoMode.height;
	_window = new sf::RenderWindow( videoMode, "G1¡t©h/Fï×", sf::Style::Fullscreen, sf::ContextSettings(24,8,0,3,3));
//	_window = new sf::RenderWindow( videoMode, "« G1¡t©h/Fï× »", sf::Style::Default, sf::ContextSettings(24,8,0,3,3));//[TESTING]
	windowWidth = videoMode.width;
	windowHeight = videoMode.height;
	_window->setMouseCursorVisible (false);
//	_cursor = new sf::Sprite (*JCPPEngine::TextureManager::GetTexture ("images/Crosshair.png"));
//	_cursor->setOrigin (12, 12);
	_luaParser = new LuaParser (_window);
	_window->setVerticalSyncEnabled(true);

	_window->pushGLStates ();
	_window->draw (sf::Sprite (*JCPPEngine::TextureManager::GetTexture ("images/LoadingScreen1.png")));
	_window->popGLStates ();
	_window->display ();
	//Draw the next image to the drawbuffer, but display it as late as possible
	_window->pushGLStates ();
	_window->draw (sf::Sprite (*JCPPEngine::TextureManager::GetTexture ("images/LoadingScreen2.png")));
	_window->popGLStates ();
}

void AbstractGame::_printVersionInfo() {
	cout << "Context info:" << endl;
    cout << "----------------------------------" << endl;
    //print some debug stats for whoever cares
    const GLubyte *renderer = glGetString( GL_RENDERER );
    const GLubyte *vendor = glGetString( GL_VENDOR );
    const GLubyte *version = glGetString( GL_VERSION );
    const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
    //nice consistency here in the way OpenGl retrieves values
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    printf("GL Vendor : %s\n", vendor);
    printf("GL Renderer : %s\n", renderer);
    printf("GL Version (string) : %s\n", version);
    printf("GL Version (integer) : %d.%d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);

    cout << "----------------------------------" << endl << endl;
}

void AbstractGame::_initializeGlew() {
    //initialize the opengl extension wrangler
    GLint glewStatus = glewInit();
}

void AbstractGame::_initializeRenderer() {
    //setup our own renderer
	_renderer = new Renderer();
}

void AbstractGame::_initializeWorld() {
    //setup the world
	_world = new World();
}

///LOOP

void AbstractGame::run ()
{
	timeSinceProgramStart = 0;
	sf::Clock updateClock;
	sf::Clock renderClock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds (1.0f / 60.0f);
	float step = (1.0f / 60.0f);

	while (_window->isOpen ())
	{
		sf::Time temp = updateClock.restart ();
		timeSinceProgramStart += temp.asSeconds ();
		timeSinceLastUpdate += temp;
		//Update managers
		JCPPEngine::InputManager::Update ();
		JCPPEngine::SoundManager::Update ();
		if (timeSinceLastUpdate > timePerFrame)
		{
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			while (timeSinceLastUpdate > timePerFrame)
			{
				timeSinceLastUpdate -= timePerFrame;
				_update (step);
				//Update the lua program
				if (_luaParser != nullptr && _luaParser->Update (step))
				{
					Stop ();
					break;
				}
			}
			ShaderDataUtil::UpdateCameraInfo ();
			if (Level::singletonInstance != nullptr)
			{
				Level::renderBackground (_window);
				_render ();
				Level::render (_window);
			}
		//	else if (Menu::singletonInstance != nullptr && !Menu::singletonInstance->isHidden)
		//	{
		//		Menu::singletonInstance->render ();
		//		_render ();
		//	}
			else
			{
				_render ();
			}
			if (_luaParser != nullptr)
			{
				_luaParser->Render ();
			}

		//	_cursor->setPosition ((sf::Vector2f)sf::Mouse::getPosition (*_window));//NOTE: cursor has to be hidden
		//	_window->pushGLStates ();
		//	_window->draw (*_cursor);
		//	_window->popGLStates ();
			_window->display ();

			float timeSinceLastRender = renderClock.restart ().asSeconds ();
			if (timeSinceLastRender != 0) _fps = 1.0f / timeSinceLastRender;
		}
		JCPPEngine::EventHandler::ProcessEvents ();
//		if (JCPPEngine::InputManager::GetKeyDown (sf::Keyboard::Escape))
//		{
//			Stop ();
//		}
//		else if (JCPPEngine::InputManager::GetKeyDown (sf::Keyboard::Num1))
//		{
//			_luaParser->Refresh ();
//		}
//		else if (JCPPEngine::InputManager::GetKeyDown (sf::Keyboard::Num2))
//		{
//			_luaParser->SafeRefresh ();
//		}
	}
//	while (!JCPPEngine::InputManager::GetKeyDown (sf::Keyboard::Space))//[TESTING]
//	{
//		//Wait until we close the window, so we can still read error messages.
//		JCPPEngine::InputManager::Update ();
//		JCPPEngine::EventHandler::ProcessEvents ();
//	} 
}

void AbstractGame::_update(float pStep) {
    _world->update(pStep);
}

void AbstractGame::_render () {
    _renderer->render(_world);
}

void AbstractGame::ProcessEvent (sf::Event pEvent)
{
	switch (pEvent.type)
	{
		case sf::Event::Closed:
			Stop ();
			break;
	//	case sf::Event::KeyPressed:
	//		if (pEvent.key.code == sf::Keyboard::Escape)
	//		{
	//			Stop ();
	//		}
	//		break;
		case sf::Event::Resized:
			//would be better to move this to the renderer
			//this version implements nonconstrained match viewport scaling
			glViewport (0, 0, pEvent.size.width, pEvent.size.height);
			break;
		default:
			break;
	}
}