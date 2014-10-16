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
	bool movingPoint;
	ofVec2f* curPoint;

	void loadMatrix();
	void saveMatrix();
	bool bSaveMatrix;
	bool homographyReady;
	
	void calcHomography();
	cv::Mat homography, homography_inv;
	cv::Mat	map_matrix;
	void logHomogr();	
	ofVec2f			proyectaPunto(ofVec2f ptSrc);
	vector<ofVec2f> proyectaPuntos(vector<ofVec2f> ptsSrc);
	
	
	bool movePoint(vector<ofVec2f>& points, ofVec2f point);
	bool movePoint(Calibrador ccc, ofVec2f point);	
	
	
	Calibrador	calib1;
	ofRectangle	c1_Src;
	ofRectangle	c1_Dst;
	
};
