#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "Calibrador.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void keyPressed(int key);
	
	ofImage left, right, warpedColor;
	vector<ofVec2f> leftPoints, rightPoints;
	bool movingPoint, isDstPt;
	ofVec2f* curPoint;
	
	Calibrador	calib1;
	ofRectangle	c1_Src;
	ofRectangle	c1_Dst;

	bool movePoint(vector<ofVec2f>& points, ofVec2f point);
		
	bool bSaveMatrix;
	
	ofImage		imgCalibr;
	
};
