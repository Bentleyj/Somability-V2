#pragma once

#include "ofMain.h"

#define DAMPING 0.5f
#define ATTRACTION 0.5f

class IntegratorVec2f
{
public:
	IntegratorVec2f(void);
	IntegratorVec2f(ofVec2f _val);
	IntegratorVec2f(ofVec2f _val, float _damping, float _attraction);
	void set(ofVec2f _val);
	void setX(float _val);
	void setY(float _val);
	void setZ(float _val);

	void update(void);
	void target(ofVec2f _tar);
	void noTarget();
	~IntegratorVec2f(void);

	ofVec2f val;
	ofVec2f vel;
	ofVec2f acc;
	ofVec2f force;
	ofVec2f tar;
	float mass;

	float damping;
	float attraction;
	bool targeting;

};

