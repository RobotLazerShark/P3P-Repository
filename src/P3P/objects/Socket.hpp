#ifndef SOCKET_Def
#define SOCKET_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <P3P/objects/Plug.hpp>
#include <P3P/ButtonTarget.hpp>
#include <P3P/Stats.hpp>

class Socket : public GameObject
{
	public:
		Socket(int pX, int pZ, int pOrientation, int pTargetX, int pTargetY);
		void update(float pStep, bool pUpdateWorldTransform = false) override;

		GameObject* _model = nullptr;

	private:
		LitMaterial * offMaterial = new LitMaterial("Socket.jpg");
		LitMaterial * onMaterial = new LitMaterial("SocketOn.jpg");
		int _orientation = 0;
		int _currentTile[2] = { 0, 0 };
		bool activated = false;
		int _targetX = 0;
		int _targetY = 0;
};

#endif