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

	bool setCorrectDisplayImage(MultiSourceFrame^ multiFrame);
	void drawCorrectDisplayImage();
	Vector<Body^>^ getBodies(MultiSourceFrame^ multiFrame);
	CoordinateMapper^ getCoordinateMapper() { return _kinect->CoordinateMapper; };
	MultiSourceFrame^ getMultiSourceFrame();
	Array<byte>^ getAudioFrame();
	bool isVolOverThreshold(float threshold);
	void setImageTransform(int width, int height, int targetWidth, int targetHeight);
	void changeDisplayMode(int key);
	bool isKinectOpen();
	void closeKinect();
	void drawDisplayMode();
	void keyboardStateChange(int key);
	ofTrueTypeFont font, smallFont;

	vector<std::pair<ofVec2f, ofVec2f>> skeletonLines;

	ofImage mirrorImage;

	void drawShape(int shapeId, ofRectangle &rect);

	enum ShapeID {
		SQUARE,
		TRIANGLE,
		HEXAGON,
		CIRCLE,
		NUM_SHAPES,
		NO_SHAPE
	};

	enum displayModeID {
		MIRROR,
		INVISIBLE,
		SKELETONS,
		SILHOUETTE,
		NUM_MODES
	};

	int displayMode;

	map<ShapeID, ofImage> shapeImages;

	const static JointType Limbs[24][2];

	pair<ofPoint, float> imgTransform;

	KinectSensor^ _kinect;
	MultiSourceFrameReader^ _multiFrameReader;
	AudioBeamFrameReader^ _audioReader;
	Array<byte>^ _audioBuffer;
	Vector<Body^>^ _bodies;
	Array<byte>^ _colorPixels;
	Array<byte>^ _bodyIndexPixels;

	Array<CameraSpacePoint>^ _camSpacePoints;
	Array<ColorSpacePoint>^ _colSpacePoints;

	bool _frameProcessed;
};