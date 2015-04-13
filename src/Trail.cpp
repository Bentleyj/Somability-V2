/**
 *  Trail.cpp
 *
 *  Created by Marek Bereza on 17/01/2014.
 */

#include "Trail.h"

int Trail::MAX_LENGTH = 90;
int Trail::colourPos = 0;
Trail::Trail() {
	colourPos++;
	colourPos %= 6;
	switch(colourPos) {
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
}


void Trail::smoothTrail() {
	deque<ofVec2f>::reverse_iterator it = points.rbegin();
		
	while(it!=points.rend()) {
		ofVec2f &prev = (*it);
		it++;
		if(it!=points.rend()) {
			float h = 0.5;
			(*it) = prev * h + (*it) * (1.f - h);
		}
	}
}
	
void Trail::update(ofVec2f p) {
	smoothTrail();
	if(p.x!=0 || p.y !=0) {
		points.push_back(p);
	}
	while(points.size()>MAX_LENGTH) {
		points.pop_front();
	}
	

}
void Trail::update2() {
	if(ofRandomuf()<0.7) return;

	if (points.size() < 1)
		return;

	points.pop_front();
}

void Trail::draw() {
	
	int numPoints = points.size();
	if(numPoints==0) return;
	int i =0 ;
	ofSetColor(colour);
	ofNoFill();

	//glBegin(GL_LINE_STRIP);
	mesh.clear();
	//ofSetLineWidth(50);
	//glLineWidth(75.0f);

	deque<ofVec2f>::iterator it;
	it = points.begin();
	while(it!=points.end()) {
//		if(it._M_cur==NULL) break;
//		glColor4f(colour.r, colour.g, colour.b, alpha);
//		glVertex2f((*it).x,(*it).y);
		colour.a = ofMap(i, numPoints, 0, 1, 0, true);
		mesh.addColor(colour);
		mesh.addVertex((*it));
		it++;
		i++;
	}
	
	//glEnd();
	mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
	mesh.draw();
	
	ofFill();
}


