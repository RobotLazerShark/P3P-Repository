#include "AbstractGame.hpp"

#include <iostream>
using namespace std;

#include "mge/core/Renderer.hpp"
#include "mge/core/World.hpp"

static float timeSinceProgramStart;
int AbstractGame::windowWidth;
int AbstractGame::windowHeight;
int AbstractGame::windowHalfWidth = AbstractGame::windowWidth * 0.5f;
int AbstractGame::windowHalfHeight = AbstractGame::windowHeight * 0.5f;

float AbstractGame::Time ()
{
	return timeSinceProgramStart;
}

AbstractGame::AbstractGame():_window(NULL),_renderer(NULL),_world(NULL), _fps(0)
{
	JCPPEngine::InputManager ();
	windowHalfWidth = windowWidth * 0.5f;
	windowHalfHeight = windowHeight * 0.5f;
    //ctor
}

AbstractGame::~AbstractGame()
{
    //dtor
    delete _window;
    delete _renderer;
    delete _world;
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

void AbstractGame::_initializeWindow() {
	sf::VideoMode videoMode = //sf::VideoMode::getFullscreenModes () [0];
		sf::VideoMode(1400,800);//[TESTING]
//	_window = new sf::RenderWindow( videoMode, "G1¡t©h/Fï×", sf::Style::Fullscreen, sf::ContextSettings(24,8,0,3,3));
	_window = new sf::RenderWindow( videoMode, "« G1¡t©h/Fï× »", sf::Style::Default, sf::ContextSettings(24,8,0,3,3));//[TESTING]
	windowWidth = videoMode.width;
	windowHeight = videoMode.height;
	_window->setMouseCursorVisible (false);
//	_cursor = new sf::Sprite (*JCPPEngine::TextureManager::GetTexture ("images/Crosshair.png"));
//	_cursor->setOrigin (12, 12);
	_luaParser = new LuaParser (_window);
	//_window->setVerticalSyncEnabled(true);
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
				if (_luaParser->Update (step))
				{
					delete _luaParser;
					_luaParser = nullptr;//LuaParser is deleted, prevent accessing invalid memory address
					break;
				}
			}
			ShaderDataUtil::UpdateCameraInfo ();
			_render ();
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
		if (JCPPEngine::InputManager::GetKeyDown (sf::Keyboard::Escape))
		{
			_luaParser->Clean ();
			JCPPEngine::SoundManager::Clean ();
			JCPPEngine::TextureManager::Clean ();
			_window->close ();
		}
		else if (JCPPEngine::InputManager::GetKeyDown (sf::Keyboard::Num1))
		{
			_luaParser->Refresh ();
		}
		else if (JCPPEngine::InputManager::GetKeyDown (sf::Keyboard::Num2))
		{
			_luaParser->SafeRefresh ();
		}
	}
	while (!JCPPEngine::InputManager::GetKeyDown (sf::Keyboard::Space))
	{
		//Wait until we close the window, so we can still read error messages.
		JCPPEngine::InputManager::Update ();
	}
}

void AbstractGame::_update(float pStep) {
    _world->update(pStep);
}

void AbstractGame::_render () {
    _renderer->render(_world);
}

void AbstractGame::ProcessEvent (sf::Event pEvent)
{
	bool exit = false;

	switch (pEvent.type)
	{
		case sf::Event::Closed:
			exit = true;
			break;
		case sf::Event::KeyPressed:
			if (pEvent.key.code == sf::Keyboard::Escape)
			{
				exit = true;
			}
			break;
		case sf::Event::Resized:
			//would be better to move this to the renderer
			//this version implements nonconstrained match viewport scaling
			glViewport (0, 0, pEvent.size.width, pEvent.size.height);
			break;
		default:
			break;
	}

	if (exit)
	{
		_window->close ();
	}
}