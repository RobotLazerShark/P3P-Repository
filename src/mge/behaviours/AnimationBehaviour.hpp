#ifndef AnimationBehaviour_Def
#define AnimationBehaviour_Def


//Include files
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm.hpp>
#include <mge/behaviours/AbstractBehaviour.hpp>
#include <mge/core/GameObject.hpp>


//Data struct
struct KeyFrame
{
	public:
		float time;
		glm::vec3 position = glm::vec3 (0, 0, 0);
		glm::vec3 rotation = glm::vec3 (0, 0, 0);
		glm::vec3 scale = glm::vec3 (1, 1, 1);
};

//Behaviour that plays animations.
class AnimationBehaviour : public AbstractBehaviour
{
	public:
		AnimationBehaviour (std::vector <std::string> pAnimFiles, bool pResetTransform = true);
		void update (float pStep) override;
		void playAnimation (int pAnimIndex, bool pLoop = false, void (*pFuncPtr) (int, GameObject*) = nullptr, GameObject* pFuncOwner = nullptr);
		int currentAnimation ();
		bool looping ();
		void stopAnimation ();
		bool isPlaying ();
		bool isPlaying (int pAnimIndex);
		float getDuration (int pAnimIndex);
	private:
		bool _loop = false;
		bool _playing = false;
		bool _resetTransform = true;
		float _time = 0;
		int _currentAnimation = -1;
		int _lastFrame = 0;
		std::vector <std::vector <KeyFrame>> _animations;
		glm::mat4 _normalTransform;
		void (*_stopFunction) (int, GameObject*) = nullptr;
		GameObject* _stopFunctionOwner;
};

#endif