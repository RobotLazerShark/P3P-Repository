#ifndef FAN_Def
#define FAN_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>

class Fan : public GameObject
{
	public:
		Fan(int pX, int pZ, int pDirection[], bool pReversed);
		void update(float pStep, bool pUpdateWorldTransform = false) override;

		void move(int pX, int pZ);
	private:
		GameObject* _model;
		int _position[2] = { 0, 0 };
		int _direction[2] = { 0, 0 };
		bool _reversed;

		//_visibleArea size is bigger than actual visile are is
		//because we need to store first object outside visible area to check if we can move a box there
		int _visibleArea[6];
		int visibleAreaSize;

		int distanceToArrayBound;

		bool checkForChanges();
		void saveVisibleArea();
		void push();
		void pull();
};

#endif
