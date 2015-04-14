/**
 *
 *  Description: 
 *				 
 *  EventHub.h, created by JB & PD on today.
 */

#pragma once
#include "ofEvents.h"
#include "ofxStateMachine.h"
#include "SharedData.h"

class EventHub : public itg::ofxState<SharedData> {
public:
	static ofEvent<string> buttonClickEvent;
	void setupEvents();
	void swapState(string& arg);
};
