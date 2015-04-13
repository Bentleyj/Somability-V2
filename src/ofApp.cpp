#include "ofApp.h"
#include <winbase.h>

//--------------------------------------------------------------
void ofApp::setup()
{
	_kinect = KinectSensor::GetDefault();
	if (_kinect != nullptr)
	{
		_multiFrameReader = _kinect->OpenMultiSourceFrameReader(FrameSourceTypes::Color);

		auto colorFrameDescription = _kinect->ColorFrameSource->CreateFrameDescription(ColorImageFormat::Rgba);

		// rgba is 4 bytes per pixel
		_bytesPerPixel = colorFrameDescription->BytesPerPixel;

		// allocate space to put the pixels to be rendered
		_colorPixels = ref new Array<byte>(colorFrameDescription->Width * colorFrameDescription->Height * _bytesPerPixel);

		_kinect->Open();
	}
}

void ofApp::exit()
{
	_kinect->Close();
}

//--------------------------------------------------------------
void ofApp::update()
{
	// Pull the frame...
	{
		auto multiFrame = _multiFrameReader->AcquireLatestFrame();
		if (multiFrame != nullptr)
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
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofClear(255, 0, 0);

	if (!_kinect->IsAvailable)
	{
		ofDrawBitmapString("No Kinect", 50, 10);
		return;
	}
	_img.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
	stringstream ss;
	ss << "mousePressed: " << x << ", " << y << " button: " << button << endl;
	auto vr = ss.str();
	OutputDebugStringA(vr.c_str());
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
	_img.resize(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
