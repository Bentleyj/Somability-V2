/**
 *  EventHub.cpp
 *
 *  Created by James Bentley & Peter Daukintis on today.
 */

#include "EventHub.h"
#include <locale>

ofEvent<string> EventHub::buttonClickEvent;

void EventHub::setupEvents() {
	ofAddListener(buttonClickEvent, this, &EventHub::swapState);
}

void EventHub::swapState(string& arg) {

	changeState(arg);
}


