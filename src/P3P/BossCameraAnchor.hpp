#ifndef BOSSCAMERAANCHOR_H
#define BOSSCAMERAANCHOR_H

//Include files
#include <mge/core/GameObject.hpp>

class BossCameraAnchor
{
public:
	BossCameraAnchor(int posX, int posY, int posZ, int rangeXmin, int rangeXmax, int rangeYmin, int rangeYmax, int pControlsMode);
	glm::vec3 position;
	glm::vec2 rangeX;
	glm::vec2 rangeY;

	int controlsMode = 0;
};

#endif 