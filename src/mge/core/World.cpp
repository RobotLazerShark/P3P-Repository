#include "mge/core/World.hpp"
#include "mge/util/ShaderDataUtil.hpp"
#include <glm.hpp>

using namespace std;

World* World::singletonInstance = nullptr;

World::World():GameObject("root", glm::vec3 (0,0,0)), _mainCamera(0)
{
	if (singletonInstance != nullptr)
	{
		return;
	}
	singletonInstance = this;
	_partOfWorld = true;
}

void World::setMainCamera (Camera* pCamera) {
    if (pCamera != NULL) _mainCamera = pCamera;
	ShaderDataUtil::SetCamera (pCamera);
}

Camera* World::getMainCamera () {
    return _mainCamera;
}
