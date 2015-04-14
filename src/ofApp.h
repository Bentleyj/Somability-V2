#pragma once

#include "ofMain.h"
#include "ofxStateMachine.h"
#include "SharedData.h"
#include "FlowState.h"
#include "ofEvents.h"
#include "EventHub.h"

using namespace WindowsPreview::Kinect;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

class ofApp : public ofBaseApp, public EventHub{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		std::string getName();

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

		itg::ofxStateMachine<SharedData> stateMachine; //state macgine that runs everything
};
