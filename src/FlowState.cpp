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

JointType Limbs[] [2] = 
{
	{ JointType::FootRight, JointType::AnkleRight },
	{ JointType::AnkleRight, JointType::KneeRight },
	{ JointType::KneeRight, JointType::HipRight },
	{ JointType::HipRight, JointType::SpineMid },
	{ JointType::FootLeft, JointType::AnkleLeft },
	{ JointType::AnkleLeft, JointType::KneeLeft },
	{ JointType::KneeLeft, JointType::HipLeft },
	{ JointType::HipLeft, JointType::SpineBase },
	{ JointType::SpineBase, JointType::SpineMid },
	{ JointType::SpineMid, JointType::SpineShoulder },
	
	{ JointType::ThumbRight, JointType::HandRight },
	{ JointType::HandTipRight, JointType::HandRight },
	{ JointType::HandRight, JointType::WristRight },
	{ JointType::WristRight, JointType::ElbowRight },
	{ JointType::ElbowRight, JointType::ShoulderRight },
	{ JointType::ShoulderRight, JointType::SpineShoulder },

	{ JointType::ThumbLeft, JointType::HandLeft },
	{ JointType::HandTipLeft, JointType::HandLeft },
	{ JointType::HandLeft, JointType::WristLeft },
	{ JointType::WristLeft, JointType::ElbowLeft },
	{ JointType::ElbowLeft, JointType::ShoulderLeft },
	{ JointType::ShoulderLeft, JointType::SpineShoulder },

	{ JointType::SpineShoulder, JointType::Neck },
	{ JointType::Neck, JointType::Head }
};

void FlowState::setup()
{
	if (!shader.load("shader"))
		return;

	_kinect = KinectSensor::GetDefault();
	if (_kinect != nullptr)
	{
		_multiFrameReader = _kinect->OpenMultiSourceFrameReader(FrameSourceTypes::Color | FrameSourceTypes::Body);

		_camSpacePoints = ref new Array<CameraSpacePoint>(2);
		_colSpacePoints = ref new Array<ColorSpacePoint>(2);

		auto colorFrameDescription = _kinect->ColorFrameSource->CreateFrameDescription(ColorImageFormat::Rgba);

		// rgba is 4 bytes per pixel
		_bytesPerPixel = colorFrameDescription->BytesPerPixel;

		// allocate space to put the pixels to be rendered
		_colorPixels = ref new Array<byte>(colorFrameDescription->Width * colorFrameDescription->Height * _bytesPerPixel);

		_bodies = ref new Vector<Body^>(_kinect->BodyFrameSource->BodyCount);

		_kinect->Open();
	}
}

void FlowState::update()
{
	_bodiesProcessed = false;
	_colorFrameProcessed = false;

	auto multiFrame = _multiFrameReader->AcquireLatestFrame();
	if (multiFrame == nullptr)
		return;
	{
		if (multiFrame->ColorFrameReference != nullptr)
		{
			auto colorFrame = multiFrame->ColorFrameReference->AcquireFrame();

			if (colorFrame != nullptr)
			{
				colorFrame->CopyConvertedFrameDataToArray(_colorPixels, ColorImageFormat::Rgba);
				_img.setFromPixels(_colorPixels->Data, colorFrame->FrameDescription->Width, colorFrame->FrameDescription->Height, ofImageType::OF_IMAGE_COLOR_ALPHA);
				_colorFrameProcessed = true;
			}
		}
		if (multiFrame->BodyFrameReference != nullptr)
		{
			auto bodyFrame = multiFrame->BodyFrameReference->AcquireFrame();
			if (bodyFrame == nullptr)
				return;
			bodyFrame->GetAndRefreshBodyData(_bodies);
			_bodiesProcessed = true;
		}
	}

	if (!_bodiesProcessed)
		return;

	int id = 0;
	for (auto body : _bodies)
	{
		if (!body->IsTracked)
			continue;
		auto camsp = ref new Array<CameraSpacePoint>(body->JointCount);

		for (int boneIdx = 0; boneIdx < 24; boneIdx++)
		{
			auto j1 = Limbs[boneIdx][0];
			auto j2 = Limbs[boneIdx][1];

			auto joint1 = body->Joints->Lookup(j1);
			auto joint2 = body->Joints->Lookup(j2);

			auto cm = _kinect->CoordinateMapper;
			_camSpacePoints[0] = joint1.Position;
			_camSpacePoints[1] = joint2.Position;

			cm->MapCameraPointsToColorSpace(_camSpacePoints, _colSpacePoints);

			auto x = _colSpacePoints[1].X - _colSpacePoints[0].X;
			auto y = _colSpacePoints[1].Y - _colSpacePoints[0].Y;

			ofVec3f limbLength = ofVec3f(x, y);
			ofVec3f joint1Pos = ofVec3f(_colSpacePoints[0].X, _colSpacePoints[0].Y);

			for (int k = 0; k < NUM_TRAILS_PER_LIMB; k++)
			{
				int myId = id * 10000 + boneIdx * 100 + k;
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
}

void FlowState::draw()
{
	//getSharedData().drawCorrectDisplayMode();
	ofClear(255);

	if (!_kinect->IsAvailable)
	{
		ofDrawBitmapString("No Kinect", 50, 10);
		return;
	}

	ofSetColor(255);
	_img.draw(0, 0);

	ofSetColor(0);
	
	ofPushMatrix();
	////ofScale
	//ofSetLineWidth(75.0f);

	shader.begin();
	shader.setUniform1f("thickness", 20.0f);
	map<int,Trail>::iterator it = trails.begin();
	while(it!=trails.end()) {
		(*it).second.draw();
		it++;
	}
	shader.end();

	ofPopMatrix();
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

void FlowState::stateEnter() {
	SomabilityApp::stateEnter();
	ofSetWindowTitle(getName());
	
	ofAddListener(getSharedData().openNIDevice.userEvent, this, &FlowState::userEvent);
}
void FlowState::stateExit() {
	ofRemoveListener(getSharedData().openNIDevice.userEvent, this, &FlowState::userEvent);
}
#endif