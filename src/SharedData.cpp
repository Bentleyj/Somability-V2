#include "SharedData.h"

const JointType SharedData::Limbs[24][2] = {
	{ JointType::FootRight, JointType::AnkleRight },
	{ JointType::AnkleRight, JointType::KneeRight },
	{ JointType::KneeRight, JointType::HipRight },
	{ JointType::HipRight, JointType::SpineBase },
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

//ofColor convertIDToColor(int ID) {
//
//}

void SharedData::setupKinect(FrameSourceTypes  frameSourceTypes) {
	_kinect = KinectSensor::GetDefault();
	if (_kinect != nullptr)
	{
		_multiFrameReader = _kinect->OpenMultiSourceFrameReader(frameSourceTypes);

		if ((frameSourceTypes & FrameSourceTypes::Color) == FrameSourceTypes::Color) {
			auto colorFrameDescription = _kinect->ColorFrameSource->CreateFrameDescription(ColorImageFormat::Rgba);

			// allocate space to put the pixels to be rendered
			_colorPixels = ref new Array<byte>(colorFrameDescription->Width * colorFrameDescription->Height * colorFrameDescription->BytesPerPixel);
		}

		if ((frameSourceTypes & FrameSourceTypes::Body) == FrameSourceTypes::Body) {
			_bodies = ref new Vector<Body^>(_kinect->BodyFrameSource->BodyCount);
		}

		if ((frameSourceTypes & FrameSourceTypes::BodyIndex) == FrameSourceTypes::BodyIndex) {
			auto bodyIndexFrameDescription = _kinect->BodyIndexFrameSource->FrameDescription;
			int width = bodyIndexFrameDescription->Width;
			int height = bodyIndexFrameDescription->Height;
			int bytesPerPixel = bodyIndexFrameDescription->BytesPerPixel;
			_bodyIndexPixels = ref new Array<byte>(bodyIndexFrameDescription->Width * bodyIndexFrameDescription->Height *  bodyIndexFrameDescription->BytesPerPixel);
		}

		AudioSource^ audioSource = _kinect->AudioSource;
		_audioBuffer = ref new Array<byte>(audioSource->SubFrameLengthInBytes);
		_audioReader = _kinect->AudioSource->OpenReader();

		_kinect->Open();
	}
}

bool SharedData::isKinectOpen() {
	return _kinect->IsOpen;
}

void SharedData::closeKinect() {
	_kinect->Close();
}

MultiSourceFrame^ SharedData::getMultiSourceFrame() {
	auto multiFrame = _multiFrameReader->AcquireLatestFrame();
	return multiFrame;
}

bool SharedData::setCorrectDisplayImage(MultiSourceFrame^ multiFrame) {
	_frameProcessed = false;
	if (multiFrame != nullptr)
	{
		if (displayMode == displayModeID::MIRROR) {
			if (multiFrame->ColorFrameReference != nullptr)
			{
				auto colorFrame = multiFrame->ColorFrameReference->AcquireFrame();

				if (colorFrame != nullptr)
				{
					colorFrame->CopyConvertedFrameDataToArray(_colorPixels, ColorImageFormat::Rgba);

					_frameProcessed = true;
				}
				if (_frameProcessed) {
					int height = _kinect->ColorFrameSource->FrameDescription->Height;
					int width = _kinect->ColorFrameSource->FrameDescription->Width;
					mirrorImage.setFromPixels(_colorPixels->Data, width, height, ofImageType::OF_IMAGE_COLOR_ALPHA);
				}
				else {
					mirrorImage.setColor(ofColor(0));
				}
			}
		}
		else if (displayMode == displayModeID::INVISIBLE) {
			_frameProcessed = true;
			mirrorImage.clear();

		}
		else if (displayMode == displayModeID::SILHOUETTE) {
			if (multiFrame->BodyIndexFrameReference != nullptr && multiFrame->BodyFrameReference != nullptr) {
				auto bodyIndexFrame = multiFrame->BodyIndexFrameReference->AcquireFrame();
				auto bodyFrame = multiFrame->BodyFrameReference->AcquireFrame();
				if (bodyIndexFrame != nullptr) {
					bodyIndexFrame->CopyFrameDataToArray(_bodyIndexPixels);
					_frameProcessed = true;
				}
				if (_frameProcessed) {
					int height = _kinect->BodyIndexFrameSource->FrameDescription->Height;
					int width = _kinect->BodyIndexFrameSource->FrameDescription->Width;
					int k = 0;
					for (int i = 0; i < height; i++) {
						for (int j = 0; j < width; j++) {
							if (_bodyIndexPixels[i*width + j] == 255) {
								_colorPixels[k] = 0;
								_colorPixels[k + 1] = 0;
								_colorPixels[k + 2] = 0;
							}
							else {
								_colorPixels[k] = _bodyIndexPixels[i*width + j]%4 * 65 + 127;
								_colorPixels[k + 1] = _bodyIndexPixels[i*width + j]%3 * 65 + 127;
								_colorPixels[k + 2] = _bodyIndexPixels[i*width + j]%2 * 65 + 127;
							}
							k += 3;
						}
						cout << endl;
					}
					mirrorImage.setFromPixels(_colorPixels->Data, width, height, ofImageType::OF_IMAGE_COLOR);
				}
			}
		}
		else if (displayMode == displayModeID::SKELETONS) {
			skeletonLines.clear();
			if (multiFrame->ColorFrameReference != nullptr && multiFrame->BodyFrameReference != nullptr)
			{
				auto colorFrame = multiFrame->ColorFrameReference->AcquireFrame();
				auto bodies = getBodies(multiFrame);

				vector<pair<ofVec2f, ofVec2f>> bones;

				if (colorFrame != nullptr && bodies != nullptr)
				{
					for (auto body : bodies) {
						if (!body->IsTracked)
							continue;
						for (int boneIdx = 0; boneIdx < 24; boneIdx++) {
							auto j1 = Limbs[boneIdx][0];
							auto j2 = Limbs[boneIdx][1];

							auto joint1 = body->Joints->Lookup(j1);
							auto joint2 = body->Joints->Lookup(j2);

							auto cm = getCoordinateMapper();
							_camSpacePoints[0] = joint1.Position;
							_camSpacePoints[1] = joint2.Position;

							cm->MapCameraPointsToColorSpace(_camSpacePoints, _colSpacePoints);

							auto bone = make_pair(ofVec2f(_colSpacePoints[0].X, _colSpacePoints[0].Y), ofVec2f(_colSpacePoints[1].X, _colSpacePoints[1].Y));
							bones.push_back(bone);
						}
					}

					colorFrame->CopyConvertedFrameDataToArray(_colorPixels, ColorImageFormat::Rgba);

					_frameProcessed = true;
				}
				if (_frameProcessed) {
					int height = _kinect->ColorFrameSource->FrameDescription->Height;
					int width = _kinect->ColorFrameSource->FrameDescription->Width;
					mirrorImage.setFromPixels(_colorPixels->Data, width, height, ofImageType::OF_IMAGE_COLOR_ALPHA);
					mirrorImage.update();
					if (bones.size() > 0) {
						for (auto bone : bones) {
							skeletonLines.push_back(make_pair(bone.first, bone.second));
						}
					}
				}
			}
		}
	}
	return _frameProcessed;
}

void SharedData::drawCorrectDisplayImage() {
	ofRectangle box = font.getStringBoundingBox("Kinect Not Found", 0, 0);
	ofSetColor(0);
	font.drawString("Kinect Not Found", ofGetWidth() / 2 - box.width / 2, ofGetHeight() / 2 - box.height / 2);
	if (displayMode == displayModeID::SKELETONS) {
		ofSetColor(255);
		mirrorImage.draw(0, 0);
		ofPushStyle();
		ofSetColor(0);
		for (auto skeleLine : skeletonLines) {
			ofLine(skeleLine.first, skeleLine.second);
		}
		ofPopStyle();
	}
	if (displayMode == displayModeID::INVISIBLE) {
		ofSetColor(255);
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofRect(0, 0, ofGetWidth(), ofGetHeight());
	}
	if (displayMode == displayModeID::MIRROR) {
		ofSetColor(255);
		mirrorImage.draw(0, 0);
	}
	if (displayMode == displayModeID::SILHOUETTE) {
		ofSetColor(255);
		mirrorImage.draw(0, 0, colFrameWidth, colFrameHeight);
		//mirrorImage.drawSubsection(0, 0, colFrameWidth, colFrameHeight * 424/512, 0, 0, 512, 424);
	}
}


Vector<Body^>^ SharedData::getBodies(MultiSourceFrame^ multiFrame) {
	 _frameProcessed = false;
	if (multiFrame != nullptr)
	{
		if (multiFrame->BodyFrameReference != nullptr)
		{
			auto bodyFrame = multiFrame->BodyFrameReference->AcquireFrame();

			if (bodyFrame != nullptr)
			{
				bodyFrame->GetAndRefreshBodyData(_bodies);
				_frameProcessed = true;
			}
			if (_frameProcessed) {
				return _bodies;
			}
		}
	}
	return nullptr;
}

Array<byte>^ SharedData::getAudioFrame() {
	auto audioFrames = _audioReader->AcquireLatestBeamFrames();
	if (audioFrames == nullptr)
		return nullptr;
	for (auto subFrame : audioFrames->GetAt(0)->SubFrames) {
		subFrame->CopyFrameDataToArray(_audioBuffer);
	}
	return _audioBuffer;
}

void SharedData::setImageTransform(int width, int height, int targetWidth, int targetHeight) {
	float xScale = (float)targetWidth / width;
	float yScale = (float)targetHeight / height;
	pair<ofPoint, float> returnTransform;
	if (xScale <= yScale) {
		float scale = xScale;
		int x = 0;
		int y = (targetHeight - height*scale) / 2;
		ofPoint loc(x, y);
		imgTransform = make_pair(loc, scale);
	}
	else {
		float scale = yScale;
		int y = 0;
		int x = (targetWidth - width*scale) / 2;
		ofPoint loc(x, y);
		imgTransform = make_pair(loc, scale);
	}
}

void SharedData::drawShape(int shapeId, ofRectangle &rect) {
	shapeImages[(ShapeID)shapeId].draw(rect.x, rect.y, rect.width, rect.height);
}

void SharedData::drawDisplayMode() {
	float height = smallFont.getStringBoundingBox("Display Mode: Mirror " + ofToString(displayModeID::MIRROR + 1) + "/4. Navigate with arrow keys or numbers 1 - 4.", 0, 0).height;
	switch (displayMode)
	{
		case displayModeID::MIRROR:
			smallFont.drawString("Display Mode: Mirror " + ofToString(displayModeID::MIRROR + 1) + "/4. Navigate with arrow keys or numbers 1 - 4.", 10, ofGetHeight() - height);
			break;
		case displayModeID::INVISIBLE:
			smallFont.drawString("Display Mode: Invisible " + ofToString(displayModeID::INVISIBLE + 1) + "/4. Navigate with arrow keys or numbers 1 - 4.", 10, ofGetHeight() - height);
			break;
		case displayModeID::SKELETONS:
			smallFont.drawString("Display Mode: Skeletons + Mirror " + ofToString(displayModeID::SKELETONS + 1) + "/4. Navigate with arrow keys or numbers 1 - 4.", 10, ofGetHeight() - height);
			break;
		case displayModeID::SILHOUETTE:
			ofSetColor(255);
			smallFont.drawString("Display Mode: Silhouette " + ofToString(displayModeID::SILHOUETTE + 1) + "/4. Navigate with arrow keys or numbers 1 - 4.", 10, ofGetHeight() - height);
			break;
		default:
			break;
	}
}

void SharedData::changeDisplayMode(int key) {
	switch (key) {
		case OF_KEY_LEFT:
			displayMode--;
			if (displayMode < 0) displayMode = displayModeID::NUM_MODES - 1;
			displayMode %= displayModeID::NUM_MODES;
			break;
		case OF_KEY_RIGHT:
			displayMode++;
			displayMode %= displayModeID::NUM_MODES;
			break;
		case '1':
			displayMode = displayModeID::MIRROR;
			break;
		case '2':
			displayMode = displayModeID::INVISIBLE;
			break;
		case '3':
			displayMode = displayModeID::SKELETONS;
			break;
		case '4':
			displayMode = displayModeID::SILHOUETTE;
			break;
		default:
			break;
	}
	if (displayMode == displayModeID::SILHOUETTE) {
		//setImageTransform(indexFrameWidth, indexFrameHeight, ofGetWidth(), ofGetHeight());
		//activeFrameWidth = indexFrameWidth;
		//activeFrameHeight = indexFrameHeight;
	}
	else {
		//setImageTransform(colFrameWidth, colFrameHeight, ofGetWidth(), ofGetHeight());
		//activeFrameWidth = colFrameWidth;
		//activeFrameHeight = colFrameHeight;
	}
}

void SharedData::keyboardStateChange(int key) {

}


