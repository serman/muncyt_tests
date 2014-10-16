/*
 *  Calibrador.h
 *  homography_calibracion
 *
 *  Created by guillermo casado on 15/10/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CALIBRADOR
#define CALIBRADOR

#include "ofxCv.h"
using namespace cv;
class Calibrador {

public:
	Calibrador();
	
	vector <ofVec2f>	ptsSrc, ptsDst;
//	vector <ofVec2f>	leftPoints, rightPoints;
	bool		movingPoint;
	ofVec2f*	curPoint;
	
	void addPts(ofVec2f ptS, ofVec2f ptD);
	
	void calcHomography();
	cv::Mat homography, homography_inv;
	cv::Mat	map_matrix;
	void logHomogr();	
	
	ofVec2f			transf_Punto(ofVec2f pt);
	vector<ofVec2f> transf_Puntos(vector<ofVec2f> pts);

	void loadMatrix(string fileName);
	void saveMatrix(string fileName);
	void reset();
	
	bool homographyReady;
	
	int size();

};

#endif