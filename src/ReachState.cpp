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
#define MIN_TIME_BETWEEN_SPAWNS 200

void ReachState::setup() {
	setupEvents();
	handTouching[LEFT_HAND] = SharedData::NO_SHAPE;
	handTouching[RIGHT_HAND] = SharedData::NO_SHAPE;
	shapeSize = 1;
	
	_camSpacePoints = ref new Array<CameraSpacePoint>(2);
	_colSpacePoints = ref new Array<ColorSpacePoint>(2);
	
	map<SharedData::ShapeID, ofImage>::iterator it;
	
	for(it = shapeImages.begin(); it != shapeImages.end(); it++) {
		(*it).second.setAnchorPercent(0.5, 0.5);
	}
}

void ReachState::initTriggers() {
	// y position of the triggers, 2/9ths down the screen
	float yy = getSharedData().colFrameHeight*1.0f / 6.0f;
	//triggers.resize(SharedData::NUM_SHAPES);
	for (int i = 0; i < SharedData::NUM_SHAPES; i++) {
		float xpos = ofMap(i, -0.25, SharedData::NUM_SHAPES - 0.25, 0, getSharedData().colFrameWidth);
		//		ofLine(xpos, 0, xpos, 480);
		ofRectangle r;
		float w = shapeSize*getSharedData().colFrameWidth / 7.0f;
		r.set(xpos, yy, w, w);
		triggers.push_back(make_pair(make_pair((SharedData::ShapeID)i, r), 1.0f));
	}
}

void ReachState::stateEnter() {
	//data.clear();
	//shapes.clear();
	//vector<b2Body*> bodies;
	//b2Body* body = getSharedData().box2d->getWorld()->GetBodyList();
	//while (body != NULL) {
	//	bodies.push_back(body);
	//	body = body->GetNext();
	//}
	//for (auto bodyToDelete : bodies) {
	//	getSharedData().box2d->getWorld()->DestroyBody(bodyToDelete);
	//}

	//getSharedData().box2d->createBounds(0, 0, ofGetWidth(), ofGetHeight());
}

void ReachState::stateExit() {
	data.clear();
	shapes.clear();
	vector<b2Body*> bodies;
	b2Body* body = getSharedData().box2d->getWorld()->GetBodyList();
	while (body != NULL) {
		bodies.push_back(body);
		body = body->GetNext();
	}
	for (auto bodyToDelete : bodies) {
		getSharedData().box2d->getWorld()->DestroyBody(bodyToDelete);
	}

	getSharedData().box2d->createBounds(0, 0, getSharedData().colFrameWidth, getSharedData().colFrameHeight);
}

//void ReachState::setupGui(SomabilityGui *gui) {
//	gui->addSlider("Shape size", shapeSize, 0.5, 1.5);
//}

bool ReachState::shapeIsTooOld(float currTime, ofxBox2dBaseShape *shape) {
	if(data.find(shape)!=data.end()) {
		return (currTime - data[shape].birthday)>MAX_SHAPE_AGE;
	}
	return false;
}

void ReachState::update()
{
	if (triggers.size() == 0) {
		initTriggers();
	}
	auto multiFrame = getSharedData().getMultiSourceFrame();
	auto bodies = getSharedData().getBodies(multiFrame);
	getSharedData().setCorrectDisplayImage(multiFrame);
	int i = 0;
	if (bodies != nullptr) {
		for (auto body : bodies) {
			if (!body->IsTracked)
				continue;
			i++;
			auto handLeft = body->Joints->Lookup(JointType::HandLeft);
			auto handRight = body->Joints->Lookup(JointType::HandRight);

			auto cm = getSharedData().getCoordinateMapper();
			_camSpacePoints[0] = handLeft.Position;
			_camSpacePoints[1] = handRight.Position;

			cm->MapCameraPointsToColorSpace(_camSpacePoints, _colSpacePoints);
			for (auto point : _colSpacePoints) {
				for (int i = 0; i < triggers.size(); i++) {
					if (triggers[i].first.second.inside(point.X, point.Y) && ofGetElapsedTimeMillis() - lastSpawnTime > MIN_TIME_BETWEEN_SPAWNS) {
						addShape(triggers[i].first.first, ofVec2f(ofRandom(0, getSharedData().colFrameWidth), 1));
						lastSpawnTime = ofGetElapsedTimeMillis();
					}
				}
			}
		}
	}

	getSharedData().box2d->update();

    // remove shapes offscreen
//    ofRemove(shapes, ofxBox2dBaseShape::shouldRemoveOffScreen);
	float currTime = ofGetElapsedTimef();
	for(int i =0 ; i < shapes.size(); i++) {
		if(/*ofxBox2dBaseShape::shouldRemoveOffScreen(shapes[i]) ||*/ shapeIsTooOld(currTime, shapes[i].get())) {
			data.erase(shapes[i].get());
			shapes.erase(shapes.begin() + i);
			i--;
		}
	}
}

void ReachState::setColorForShape(SharedData::ShapeID t) {
	if (t == SharedData::CIRCLE) {
		ofSetHexColor(0xeb9f24);
	}
	else if (t == SharedData::HEXAGON) {
		ofSetHexColor(0x35c3f2);
	}
	else if (t == SharedData::TRIANGLE) {
		ofSetHexColor(0xc6d92c);
	}
	else if (t == SharedData::SQUARE) {
		ofSetHexColor(0xf085b6);
	}
}

void _ofVertex(ofVec2f v) {
	ofVertex(v.x, v.y);
}

//void ReachState::drawFluffBall(ofVec2f p, float radius) {
//	ofPushMatrix();
//	ofTranslate(p.x, p.y, 0);
//	ofVec2f a(radius, 0);
//	int steps = 60;
//	float angle = 360.f/steps;
//
//	//ofSetStyle(OF_PRIMITIVE_TRIANGLE_FAN);
//	ofBeginShape();
//	ofSetColor(1, 1, 1, 1);
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
	ofBackground(255);
	ofDrawBitmapString("reach", 30, 30);
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(getSharedData().imgTransform.first);
	ofScale(getSharedData().imgTransform.second, getSharedData().imgTransform.second, getSharedData().imgTransform.second);
	ofSetColor(255);
	getSharedData().drawCorrectDisplayImage();
	ofPopStyle();

	ofFill();

    for(int i=0; i<shapes.size(); i++) {
		SharedData::ShapeID t = data[shapes[i].get()].type;
		ofVec2f pos = shapes[i].get()->getPosition();
		ofPushMatrix();
		ofTranslate(pos.x, pos.y, 0);
		ofRotate(shapes[i].get()->getRotation());
		ofPushStyle();
		ofSetRectMode(OF_RECTMODE_CENTER);
		getSharedData().drawShape(t, ofRectangle(0, 0, 50, 50));
		ofPopStyle();
		ofPopMatrix();
	}

	for(int i = 0; i < triggers.size(); i++) {
		ofFill();
		ofSetColor(255);
		SharedData::ShapeID shapeType = triggers[i].first.first;
		if(handTouching[LEFT_HAND]==shapeType || handTouching[RIGHT_HAND]==shapeType) {
			ofRectangle r = triggers[i].first.second;
			ofPoint c = r.getCenter();
			float scale = ofMap(sin(ofGetElapsedTimef()*3 + i), -1, 1, 1.1, 1.4);
			r.setFromCenter(c, r.width * scale, r.height * scale);
			getSharedData().drawShape(triggers[i].first.first, r);
			//drawFluffBall(triggers[i].second.getCenter(), triggers[i].second.getWidth()*0.9);
		}
		
		ofFill();
		//setColorForShape(triggers[i].first);
		//ofRotateZ(triggers[i].second);
		getSharedData().drawShape(triggers[i].first.first, triggers[i].first.second);
	}
	ofPushStyle();
		ofSetColor(0);
		getSharedData().drawDisplayMode();
	ofPopStyle();
	ofPopMatrix();

}

string ReachState::getName()
{
	return "reach";
}

//--------------------------------------------------------------
void ReachState::keyPressed(int k) {

	//ofVec2f m(ofGetMouseX(), ofGetMouseY());
	//ofVec2f x(getSharedData().openNIDevice.getWidth(), getSharedData().openNIDevice.getHeight());
	//ofVec2f s(WIDTH, HEIGHT);
	
	//m = m*x/s;
	//
	//if(k=='j') {
	//	addShape(SharedData::CIRCLE, m);
	//} else if(k=='k') {
	//	addShape(SharedData::HEXAGON, m);
	//} else if(k=='l') {
	//	addShape(SharedData::TRIANGLE, m);
	//} else if(k==';') {
	//	addShape(SharedData::SQUARE, m);
	//}

	getSharedData().changeDisplayMode(k);
	//if (k == '1') {
	//	changeState("reach");
	//}
	//if (k == '2') {
	//	changeState("flow");
	//}
	//if (k == '3') {
	//	changeState("balance");
	//}

}
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
void ReachState::addShape(SharedData::ShapeID type, ofVec2f pos) {
	ofxBox2dBaseShape *shape = NULL;
	float density = 10;
	float minSize = 25;
	float maxSize = 25;

	if (type == SharedData::CIRCLE) {
		ofxBox2dCircle *c = new ofxBox2dCircle();
		float r = ofRandom(minSize, maxSize);
		c->setPhysics(density, 0.53, 0.1);
		c->setup(getSharedData().box2d->getWorld(), pos.x, pos.y, r);
		shape = c;
	}
	else if (type == SharedData::SQUARE) {
		float w = ofRandom(minSize, maxSize);
		float h = w;
		ofxBox2dRect *r = new ofxBox2dRect();
		r->setPhysics(density, 0.53, 0.1);
		r->setup(getSharedData().box2d->getWorld(), pos.x, pos.y, w, h);
		ofVec2f pos = r->getPosition();
		float width = r->getWidth();
		float height = r->getHeight();
		shape = r;
	}
	else if (type == SharedData::TRIANGLE) {
		float w = ofRandom(minSize, maxSize);
		float h = w*1.73;
		ofxBox2dPolygon *p = new ofxBox2dPolygon();
		p->setPhysics(density, 0.53, 0.1);

		p->addTriangle(ofVec2f(-w, h/2),ofVec2f(w, h/2), ofVec2f(0, -h/2));
		p->create(getSharedData().box2d->getWorld());
		p->setPosition(pos.x, pos.y);
		shape = p;
	}
	else if (type == SharedData::HEXAGON) {
		ofVec2f a, b;
		float h = ofRandom(minSize, maxSize);
		a = b = ofVec2f(h, 0);
		a.rotate(-30);
		b.rotate(30);
		
		ofxBox2dPolygon *p = new ofxBox2dPolygon();
		p->setPhysics(density, 0.53, 0.1);
		for(int i = 0; i < 6; i++) {
			p->addTriangle(ofVec2f(0,0),a, b);
			a.rotate(60);
			b.rotate(60);
		}
		
		p->create(getSharedData().box2d->getWorld());
		p->setPosition(pos.x, pos.y);
		shape = p;
	}
	if(shape!=NULL) {
		shapes.push_back(ofPtr<ofxBox2dBaseShape>(shape));
		data[shape] = ShapeData(type, ofGetElapsedTimef());
	}
}

