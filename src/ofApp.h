#pragma once

#include "ofMain.h"

using namespace WindowsPreview::Kinect;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	private:
		KinectSensor^ _kinect;
		MultiSourceFrameReader^ _multiFrameReader;
		ofTexture _tex;
		ofImage _img;
		Array<byte>^ _colorPixels;

		unsigned int _bytesPerPixel;
		bool _colorFrameProcessed;
};
