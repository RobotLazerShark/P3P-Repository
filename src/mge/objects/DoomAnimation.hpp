#ifndef DoomAnimation_Def
#define DoomAnimation_Def


//Include files
#include <mge/objects/DoomObject.hpp>


class DoomAnimation : public DoomObject
{

	public:
		DoomAnimation (std::string pFilename, int pFrameWidth, int pFrameHeight, float pFrameTime);
		DoomAnimation (glm::vec3 pPosition, std::string pFilename, int pFrameWidth, int pFrameHeight, float pFrameTime);
		virtual void update (float pStep, bool pUpdateWorldTransform = false) override;
	private:
		int _columns = 1;
		int _rows = 1;
		int _frameWidth;
		int _frameHeight;
		int _currentFrame = 0;
		int _frameCount = 1;
		float _frameTimer = 0.0f;
		float _frameDelay = 0.1f;

};

#endif