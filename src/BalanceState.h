/*
 *  BalanceState.h
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

class BalanceState : public EventHub
{
public:
	
	
	void setup();
	void update();
	void draw();
	//void mouseReleased(int x, int y, int button);
	string getName();

	/*
	void stateEnter();
	void stateExit();
	void shoot();
	void tryToFire();
	ofSoundPlayer boing;

	void setupGui(SomabilityGui *gui);

	ofSoundStream soundStream;
	void audioIn(float *samples, int length, int numChannels);
	void keyPressed(int k);
	float volume;
	int audioFramesSinceLastFired;
	int MIN_FRAMES_BETWEEN_FIRES;
	bool mustFire;
	ofImage gun;
	ofImage circle;
	ofxCvGrayscaleImage greyImg;
	ofxCvContourFinder contours;
	vector<ofxBox2dEdge *> persons;
	unsigned char *buff;
	
	float shootingAngle;
	///////////////////////////////////////////////////////////////////////////////////////
	// BOX2D stuff
	
	vector    <ofPtr<ofxBox2dBaseShape> >	shapes;		  //	default box2d circles
	
	
	class ShapeData {
	public:
		float birthday;
		
		ShapeData(float birthday = 0) {
			this->birthday = birthday;
		}
	};
	
	map<ofxBox2dBaseShape*,ShapeData> data;
	bool shapeIsTooOld(float currTime, ofxBox2dBaseShape *shape);
	//
	///////////////////////////////////////////////////////////////////////////////////////
	float MAX_SHAPE_AGE;
	float sensitivity;
	float displayVolume;
*/
};

