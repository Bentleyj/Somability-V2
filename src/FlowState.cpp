/*
 *  FlowState.cpp
 *
 *  based on GreenState.cpp Copyright (c) 2011, Neil Mendoza, http://www.neilmendoza.com
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
#include "FlowState.h"

#define NUM_TRAILS_PER_LIMB 3

string FlowState::getName() {
	return "flow";
}

void FlowState::setup()
{
	setupEvents();

	//if (!shader.load("shader"))
	//	return;

	//getSharedData().setupKinect(FrameSourceTypes::Color | FrameSourceTypes::Body);

	_camSpacePoints = ref new Array<CameraSpacePoint>(2);
	_colSpacePoints = ref new Array<ColorSpacePoint>(2);
}

void FlowState::update()
{
	auto multiFrame = getSharedData().getMultiSourceFrame();
	auto bodies = getSharedData().getBodies(multiFrame);
	getSharedData().setCorrectDisplayImage(multiFrame);


	if (bodies == nullptr)
		return;

	int id = 0;
	vector<int> trackedIds;
	for (auto body : bodies)
	{
		if (!body->IsTracked)
			continue;
		trackedIds.push_back(body->TrackingId);
		auto camsp = ref new Array<CameraSpacePoint>(body->JointCount);

		for (int boneIdx = 0; boneIdx < 24; boneIdx++)
		{
			auto j1 = SharedData::Limbs[boneIdx][0];
			auto j2 = SharedData::Limbs[boneIdx][1];

			auto joint1 = body->Joints->Lookup(j1);
			auto joint2 = body->Joints->Lookup(j2);

			auto cm = getSharedData().getCoordinateMapper();
			_camSpacePoints[0] = joint1.Position;
			_camSpacePoints[1] = joint2.Position;

			ofVec3f limbLength;
			ofVec3f joint1Pos;

			cm->MapCameraPointsToColorSpace(_camSpacePoints, _colSpacePoints);

			auto x = _colSpacePoints[1].X - _colSpacePoints[0].X;
			auto y = _colSpacePoints[1].Y - _colSpacePoints[0].Y;

			limbLength = ofVec3f(x, y);
			joint1Pos = ofVec3f(_colSpacePoints[0].X, _colSpacePoints[0].Y);

			for (int k = 0; k < NUM_TRAILS_PER_LIMB; k++)
			{
				int myId = body->TrackingId * 10000 + boneIdx * 100 + k;
				float p = (float) k / NUM_TRAILS_PER_LIMB;
				trails[myId].update(joint1Pos + limbLength * p);
			}
		}
		id++;
	}

	if (id > 0)
	{
		map<int, Trail>::iterator it = trails.begin();
		while (it != trails.end()) {
			(*it).second.update2();
			it++;
		}
	}

	vector<int> trailsToDelete;
	for (auto trail : trails) {
		bool found = false;
		int trackingId = (int)(trail.first / 10000);
		for (auto trackedId : trackedIds) {
			if (trackingId == trackedId) found = true;
		}
		if (!found && !trail.second.needsDelete) {
			trails[trail.first].fadeOut();
		}
	}
	for (auto trail : trails) {
		if (trail.second.needsDelete) {
			trailsToDelete.push_back(trail.first);
		}
	}

	for (auto trailToDelete : trailsToDelete) {
		trails.erase(trailToDelete);
	}
}

void FlowState::draw()
{
	ofBackground(255);
	//getSharedData().drawCorrectDisplayMode();
	ofClear(255);

	if (!getSharedData().isKinectOpen())
	{
		ofDrawBitmapString("No Kinect", 50, 10);
		return;
	}

	ofPushMatrix();
	ofTranslate(getSharedData().imgTransform.first);
	ofScale(getSharedData().imgTransform.second, getSharedData().imgTransform.second, getSharedData().imgTransform.second);
	ofSetColor(255);
	getSharedData().drawCorrectDisplayImage();
	ofPopMatrix();

	ofSetColor(0);
	
	ofPushMatrix();
	//ofScale
	//ofSetLineWidth(75.0f);

	map<int,Trail>::iterator it = trails.begin();
	while(it!=trails.end()) {
		(*it).second.draw();
		it++;
	}
	ofMesh mesh;
	for (int i = 0; i < points.size(); i++) {
		//ofColor col2 = col.a = ofMap(i, points.size(), 0, 1, 0, true);
		mesh.addColor(ofColor(255, 0, 0));
		mesh.addVertex(points[i]);

		//ofSetColor(ofColor(255, 0, 0));
		//ofCircle(points[i], 10);
	}
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	mesh.draw();

	ofPopMatrix();
	getSharedData().drawDisplayMode();
}

void FlowState::keyPressed(int k) {
	getSharedData().changeDisplayMode(k);
}

void FlowState::mouseDragged(int x, int y, int button) {
	ofVec2f p = ofVec2f(x, y);
	points.push_back(p);
	if (points.size() > 2) {
		ofVec2f first = p;
		ofVec2f second = *(points.begin() + 2);
		ofVec2f diff = first - second;
		diff.rotate(90);
		diff.normalize();
		diff *= 50.0f;
		points.push_back(p + diff);
	}
	while (points.size() > 20) {
		points.pop_front();
	}
}

#if 0
void FlowState::userEvent(ofxOpenNIUserEvent & event) {
	if(event.userStatus==USER_SKELETON_LOST || event.userStatus==USER_TRACKING_STOPPED) {
		// delete user trails

	}
}


string FlowState::getName()
{
	return "flow";
}

void FlowState::mouseReleased(int x, int y, int button)
{
	changeState("choice");
}


void FlowState::mouseMoved(int x, int y, int button) {
	
}

#endif