#ifndef PLAYER_H
#define PLAYER_H

//Include files
#include <array>
#include <SFML/Graphics.hpp>
#include <mge/core/GameObject.hpp>
#include <mge/behaviours/AnimationBehaviour.hpp>
#include <JCPPEngine/AbstractListener.hpp>
#include <P3P/ProgressTracker.hpp>

class Collectable;


class Player : public GameObject, public JCPPEngine::AbstractListener
{
    public:
        Player (int pX, int pZ, ProgressTracker* pProgressTracker, int pSkin);
		~Player () override;
        bool movePlayer (int pX, int pZ, bool pAnimate = true);
        bool movePlayer (int pX, int pZ, bool pAnimate, void (*pFuncPtr) (int, GameObject*), GameObject* pFuncOwner);
		void stopAnimation ();
		void ProcessEvent (JCPPEngine::Event* pEvent) override;
		void die ();
		static Player* singletonInstance;
		std::vector <std::string> inventory;
		bool hasItem (std::string pItemName);
		void setDirection (int pDirection [2]);
		//Preferably these variables would be private, but they are needed in a globalspace function
		//They use the private name conventions because they should be used as such
		bool _noMove = false;
		bool _questFinished = false;
		bool _dead = false;
		int _currentTile[2];//Arrays, because glm::vec uses floats
		int _oldTile[2];
		GameObject* _funcOwner = nullptr;
		void (*_stopFunc) (int, GameObject*) = nullptr;
		int _movementToComplete [2] = { 0, 0 };
		bool blockMovement = false;
		glm::vec3 _modelOrientation = glm::vec3 (0, 0, 0);
		AnimationBehaviour* _rotationAnimator = nullptr;
	protected:

    private:
        ProgressTracker* _progressTracker = nullptr;
		GameObject* _model = nullptr;
		GameObject* _shadow = nullptr;
		AnimationBehaviour* _wheelAnimator = nullptr;
		AnimationBehaviour* _baseAnimator = nullptr;
};

#endif // PLAYER_H