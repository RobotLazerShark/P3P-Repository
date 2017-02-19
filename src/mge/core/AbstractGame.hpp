#ifndef ABSTRACTGAME_H
#define ABSTRACTGAME_H

#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <string>
#include <glm.hpp>
#include <mge/lua/LuaParser.hpp>
#include <mge/util/ShaderDataUtil.hpp>
#include <JCPPEngine/AbstractListener.hpp>
#include <JCPPEngine/EventHandler.hpp>
#include <JCPPEngine/TextureManager.hpp>
#include <JCPPEngine/FontManager.hpp>
#include "mge/core/Renderer.hpp"
using namespace std;

class World;
class Renderer;
class LuaParser;

/**
 * Defines the basic structure for a game, with defaults already filled in.
 * Your main game loop should run:
 *
 *  initialize
 *  run
 *
 * in order.
 */


class AbstractGame : public JCPPEngine::AbstractListener
{
    public:

        AbstractGame();
        virtual ~AbstractGame();

        static float Time ();
	static int windowWidth;
	static int windowHeight;
	static int windowHalfWidth;
	static int windowHalfHeight;
	static AbstractGame* singletonInstance;
	void Stop ();

        //creates a window, initializes glew, a renderer and a world instance
        virtual void initialize();
        //run the actual process of updating all objects, rendering them and processing events
        virtual void run();
        //process any sfml window events
        virtual void ProcessEvent (sf::Event pEvent);

    protected:

        //methods above delegate behaviour to the methods below so that you can override it in a subclass

        //initialize sfml rendering context
        virtual void _initializeWindow();
        //print info about the current driver version etc
        virtual void _printVersionInfo();
        //initialize the extension wrangler
        virtual void _initializeGlew();
        //create our own custom renderer instance
        virtual void _initializeRenderer();
        //initialize a scene root to which we can attach/add objects
        virtual void _initializeWorld();

        //initialize the actual scene, to be done by a subclass
        virtual void _initializeScene() = 0;

        //call update on all game objects in the display root
        virtual void _update(float pStep);
        //render all game objects in the display root
        virtual void _render();

		sf::RenderWindow* _window = nullptr;  //sfml window to render into
		Renderer* _renderer = nullptr;        //the renderer class to render the world
		World* _world = nullptr;              //the root game object that represents our scene
		float _fps = 0;                 //stores the real fps

    private:
	sf::Sprite* _cursor = nullptr;
	LuaParser* _luaParser = nullptr;
        AbstractGame(const AbstractGame&);
        AbstractGame& operator=(const AbstractGame&);


};

#endif // ABSTRACTGAME_H
