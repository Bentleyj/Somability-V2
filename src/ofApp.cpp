#include "ofApp.h"
#include <winbase.h>

//--------------------------------------------------------------
void ofApp::setup()
{
	ofDisableArbTex();
	ofSetLogLevel(OF_LOG_VERBOSE);
	//enable events for all states
	stateMachine.enableKeyEvents();
	stateMachine.enableMouseEvents();

	//load Shared Font
	stateMachine.getSharedData().font.loadFont("fonts/verdana.ttf", 30);
	stateMachine.getSharedData().smallFont.loadFont("fonts/verdana.ttf", 10);


	//set up variables in shared data
	stateMachine.getSharedData().box2d->init();
	stateMachine.getSharedData().box2d->setGravity(0, 5);
	stateMachine.getSharedData().box2d->createBounds(0, 0, ofGetWidth(), ofGetHeight());
	stateMachine.getSharedData().box2d->setFPS(30.0);

	//Load Images
	stateMachine.getSharedData().shapeImages[SharedData::CIRCLE].loadImage("imgs/circle.png");
	stateMachine.getSharedData().shapeImages[SharedData::SQUARE].loadImage("imgs/square.png");
	stateMachine.getSharedData().shapeImages[SharedData::HEXAGON].loadImage("imgs/hexagon.png");
	stateMachine.getSharedData().shapeImages[SharedData::TRIANGLE].loadImage("imgs/triangle.png");

	//add all the states
	stateMachine.addState<FlowState>();
	stateMachine.addState<BalanceState>();
	stateMachine.addState<ReachState>();

	stateMachine.getSharedData().setupKinect(FrameSourceTypes::Color | FrameSourceTypes::Body | FrameSourceTypes::BodyIndex);

	stateMachine.getSharedData()._camSpacePoints = ref new Array<CameraSpacePoint>(2);
	stateMachine.getSharedData()._colSpacePoints = ref new Array<ColorSpacePoint>(2);


	int width = stateMachine.getSharedData()._kinect->ColorFrameSource->FrameDescription->Width;
	int height = stateMachine.getSharedData()._kinect->ColorFrameSource->FrameDescription->Height;

	stateMachine.getSharedData().setImageTransform(width, height, ofGetWidth(), ofGetHeight());

	stateMachine.getSharedData().displayMode = stateMachine.getSharedData().MIRROR;

	//set initial state
	stateMachine.changeState("flow");
}

void ofApp::exit()
{
	stateMachine.getSharedData().closeKinect();
}

//--------------------------------------------------------------
void ofApp::update()
{

}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofClear(255, 0, 0);

	if (!stateMachine.getSharedData().isKinectOpen())
	{
		ofDrawBitmapString("No Kinect", 50, 10);
		return;
	}
}

std::string ofApp::getName() {
	return "ofApp";
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

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
//	_img.resize(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
