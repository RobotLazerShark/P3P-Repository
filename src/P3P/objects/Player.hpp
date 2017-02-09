<<<<<<< HEAD
#ifndef PLAYER_H
#define PLAYER_H

//Include files
#include <SFML/Graphics.hpp>
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <JCPPEngine/AbstractListener.hpp>
#include <JCPPEngine/KeyEvent.hpp>
#include <JCPPEngine/InputManager.hpp>
#include <P3P/ProgressTracker.hpp>


class Player : public GameObject, public JCPPEngine::AbstractListener
{
    public:
        Player (int pX, int pZ, ProgressTracker* pProgressTracker);
        glm::vec2 _currentTile;
	glm::vec2 _oldTile;
        void movePlayer (int pX, int pZ);
	void update (float pStep, bool pUpdateWorldTransform = false) override;
	void ProcessEvent (JCPPEngine::Event* pEvent) override;

    protected:

    private:
        ProgressTracker* _progressTracker;
	GameObject* _model;

};

=======
#ifndef PLAYER_H
#define PLAYER_H

//Include files
#include <SFML/Graphics.hpp>
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <JCPPEngine/AbstractListener.hpp>
#include <JCPPEngine/KeyEvent.hpp>
#include <JCPPEngine/InputManager.hpp>
#include <P3P/ProgressTracker.hpp>


class Player : public GameObject, public JCPPEngine::AbstractListener
{
    public:
        Player (int pX, int pZ, ProgressTracker* pProgressTracker);
        glm::vec2 _currentTile;
	glm::vec2 _oldTile;
        void movePlayer (int pX, int pZ);
	void update (float pStep, bool pUpdateWorldTransform = false) override;
	void ProcessEvent (JCPPEngine::Event* pEvent) override;

    protected:

    private:
        ProgressTracker* _progressTracker;
	GameObject* _model;

};

>>>>>>> 05a65edaed0d726361052543950587d121f9c233
#endif // PLAYER_H