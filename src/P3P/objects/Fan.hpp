#ifndef FAN_Def
#define FAN_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/LitMaterial.hpp>
#include <mge/behaviours/AnimationBehaviour.hpp>
#include <P3P/ButtonTarget.hpp>

class Fan : public ButtonTarget
{
	public:
		Fan(int pX, int pZ, int pXDirection, int pYDirection, bool pReversed);
		void update(float pStep, bool pUpdateWorldTransform = false) override;
		void move(int pX, int pZ);
		bool setActive (bool pActive);
	private:
		GameObject* _model = nullptr;
		int _position[2] = { 0, 0 };
		int _direction[2] = { 0, 0 };
		bool _reversed = false;
		bool _active = false;
		AnimationBehaviour* _animator = nullptr;
		//_visibleArea size is bigger than actual visible area is
		//because we need to store first object outside visible area to check if we can move a box there
		static const int _visibleAreaSize = 6;
		int _visibleArea[_visibleAreaSize];
		int _changeIndex;
		bool checkForChanges();
		void push();
		void pull();
};

#endif
