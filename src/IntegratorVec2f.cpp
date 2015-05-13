#include "IntegratorVec2f.h"


IntegratorVec2f::IntegratorVec2f(void)
{
	val = ofVec2f(0, 0);
	mass = 1;
	damping = DAMPING;
	attraction = ATTRACTION;
}

IntegratorVec2f::IntegratorVec2f(ofVec2f _val) {
	val = _val;
	mass = 1;
	damping = DAMPING;
	attraction = ATTRACTION;
}

IntegratorVec2f::IntegratorVec2f(ofVec2f _val, float _damping, float _attraction) {
	val = _val;
	damping = _damping;
	attraction = _attraction;
	mass = 1;
}

void IntegratorVec2f::set(ofVec2f _val) {
	val = _val;
}

void IntegratorVec2f::setX(float _val) {
	val.x = _val;
}

void IntegratorVec2f::setY(float _val) {
	val.y = _val;
}

void IntegratorVec2f::update(void) {
	if(targeting) {
		force += attraction * (tar - val);
	}
	acc = force / mass;
	vel = (vel + acc) * damping;
	val += vel;

	force = ofVec2f(0, 0);
}

void IntegratorVec2f::target(ofVec2f _tar) {
	targeting = true;
	tar = _tar;
}
	
void IntegratorVec2f::noTarget() {
	targeting = false;
}


IntegratorVec2f::~IntegratorVec2f(void)
{
}
