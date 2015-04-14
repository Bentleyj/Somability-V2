/**
 *  EventHub.cpp
 *
 *  Created by James Bentley & Peter Daukintis on today.
 */

#include "EventHub.h"

ofEvent<int> EventHub::buttonClickEvent;

void EventHub::setupEvents() {

	ofAddListener(buttonClickEvent, this, &EventHub::swapState);
}

void EventHub::swapState(int& arg) {

}


