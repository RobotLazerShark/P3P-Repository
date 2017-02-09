#ifndef PLAYER_H
#define PLAYER_H

//Include files
#include <array>
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
	~Player () override;
        int _currentTile [2];//Arrays, because glm::vec uses floats
	int _oldTile [2];
        void movePlayer (int pX, int pZ);
	void update (float pStep, bool pUpdateWorldTransform = false) override;
	void ProcessEvent (JCPPEngine::Event* pEvent) override;

    protected:

    private:
        ProgressTracker* _progressTracker;
	GameObject* _model;

};

#endif // PLAYER_H