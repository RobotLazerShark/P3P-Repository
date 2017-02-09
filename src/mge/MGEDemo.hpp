#ifndef MGEDEMO_H
#define MGEDEMO_H


//Include files#include <glm.hpp>
#include <iostream>
#include <string>
#include "mge/core/Renderer.hpp"
#include "mge/core/World.hpp"
#include "mge/objects/Camera.hpp"
#include <P3P/Level.hpp>
#include <mge/core/AbstractGame.hpp>

class MGEDemo: public AbstractGame
{
	//PUBLIC FUNCTIONS
	public:
		MGEDemo();
		virtual ~MGEDemo();
		virtual void initialize();
	protected:
		virtual void _initializeScene();
	private:
		MGEDemo(const MGEDemo&);
		MGEDemo& operator=(const MGEDemo&);
};

#endif // MGEDEMO_H
