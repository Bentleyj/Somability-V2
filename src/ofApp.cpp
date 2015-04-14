#include "ofApp.h"
#include <winbase.h>

//--------------------------------------------------------------
void ofApp::setup()
{
	//enable events for all states
	//stateMachine.enableKeyEvents();
	//stateMachine.enableMouseEvents();

	//set up variables in shared data

	//add all the states
	stateMachine.addState<FlowState>();
	stateMachine.addState<BalanceState>();
	stateMachine.addState<ReachState>();

	stateMachine.getSharedData().setupKinect(FrameSourceTypes::Color | FrameSourceTypes::Body);

	//set initial state
	stateMachine.changeState("balance");

	//stateMachine.getSharedData().setupKinect(Frame);
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
