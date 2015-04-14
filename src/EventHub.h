/**
 *
 *  Description: 
 *				 
 *  EventHub.h, created by JB & PD on today.
 */

#pragma once
#include "ofEvents.h"

class EventHub {
public:
	static ofEvent<int> buttonClickEvent;
	void setupEvents();
	void swapState(int& arg);
};
