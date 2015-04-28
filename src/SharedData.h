/**     ___           ___           ___                         ___           ___     
 *     /__/\         /  /\         /  /\         _____         /  /\         /__/|    
 *    |  |::\       /  /::\       /  /::|       /  /::\       /  /::\       |  |:|    
 *    |  |:|:\     /  /:/\:\     /  /:/:|      /  /:/\:\     /  /:/\:\      |  |:|    
 *  __|__|:|\:\   /  /:/~/::\   /  /:/|:|__   /  /:/~/::\   /  /:/  \:\   __|__|:|    
 * /__/::::| \:\ /__/:/ /:/\:\ /__/:/ |:| /\ /__/:/ /:/\:| /__/:/ \__\:\ /__/::::\____
 * \  \:\~~\__\/ \  \:\/:/__\/ \__\/  |:|/:/ \  \:\/:/~/:/ \  \:\ /  /:/    ~\~~\::::/
 *  \  \:\        \  \::/          |  |:/:/   \  \::/ /:/   \  \:\  /:/      |~~|:|~~ 
 *   \  \:\        \  \:\          |  |::/     \  \:\/:/     \  \:\/:/       |  |:|   
 *    \  \:\        \  \:\         |  |:/       \  \::/       \  \::/        |  |:|   
 *     \__\/         \__\/         |__|/         \__\/         \__\/         |__|/   
 *
 *  Description: 
 *				 
 *  Trail.h, created by Marek Bereza on 17/01/2014.
 */

#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"

using namespace WindowsPreview::Kinect;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

class SharedData {

public:
	SharedData() {
		box2d.reset(new ofxBox2d());
	};
	ofPtr<ofxBox2d> box2d;
	void setupKinect(FrameSourceTypes  frameSourceTypes);

	bool setColorImage(ofImage& img, MultiSourceFrame^ multiFrame);
	Vector<Body^>^ getBodies(MultiSourceFrame^ multiFrame);
	CoordinateMapper^ getCoordinateMapper() { return _kinect->CoordinateMapper; };
	MultiSourceFrame^ getMultiSourceFrame();
	Array<byte>^ getAudioFrame();
	bool isVolOverThreshold(float threshold);
	void setImageTransform(int width, int height, int targetWidth, int targetHeight);
	bool isKinectOpen();
	void closeKinect();

	void drawShape(int shapeId, ofRectangle &rect);

	enum ShapeID {

		SQUARE,
		TRIANGLE,
		HEXAGON,
		CIRCLE,
		NUM_SHAPES,
		NO_SHAPE
	};

	map<ShapeID, ofImage> shapeImages;


	const static JointType Limbs[24][2];

	pair<ofPoint, float> imgTransform;

	KinectSensor^ _kinect;
	MultiSourceFrameReader^ _multiFrameReader;
	AudioBeamFrameReader^ _audioReader;
	Array<byte>^ _audioBuffer;
	ofTexture _tex;
	ofImage _img;
	Vector<Body^>^ _bodies;
	Array<byte>^ _colorPixels;

	unsigned int _bytesPerPixel;
	bool _frameProcessed;

};