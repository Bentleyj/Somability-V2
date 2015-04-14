#include "SharedData.h"

const JointType SharedData::Limbs[24][2] = {
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

void SharedData::setupKinect(FrameSourceTypes  frameSourceTypes) {
	_kinect = KinectSensor::GetDefault();
	if (_kinect != nullptr)
	{
		_multiFrameReader = _kinect->OpenMultiSourceFrameReader(frameSourceTypes);

		auto colorFrameDescription = _kinect->ColorFrameSource->CreateFrameDescription(ColorImageFormat::Rgba);

		if ((frameSourceTypes & FrameSourceTypes::Color) == FrameSourceTypes::Color) {
			// rgba is 4 bytes per pixel
			_bytesPerPixel = colorFrameDescription->BytesPerPixel;

			// allocate space to put the pixels to be rendered
			_colorPixels = ref new Array<byte>(colorFrameDescription->Width * colorFrameDescription->Height * _bytesPerPixel);
		}

		if ((frameSourceTypes & FrameSourceTypes::Body) == FrameSourceTypes::Body) {
			_bodies = ref new Vector<Body^>(_kinect->BodyFrameSource->BodyCount);
		}


		_kinect->Open();
	}
}

bool SharedData::isKinectOpen() {
	return _kinect->IsOpen;
}

void SharedData::closeKinect() {
	_kinect->Close();
}

bool SharedData::setColorImage(ofImage& img) {
	_frameProcessed = false;
	auto multiFrame = _multiFrameReader->AcquireLatestFrame();
	if (multiFrame != nullptr)
	{
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
				img.setFromPixels(_colorPixels->Data, width, height, ofImageType::OF_IMAGE_COLOR_ALPHA);
			}
		}
	}
	return _frameProcessed;
}

Vector<Body^>^ SharedData::getBodies() {
	 _frameProcessed = false;
	auto multiFrame = _multiFrameReader->AcquireLatestFrame();
	if (multiFrame != nullptr)
	{
		if (multiFrame->ColorFrameReference != nullptr)
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


