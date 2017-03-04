#include "P3P/BossCameraAnchor.hpp"

BossCameraAnchor::BossCameraAnchor(int posX, int posY, int posZ, int rangeXmin, int rangeXmax, int rangeYmin, int rangeYmax, int pControlsMode)
{
	
	std::cout << posX << "posX" << std::endl;
	std::cout << posY << "posY" << std::endl;
	std::cout << posZ << "posZ" << std::endl;

	std::cout << rangeXmin << "Xmin" << std::endl;
	std::cout << rangeXmax << "Xmax" << std::endl;

	std::cout << rangeYmin << "Ymin" << std::endl;
	std::cout << rangeYmax << "Ymax" << std::endl;

	std::cout << pControlsMode << "pControlsMode" << std::endl << std::endl;
	

	position = glm::vec3(posX, posY, posZ);
	rangeX = glm ::vec2(rangeXmin, rangeXmax);
	rangeY = glm::vec2(rangeYmin, rangeYmax);

	controlsMode = pControlsMode;
}
