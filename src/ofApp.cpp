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

	//load size of color stream
	stateMachine.getSharedData().colFrameWidth = stateMachine.getSharedData()._kinect->ColorFrameSource->FrameDescription->Width;
	stateMachine.getSharedData().colFrameHeight = stateMachine.getSharedData()._kinect->ColorFrameSource->FrameDescription->Height;

	//load size of index stream
	stateMachine.getSharedData().indexFrameWidth = stateMachine.getSharedData()._kinect->BodyIndexFrameSource->FrameDescription->Width;
	stateMachine.getSharedData().indexFrameHeight = stateMachine.getSharedData()._kinect->BodyIndexFrameSource->FrameDescription->Height;

	//seActive Frame initially
	stateMachine.getSharedData().activeFrameWidth = stateMachine.getSharedData().colFrameWidth;
	stateMachine.getSharedData().activeFrameHeight = stateMachine.getSharedData().colFrameHeight;

	//set Mesh Mode for skeleton Mesh
	stateMachine.getSharedData().skeletonMesh.setMode(OF_PRIMITIVE_TRIANGLES);

	//set up variables in shared data for box2d
	stateMachine.getSharedData().box2d->init();
	stateMachine.getSharedData().box2d->setGravity(0, 5);
	stateMachine.getSharedData().box2d->createBounds(0, 0, stateMachine.getSharedData().colFrameWidth, stateMachine.getSharedData().colFrameHeight);
	stateMachine.getSharedData().box2d->setFPS(30.0);

	stateMachine.getSharedData().setImageTransform(stateMachine.getSharedData().colFrameWidth, stateMachine.getSharedData().colFrameHeight, ofGetWidth(), ofGetHeight());

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
	stateMachine.getSharedData().setImageTransform(stateMachine.getSharedData()._kinect->ColorFrameSource->FrameDescription->Width, stateMachine.getSharedData()._kinect->ColorFrameSource->FrameDescription->Height, w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
