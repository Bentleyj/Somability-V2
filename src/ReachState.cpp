/*
 *  ReachState.cpp
 *
 *  based on ReachState.cpp Copyright (c) 2011, Neil Mendoza, http://www.neilmendoza.com
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of 16b.it nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "ReachState.h"

#define MAX_SHAPE_AGE 20


void ReachState::setup() {
	setupEvents();
	/*handTouching[LEFT_HAND] = NO_SHAPE;
	handTouching[RIGHT_HAND] = NO_SHAPE;
	shapeSize = 1;
	

	shapeImages[CIRCLE].loadImage("img/circle.png");
	shapeImages[SQUARE].loadImage("img/square.png");
	shapeImages[HEXAGON].loadImage("img/hexagon.png");
	shapeImages[TRIANGLE].loadImage("img/triangle.png");
	
	map<ShapeID, ofImage>::iterator it;
	
	for(it = shapeImages.begin(); it != shapeImages.end(); it++) {
		(*it).second.setAnchorPercent(0.5, 0.5);
	}*/
}

//void ReachState::setupGui(SomabilityGui *gui) {
//	gui->addSlider("Shape size", shapeSize, 0.5, 1.5);
//}
//
//bool ReachState::shapeIsTooOld(float currTime, ofxBox2dBaseShape *shape) {
//	if(data.find(shape)!=data.end()) {
//		return (currTime - data[shape].birthday)>MAX_SHAPE_AGE;
//	}
//	return false;
//}

void ReachState::update()
{
	
//	// y position of the triggers, 2/9ths down the screen
//	float yy = HEIGHT*2.f/9.f;
//	triggers.clear();
//	for(int i = 0; i < NUM_SHAPES; i++) {
//		
//		float xpos = ofMap(i, -0.5, NUM_SHAPES-0.5, 0, WIDTH);
////		ofLine(xpos, 0, xpos, 480);
//		ofRectangle r;
//		float w = shapeSize*WIDTH/7.f;
//		r.setFromCenter(xpos, yy, w, w);
//		triggers.push_back(make_pair((ShapeID)i, r));
//	}
//	int numUsers = getSharedData().openNIDevice.getNumTrackedUsers();
//	// FIX THIS
//	
//	ofVec2f skelScale(WIDTH/getSharedData().openNIDevice.getWidth(), HEIGHT/getSharedData().openNIDevice.getHeight());
//	for(int i = 0; i < numUsers; i++) {
//	
//		ofxOpenNIUser &user = getSharedData().openNIDevice.getTrackedUser(i);
//		ofxOpenNIJoint &j1 = user.getJoint(JOINT_LEFT_HAND);
//		ofxOpenNIJoint &j2 = user.getJoint(JOINT_RIGHT_HAND);
//		
//		ofVec2f ap = j1.getProjectivePosition()*skelScale;
//		ofVec2f bp = j2.getProjectivePosition()*skelScale;
//
//		if(ap.x!=0) handMoved(ap, LEFT_HAND);
//		if(bp.x!=0) handMoved(bp, RIGHT_HAND);
//		
//	}
//	
//	
//	
//	
//    getSharedData().box2d->update();
//	if(ofGetFrameNum()%10==0) {
//		if(handTouching[LEFT_HAND]!=NO_SHAPE) {
//			addShape(handTouching[LEFT_HAND], ofVec2f(ofRandom(0, WIDTH), 1));
//		}
//		if(handTouching[RIGHT_HAND]!=NO_SHAPE) {
//			addShape(handTouching[RIGHT_HAND], ofVec2f(ofRandom(0, WIDTH), 1));
//		}
//	}
//    // remove shapes offscreen
//    //ofRemove(shapes, ofxBox2dBaseShape::shouldRemoveOffScreen);
//	float currTime = ofGetElapsedTimef();
//	for(int i =0 ; i < shapes.size(); i++) {
//		if(ofxBox2dBaseShape::shouldRemoveOffScreen(shapes[i]) || shapeIsTooOld(currTime, shapes[i].get())) {
//			data.erase(shapes[i].get());
//			shapes.erase(shapes.begin() + i);
//			i--;
//		}
//	}

}

//void ReachState::setColorForShape(ShapeID t) {
//	if(t==CIRCLE) {
//		ofSetHexColor(0xeb9f24);
//	} else if(t==HEXAGON) {
//		ofSetHexColor(0x35c3f2);
//	} else if(t==TRIANGLE) {
//		ofSetHexColor(0xc6d92c);
//	} else if(t==SQUARE) {
//		ofSetHexColor(0xf085b6);
//	}
//}

//void _ofVertex(ofVec2f v) {
//	ofVertex(v.x, v.y);
//}
//
//void ReachState::drawFluffBall(ofVec2f p, float radius) {
//	glPushMatrix();
//	glTranslatef(p.x, p.y, 0);
//	ofVec2f a(radius, 0);
//	int steps = 60;
//	float angle = 360.f/steps;
//	
//	
//	glBegin(GL_TRIANGLE_FAN);
//	glColor4f(1, 1, 1, 1);
//	glVertex2f(0,0);
//	for(int i = 0; i <= steps; i++) {
//	glColor4f(1, 1, 1, 0);
//		glVertex2f(a.x, a.y);
//		a.rotate(angle);
//	}
//	glEnd();
//	glPopMatrix();
//}
void ReachState::draw()
{
	ofDrawBitmapString("reach", 30, 30);
 //   getSharedData().drawCorrectDisplayMode();

	//ofFill();

 //   for(int i=0; i<shapes.size(); i++) {
	//	ShapeID t = data[shapes[i].get()].type;
	//	setColorForShape(t);
	//	shapes[i].get()->draw();
	//}
	//ofSetColor(255);
	//for(int i = 0; i < triggers.size(); i++) {
	//	ofFill();
	//	ofSetColor(255);
	//	ShapeID shapeType = triggers[i].first;
	//	
	//	if(handTouching[LEFT_HAND]==shapeType || handTouching[RIGHT_HAND]==shapeType) {
	//		ofRectangle r = triggers[i].second;
	//		ofPoint c = r.getCenter();
	//		float scale = ofMap(sin(ofGetElapsedTimef()*3 + i), -1, 1, 1.1, 1.4);
	//		r.setFromCenter(c, r.width * scale, r.height * scale);
	//		drawShape(triggers[i].first, r);
	//		//drawFluffBall(triggers[i].second.getCenter(), triggers[i].second.getWidth()*0.9);
	//	}
	//	
	//	ofFill();
	//	//setColorForShape(triggers[i].first);
	//	drawShape(triggers[i].first, triggers[i].second);
	//}
}
//void ReachState::drawShape(int shapeId, ofRectangle &rect) {
//	
//	shapeImages[(ShapeID)shapeId].draw(rect.getCenter().x, rect.getCenter().y, rect.width, rect.height);
//	
//	
//	/*switch(shapeId) {
//			case CIRCLE:
//				ofCircle(rect.getCenter(), rect.width/2);
//				
//				break;
//			case SQUARE:{
//				ofRectangle r = rect;
//				ofPoint p = r.getCenter();
//				r.setFromCenter(p, r.width, r.height);
//				ofRect(r);
//				break;}
//			case TRIANGLE:
//			{
//				ofBeginShape();
//				_ofVertex(rect.getBottomLeft());
//				_ofVertex(rect.getBottomRight());
//				
//				ofVec2f v = (rect.getBottomRight() + rect.getBottomLeft())/2;
//				v.y -= rect.height*sqrt(3)/2;
//				_ofVertex(v);
//				
//				ofEndShape();
//			}
//				break;
//			case HEXAGON:
//			{
//				ofPushMatrix();
//				ofTranslate(rect.getCenter().x, rect.getCenter().y, 0);
//				ofBeginShape();
//				ofVec2f v(rect.width/2, 0);
//				for(int i = 0; i < 6; i++) {
//					_ofVertex(v);
//					v.rotate(60);
//				}
//				ofEndShape();
//				
//				ofPopMatrix();
//			}
//				break;
//		}*/
//}
string ReachState::getName()
{
	return "reach";
}

//--------------------------------------------------------------
//void ReachState::keyPressed(int k) {
//
//	ofVec2f m(ofGetMouseX(), ofGetMouseY());
//	ofVec2f x(getSharedData().openNIDevice.getWidth(), getSharedData().openNIDevice.getHeight());
//	ofVec2f s(WIDTH, HEIGHT);
//	
//	m = m*x/s;
//	
//	if(k=='j') {
//		addShape(CIRCLE, m);
//	} else if(k=='k') {
//		addShape(HEXAGON, m);
//	} else if(k=='l') {
//		addShape(TRIANGLE, m);
//	} else if(k==';') {
//		addShape(SQUARE, m);
//	}
//	
//	
//}
//
//void ReachState::mouseMoved(int x, int y) {
//	ofVec2f m(x,y);
//	ofVec2f k(getSharedData().openNIDevice.getWidth(), getSharedData().openNIDevice.getHeight());
//	ofVec2f s(ofGetWidth(), ofGetHeight());
//	
//	m = m*k/s;
//	
//	handMoved(m, LEFT_HAND);
//
//}
//
//void ReachState::handMoved(ofVec2f p, Hand hand) {
//	bool found = false;
//	printf("%f %f\n", p.x, p.y);
//	for(int i = 0; i < triggers.size(); i++) {
//		if(triggers[i].second.inside(p.x, p.y)) {
//			handTouching[hand] = triggers[i].first;
//			found = true;
//		}
//	}
//	if(!found) handTouching[hand] = NO_SHAPE;
//}
//
//
//
//void ReachState::mouseReleased(int x, int y, int button)
//{
//    shapes.clear();
//	data.clear();
//	changeState("choice");
//}
//
//void ReachState::addShape(ShapeID type, ofVec2f pos) {
//	ofxBox2dBaseShape *shape = NULL;
//	float density = 10;
//	float minSize = 15;
//	float maxSize = 30;
//
//	if(type==CIRCLE) {
//		
//		ofxBox2dCircle *c = new ofxBox2dCircle();
//		
//		float r = ofRandom(minSize, maxSize);
//		c->setPhysics(density, 0.53, 0.1);
//		c->setup(getSharedData().box2d->getWorld(), pos.x, pos.y, r);
//		shape = c;
//		
//	} else if(type==SQUARE) {
//		float w = ofRandom(minSize, maxSize);
//		float h = w;
//		ofxBox2dRect *r = new ofxBox2dRect();
//		r->setPhysics(density, 0.53, 0.1);
//		r->setup(getSharedData().box2d->getWorld(), pos.x, pos.y, w*0.8, h*0.8);
//		shape = r;
//	} else if(type==TRIANGLE) {
//		float w = ofRandom(minSize, maxSize);
//		float h = w*1.73;
//		ofxBox2dPolygon *p = new ofxBox2dPolygon();
//		p->setPhysics(density, 0.53, 0.1);
//
//		p->addTriangle(ofVec2f(-w, h/2),ofVec2f(w, h/2), ofVec2f(0, -h/2));
//		p->create(getSharedData().box2d->getWorld());
//		p->setPosition(pos.x, pos.y);
//		shape = p;
//	} else if(type==HEXAGON) {
//		ofVec2f a, b;
//		float h = ofRandom(minSize, maxSize);
//		a = b = ofVec2f(h, 0);
//		a.rotate(-30);
//		b.rotate(30);
//		
//		ofxBox2dPolygon *p = new ofxBox2dPolygon();
//		p->setPhysics(density, 0.53, 0.1);
//		for(int i = 0; i < 6; i++) {
//			p->addTriangle(ofVec2f(0,0),a, b);
//			a.rotate(60);
//			b.rotate(60);
//		}
//		
//		p->create(getSharedData().box2d->getWorld());
//		p->setPosition(pos.x, pos.y);
//		shape = p;
//	}
//
//	
//	if(shape!=NULL) {
//		shapes.push_back(ofPtr<ofxBox2dBaseShape>(shape));
//		data[shape] = ShapeData(type, ofGetElapsedTimef());
//	}
//
//}

