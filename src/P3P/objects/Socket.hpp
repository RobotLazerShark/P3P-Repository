#ifndef SOCKET_Def
#define SOCKET_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <P3P/objects/Plug.hpp>
#include <P3P/ButtonTarget.hpp>

class Socket : public GameObject
{
	public:
		Socket(int pX, int pZ, int pOrientation);
		void update(float pStep, bool pUpdateWorldTransform = false) override;

		GameObject* _model = nullptr;

	private:
		LitMaterial * offMaterial = new LitMaterial("Socket.jpg");
		LitMaterial * onMaterial = new LitMaterial("SocketOn.jpg");
		int _orientation = 0;
		int _currentTile[2] = { 0, 0 };
		bool activated = false;
		ButtonTarget * target = nullptr;
};

#endif