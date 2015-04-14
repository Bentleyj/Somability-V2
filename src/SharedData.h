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

using namespace WindowsPreview::Kinect;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

class SharedData {

public:

	void setupKinect(FrameSourceTypes  frameSourceTypes);
	bool setColorImage(ofImage& img);
	Vector<Body^>^ getBodies();
	bool isKinectOpen();
	void closeKinect();

	const static JointType Limbs[24][2];

	KinectSensor^ _kinect;
	MultiSourceFrameReader^ _multiFrameReader;
	ofTexture _tex;
	ofImage _img;
	Vector<Body^>^ _bodies;
	Array<byte>^ _colorPixels;

	unsigned int _bytesPerPixel;
	bool _frameProcessed;

};