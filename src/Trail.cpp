/**
 *  Trail.cpp
 *
 *  Created by Marek Bereza on 17/01/2014.
 */

#include "Trail.h"

#define ROOT_TWO 1.414213

int Trail::MAX_LENGTH = 20;
int Trail::colourPos = 0;
Trail::Trail() {
	colourPos++;
	colourPos %= 6;
	needsDelete = false;
	fadingStep = -1;
	switch (colourPos) {
		case 0:
			colour.set(0, 1, 0);
			break;
		case 1:
			colour.set(1, 0, 0);
			break;
		case 2:
			colour.set(1, 0, 1);
			break;
		case 3:
			colour.set(1, 1, 0);
			break;
		case 4:
			colour.set(0, 1, 1);
			break;
		case 5:
			colour.set(0, 0, 1);
			break;
	}
	loc = IntegratorVec2f();
}

void Trail::smoothTrail() {
	deque<ofVec2f>::reverse_iterator it = points.rbegin();

	while (it != points.rend()) {
		ofVec2f &prev = (*it);
		it++;
		if (it != points.rend()) {
			it++;
			if (it != points.rend()) {
				float h = 0.5;
				(*it) = prev * h + (*it) * (1.f - h);
			}
		}
	}
}

void Trail::update(ofVec2f p) {
	smoothTrail();
	loc.target(p);
	loc.update();

	if (loc.val.x != 0 || loc.val.y != 0) {
		points.push_back(loc.val);
		if (points.size() > 2) {
			ofVec2f first = loc.val;
			ofVec2f second = *(points.begin() + 2);
			ofVec2f diff = first - second;
			diff.rotate(90);
			diff.normalize();
			diff *= 20.0f;
			points.push_back(loc.val + diff);
		}
		while (points.size() > MAX_LENGTH) {
			points.pop_front();
		}
	}
}
void Trail::update2() {
	if (ofRandomuf() < 0.7) return;

	if (points.size() < 1)
		return;

	points.pop_front();
}

void Trail::fadeOut() {
	if (fadingStep == -1) fadingStep = 20;
}

bool Trail::getNeedsDelete() {
	return needsDelete;
}

void Trail::draw() {

	int numPoints = points.size();
	if (numPoints == 0) return;
	int i = 0;
	ofSetColor(colour);
	ofNoFill();

	//glBegin(GL_LINE_STRIP);
	mesh.clear();
	//ofSetLineWidth(50);
	//glLineWidth(75.0f);

	//ofSetLineWidth(10);
	for (auto point : points) {


	}
	deque<ofVec2f>::iterator it;
	it = points.begin();
	float alphaLoss = 0;
	if (fadingStep > 0) {
		alphaLoss = ofMap(fadingStep, 20, 0, 0, 1, true);
		fadingStep--;
	}
	else if (fadingStep == 0) {
		alphaLoss = ofMap(fadingStep, 20, 0, 0, 1, true);
		fadingStep--;
		needsDelete = true;
	}
	while (it + 1 != points.end() && it != points.end()) {
		colour.a = ofMap(i, numPoints, 0, 1, 0, true) - alphaLoss;
		colour.a = (colour.a < 0.0f) ? 0.0f : colour.a;
		mesh.addColor(colour);
		mesh.addVertex(*it);
		it++;
		i++;
	}

	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	mesh.draw();

	//fatLine.draw();
	//fatLine.printDebug();

	//glEnd();
	//shader.begin();
	//shader.setUniform1f("u_width", ofGetWidth());
	//shader.setUniform1f("u_height", ofGetHeight());
	//shader.setUniform1f("u_thickness", 20);
	//mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	//mesh.draw();
	//shader.end();
	
	ofFill();
}


