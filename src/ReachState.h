/*
 *  ReachState.h
 *
 *  based on GreenState.h Copyright (c) 2011, Neil Mendoza, http://www.neilmendoza.com
 *  All rights reserved. 
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met: 
 *  
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer. 
 *  * Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *  * Neither the name of 16b.it nor the names of its contributors may be used 
 *    to endorse or promote products derived from this software without 
 *    specific prior written permission. 
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 *  POSSIBILITY OF SUCH DAMAGE. 
 *
 */
#pragma once

#include "EventHub.h"

class ReachState : public EventHub
{
public:
	void setup();
	void update();
	void draw();
	string getName();

	int numPeople;
	
    void keyPressed(int key);
	//void setupGui(SomabilityGui *gui);

	//void mouseReleased(int x, int y, int button);
    vector    <ofPtr<ofxBox2dBaseShape> >	shapes;		  //	default box2d circles
	void drawShape(int shapeId, ofRectangle &rect);
	float shapeSize;
	//
	map<SharedData::ShapeID, ofImage> shapeImages;

	Array<CameraSpacePoint>^ _camSpacePoints;
	Array<ColorSpacePoint>^ _colSpacePoints;

	class ShapeData {
	public:
		float birthday;
		SharedData::ShapeID type;
		ShapeData(SharedData::ShapeID type = SharedData::TRIANGLE, float birthday = 0) {
			this->birthday = birthday;
			this->type = type;
		}
	};
	//
	//
	void addShape(SharedData::ShapeID name, ofVec2f pos);

	void stateEnter();
	void stateExit();
	//
	//	
	int lastSpawnTime;
	map<ofxBox2dBaseShape*,ShapeData> data;
	bool shapeIsTooOld(float currTime, ofxBox2dBaseShape *shape);
	void setColorForShape(SharedData::ShapeID t);
	void initTriggers();
	//
	//
	vector<pair<pair<SharedData::ShapeID, ofRectangle>, float>> triggers;

	enum Hand {
		LEFT_HAND,
		RIGHT_HAND
	};
	//
	SharedData::ShapeID handTouching[2];

	//
	//void handMoved(ofVec2f p, Hand hand);
	//void mouseMoved(int x, int y);
	//void drawFluffBall(ofVec2f p, float radius);
	
};
