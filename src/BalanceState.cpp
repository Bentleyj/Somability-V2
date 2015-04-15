/*
 *  BalanceState.cpp
 *
 *  based on BalanceState.cpp Copyright (c) 2011, Neil Mendoza, http://www.neilmendoza.com
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
#include "BalanceState.h"

#define WIDTH ofGetWidth()
#define HEIGHT ofGetHeight()


void BalanceState::addWall() {
	wall[0].x += 100;
	wall[0].y += 100;
	//wall.addVertex(ofRandom(ofGetWidth()/2), ofRandom(ofGetHeight()));
	//wall.addVertex(ofRandom(ofGetWidth()/2), ofRandom(ofGetHeight()));
	//wall.simplify(5);
	wall.setPhysics(0, 0, 0);
	wall.create(getSharedData().box2d->getWorld());
	wall.flagHasChanged();
	wall.updateShape();
}

void BalanceState::setup() {
	setupEvents();
	gun.loadImage("gun.png");
	circle.loadImage("circle.png");
	circle.setAnchorPercent(0.5, 0.5);
	gun.setAnchorPercent(0.5, 0.5);
	sensitivity = 50;

	//persons.resize(144);

	_camSpacePoints = ref new Array<CameraSpacePoint>(2);
	_colSpacePoints = ref new Array<ColorSpacePoint>(2);

	wall.addVertex(0, 0);
	wall.addVertex(200, 200);
	wall.simplify(5);
	wall.setPhysics(0, 0, 0);
	wall.create(getSharedData().box2d->getWorld());
	wall.flagHasChanged();
	wall.updateShape();

	//
	//if(ofFile("micSensitivity.txt").exists()) {
	//	sensitivity = ofToInt(ofBufferFromFile("micSensitivity.txt").getText());
	//	if(sensitivity==0) sensitivity = 42;
	//}
	//soundStream.setup(0, 2, 44100, 512, 1);
	//soundStream.setInput(this);
	//soundStream.stop();
	//audioFramesSinceLastFired = 0;
	//MIN_FRAMES_BETWEEN_FIRES = 0.3 * 44100; // 0.3 seconds at 44.1kHz
	//boing.loadSound("boing.wav");
	//mustFire = false;
	//MAX_SHAPE_AGE = 20;



	//greyImg.allocate(VISION_WIDTH, VISION_HEIGHT);
	//buff = new unsigned char[VISION_WIDTH*VISION_HEIGHT];
}

void BalanceState::shoot() {
	ofxBox2dCircle *c = new ofxBox2dCircle();
	
	float r = ofRandom(23, 28);
	c->setPhysics(3.0, 0.53, 0.1);
	c->setup(getSharedData().box2d->getWorld(), WIDTH, HEIGHT/2, r);
	ofVec2f v(-20,0);
	v.rotateRad(shootingAngle);
	c->setVelocity(v);
	shapes.push_back(ofPtr<ofxBox2dBaseShape>(c));
	data[c] = ShapeData(ofGetElapsedTimef());

}


bool BalanceState::shapeIsTooOld(float currTime, ofxBox2dBaseShape *shape) {
	if(data.find(shape)!=data.end()) {
		return (currTime - data[shape].birthday)>MAX_SHAPE_AGE;
	}
	return false;
}


void BalanceState::update()
{
	trackedIds.clear();
	IdsToDelete.clear();

	shootingAngle = ofMap(sin(ofGetElapsedTimef()/3), -1, 1, -PI/8, PI/4);

	if(mustFire) {
	//	boing.play();
		shoot();
		mustFire = false;
	}

	auto multiFrame = getSharedData().getMultiSourceFrame();
	auto bodies = getSharedData().getBodies(multiFrame);
	getSharedData().setColorImage(_img, multiFrame);

	if (bodies != nullptr) {
		for (auto body : bodies) {
			if (!body->IsTracked) 
				continue;
			auto it = persons.find(body->TrackingId);
			vector<ofxBox2dEdge*> * edges;
			if (it != persons.end()) {
				edges = (*it).second;
				for (int boneIdx = 0; boneIdx < 24; boneIdx++)
				{
					auto j1 = SharedData::Limbs[boneIdx][0];
					auto j2 = SharedData::Limbs[boneIdx][1];

					auto joint1 = body->Joints->Lookup(j1);
					auto joint2 = body->Joints->Lookup(j2);

					auto cm = getSharedData().getCoordinateMapper();
					_camSpacePoints[0] = joint1.Position;
					_camSpacePoints[1] = joint2.Position;

					cm->MapCameraPointsToColorSpace(_camSpacePoints, _colSpacePoints);

					auto edge = (*edges)[boneIdx];
					(*edge)[0].x = _colSpacePoints[0].X;
					(*edge)[0].y = _colSpacePoints[0].Y;
					(*edge)[1].x = _colSpacePoints[1].X;
					(*edge)[1].y = _colSpacePoints[1].Y;
				}
				persons[body->TrackingId] = edges;
			}
			else {
				edges = new vector<ofxBox2dEdge*>();

				for (int boneIdx = 0; boneIdx < 24; boneIdx++)
				{
					auto j1 = SharedData::Limbs[boneIdx][0];
					auto j2 = SharedData::Limbs[boneIdx][1];

					auto joint1 = body->Joints->Lookup(j1);
					auto joint2 = body->Joints->Lookup(j2);

					auto cm = getSharedData().getCoordinateMapper();
					_camSpacePoints[0] = joint1.Position;
					_camSpacePoints[1] = joint2.Position;

					cm->MapCameraPointsToColorSpace(_camSpacePoints, _colSpacePoints);

					ofxBox2dEdge* edge = new ofxBox2dEdge();
					edge->addVertex(ofPoint(_colSpacePoints[0].X, _colSpacePoints[0].Y));
					edge->addVertex(ofPoint(_colSpacePoints[1].X, _colSpacePoints[1].Y));
					edges->push_back(edge);
				}
				persons[body->TrackingId] = edges;
			}

			trackedIds.push_back(body->TrackingId);
		}
		for (auto person : persons) {
			if (std::find(trackedIds.begin(), trackedIds.end(), person.first) != trackedIds.end()) {
				for (auto bone : *person.second) {
					bone->simplify(5);
					bone->setPhysics(0, 0, 0);
					bone->create(getSharedData().box2d->getWorld());
					bone->flagHasChanged();
					bone->updateShape();
				}
			}
			else {
				IdsToDelete.push_back(person.first);
			}
		}

		for (auto IdToDelete : IdsToDelete) {
			for (auto edge : *persons.find(IdToDelete)->second) {
				getSharedData().box2d->getWorld()->DestroyBody(edge->body);
			}
			persons.erase(IdToDelete);
		}
	}
	
	getSharedData().box2d->update();

	//greyImg.set(0);
	//unsigned char *pix = greyImg.getPixels();
	//memset(buff, 0, VISION_WIDTH*VISION_HEIGHT);
	//for(int i =0 ; i < numUsers; i++) {
	//	
	//	ofxOpenNIUser &user = getSharedData().openNIDevice.getTrackedUser(i);
	//	unsigned char *c = user.getMaskPixels().getPixels();
	//	if(c!=NULL) {
	//		int nc = user.getMaskPixels().getNumChannels();
	//			//printf("%d channels\n", nc);
	//		
	//		
	//		for(int i = 0; i < 640*480; i++) {
	//			buff[i] = (c[i*nc])>0?255:buff[i];
	//		}
	//		
	//	} else {
	//		//ofLogError() << "Pixel mask of person is null!";
	//	}
	//	
	//}
	//greyImg.setFromPixels(buff, VISION_WIDTH, VISION_HEIGHT);

	//
	//if(greyImg.getWidth()>0) {
	//	contours.findContours(greyImg, 50, VISION_HEIGHT*VISION_HEIGHT, 20, false);
	//}

	//// resize persons array
	//while(persons.size() < contours.blobs.size()) {
	//	persons.push_back(new ofxBox2dEdge());
	//}

	//while(contours.blobs.size() < persons.size()) {
	//	delete persons.back();
	//	persons.pop_back();
	//}
	//

	//ofVec3f contourScale(WIDTH/(float)greyImg.getWidth(),HEIGHT/(float)greyImg.getHeight());
	//int numBlobs = contours.nBlobs;
	//for(int blob = 0; blob < numBlobs; blob++) {
	//	persons[blob]->clear();
	//
	//	for(int i = 0; i < contours.blobs[blob].nPts; i++) {
	//		ofVec3f p = contours.blobs[blob].pts[i]*contourScale;
	//
	//		persons[blob]->addVertex(p);
	//	}
	//
	//	persons[blob]->simplify(5);
	////if(person->size()>2) {
	//	persons[blob]->setPhysics(0,0,0);
	//	persons[blob]->create(getSharedData().box2d->getWorld());
	//	persons[blob]->flagHasChanged();
	//	persons[blob]->updateShape();
	//}
	//
	//getSharedData().box2d->update();

	// remove shapes offscreen
    ofRemove(shapes, ofxBox2dBaseShape::shouldRemoveOffScreen);
	float currTime = ofGetElapsedTimef();
	for(int i =0 ; i < shapes.size(); i++) {
		if(ofxBox2dBaseShape::shouldRemoveOffScreen(shapes[i]) || shapeIsTooOld(currTime, shapes[i].get())) {
			data.erase(shapes[i].get());
			shapes.erase(shapes.begin() + i);
			i--;
		}
	}
}
//void BalanceState::setupGui(SomabilityGui *gui) {
//	gui->addSlider("sensitivity", sensitivity, 0, 1);
//	gui->addSlider("volume", displayVolume, 0, 1);
//}

void BalanceState::draw()
{
	ofDrawBitmapString("balance", 30, 30);

	ofPushMatrix();
	ofNoFill();
	ofSetColor(255);
	ofTranslate(getSharedData().imgTransform.first);
	ofScale(getSharedData().imgTransform.second, getSharedData().imgTransform.second, getSharedData().imgTransform.second);
	ofSetColor(255);
	_img.draw(0, 0);
	//ofSetColor(0);
	//ofDrawBitmapString(ofToString(persons.size()), 30, 30);
	ofDrawBitmapString(ofToString(getSharedData().box2d->getWorld()->GetBodyCount()), 30, 30);

	//getSharedData().drawCorrectDisplayMode();
	//glPushMatrix();
	//
	//
	//ofNoFill();
	//ofSetColor(255);
	////greyImg.draw(0,0);
	////person->draw();
	//ofFill();
	//
	//ofSetColor(255);
	////getSharedData().openNIDevice.drawDepth(0, 0, ofGetWidth(), ofGetHeight());

	//
	//ofFill();
	//ofSetColor(ofColor::white);
	//

    for(int i=0; i<shapes.size(); i++) {
		ofxBox2dCircle *c = (ofxBox2dCircle*) shapes[i].get();
		circle.draw(c->getPosition().x, c->getPosition().y, c->getRadius()*2, c->getRadius()*2);
		shapes[i].get()->draw();
	}

	for (auto person : persons) {
		ofSetColor(255);
		ofSetLineWidth(20);
		for (auto edge : *person.second) {
			edge->draw();
		}
	}
	wall.draw();
	ofPopMatrix();


	// draw the cannon
	ofPushMatrix();
	ofTranslate(WIDTH,HEIGHT/2, 0);
	ofRotate(ofRadToDeg(shootingAngle),0,0,1);
	ofSetColor(255);
	gun.draw(0,0);
	ofPopMatrix();
	//// contours.draw();

	//ofSetColor(255);
	////ofDrawBitmapString("Use the up and down arrow keys to change audio sensitivity ("+ofToString(sensitivity)+" / 100)", 5, 60);

}

string BalanceState::getName()
{
	return "balance";
}

void BalanceState::keyPressed(int k)
{
	//changeState("choice");
	if (k == 'f') tryToFire();
	if (k == 'w') addWall();
	//mustFire = true;
}
//
void BalanceState::tryToFire() {
	mustFire = true;
}
//
//
//void BalanceState::audioIn(float *samples, int length, int numChannels) {
//
//	for(int i =0 ; i < length; i++) {
//		float f = ABS(samples[i]);
//		if(volume<f) volume = f;
//		else volume *= 0.9995;
//		displayVolume = sqrt(volume);
//		if(displayVolume>sensitivity) {
//			if(audioFramesSinceLastFired>MIN_FRAMES_BETWEEN_FIRES) {
//				tryToFire();
//				
//				audioFramesSinceLastFired = 0;
//			}
//		}
//		
//		audioFramesSinceLastFired++;
//	}
//}
//
//bool togethernessSoundStarted = false;
//
//void BalanceState::stateEnter() {
//	SomabilityApp::stateEnter();
//	ofSetWindowTitle(getName());
//	if(!togethernessSoundStarted) {
//		togethernessSoundStarted = true;
//		soundStream.start();
//
//	}
//}
//void BalanceState::stateExit() {
//
//	map<ofxBox2dBaseShape*,ShapeData>::iterator it;
//	for(it = data.begin(); it != data.end(); it++) {
//		delete (*it).first;
//	}
//	data.clear();
//	//soundStream.stop();
//}
//
//void BalanceState::keyPressed(int k) {
//	bool saveSensitivity = false;
//
//	if(k==OF_KEY_DOWN) {
//		sensitivity--;
//		saveSensitivity = true;
//	}else if (k==OF_KEY_UP) {
//		sensitivity++;
//		saveSensitivity = true;
//	} else if(k=='o') {
//		tryToFire();
//	}
//	if(saveSensitivity) {
//		sensitivity = ofClamp(sensitivity, 0, 100);
//		ofBuffer b(ofToString(sensitivity));
//		ofBufferToFile("micSensitivity.txt", b);
//	}
//
//
//
//}