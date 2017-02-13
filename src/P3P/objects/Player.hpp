#ifndef PLAYER_H
#define PLAYER_H

//Include files
#include <array>
#include <SFML/Graphics.hpp>
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <mge/behaviours/AnimationBehaviour.hpp>
#include <JCPPEngine/AbstractListener.hpp>
#include <JCPPEngine/KeyEvent.hpp>
#include <JCPPEngine/InputManager.hpp>
#include <P3P/ProgressTracker.hpp>


class Player : public GameObject, public JCPPEngine::AbstractListener
{
    public:
        Player (int pX, int pZ, ProgressTracker* pProgressTracker);
	~Player () override;
        bool movePlayer (int pX, int pZ, bool pTranslate = true);
	void ProcessEvent (JCPPEngine::Event* pEvent) override;
	static Player* singletonInstance;
	//Preferably these variables would be private, but they are needed in a globalspace function
	bool _moving = false;
	int _currentTile[2];//Arrays, because glm::vec uses floats
	int _oldTile[2];

    protected:

    private:
        ProgressTracker* _progressTracker;
	GameObject* _model;
	glm::vec3 _modelOrientation = glm::vec3 (0, 0, 0);
	AnimationBehaviour* _wheelAnimator;
	AnimationBehaviour* _baseAnimator;

};

#endif // PLAYER_H