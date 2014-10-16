#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	
	left.loadImage("left.jpg");
	right.loadImage("right.jpg");
	imitate(warpedColor, right);
	
	movingPoint = false;
	homographyReady = false;

}

void testApp::update() {

//	calcHomography();	// solo e calcula cuando se a–aden o modifican m‡s puntos y hay al menos 4
	
	if(homographyReady) {
		// this is how you warp one ofImage into another ofImage given the homography matrix
		// CV INTER NN is 113 fps, CV_INTER_LINEAR is 93 fps
		warpPerspective(right, warpedColor, homography, CV_INTER_LINEAR);
		warpedColor.update();
	}
}

void testApp::calcHomography() {

	if(leftPoints.size() >= 4) {
		vector<Point2f> srcPoints, dstPoints;
		for(int i = 0; i < leftPoints.size(); i++) {
			srcPoints.push_back(Point2f(rightPoints[i].x - left.getWidth(), rightPoints[i].y));
			dstPoints.push_back(Point2f(leftPoints[i].x, leftPoints[i].y));
		}

		// generate a homography from the two sets of points
		homography = findHomography(Mat(srcPoints), Mat(dstPoints));
		homographyReady = true;
		cv::invert(homography, homography_inv);
		
		// getPerspective
		// esto necesita 4 puntos (ni mas ni menos)
		srcPoints.erase(srcPoints.begin()+4,srcPoints.end());
		dstPoints.erase(dstPoints.begin()+4,dstPoints.end());

		map_matrix = getPerspectiveTransform(srcPoints, dstPoints);
	}
//	ofLogNotice() << "finHmography";

}

void testApp::saveMatrix() {
	if(homographyReady) {
		FileStorage fs(ofToDataPath("homography.yml"), FileStorage::WRITE);
		fs << "homography" << homography;
		
		ofFile fileL("ptsHomogL.txt", ofFile::WriteOnly);
		for(int i=0; i<leftPoints.size(); i++) {
			fileL << leftPoints[i] << endl;
		}
		ofFile fileR("ptsHomogR.txt", ofFile::WriteOnly);
		for(int i=0; i<rightPoints.size(); i++) {
			fileR << rightPoints[i] << endl;
		}
	}
}

void testApp::loadMatrix() {
	bool ok1 = false;
	bool ok2 = false;
	bool ok3 = false;
//	ofLogNotice("LOAD HOMOGR");
	// load the previous homography if it's available
	ofFile previous("homography.yml");
	if(previous.exists()) {
		FileStorage fs(ofToDataPath("homography.yml"), FileStorage::READ);
		fs["homography"] >> homography;
//		calcHomography();
		homographyReady = true;
		
		ok1 = true;
	}
	
//	ofLogNotice("PTS_L");
	ofFile fileL("ptsHomogL.txt");
	if(fileL.exists()) {
		leftPoints.clear();
		vector<float>coords;
		while(fileL) {
			string svv;
			fileL >> svv;
			vector<string> vals = ofSplitString(svv,", ");
			coords.push_back(ofToFloat(vals[0]));
		}
//		ofLogNotice("coords: " + ofToString(coords.size()) + "    " + ofToString(coords));
		if(coords.size()>0) {
			for(int i=0; i<coords.size()-1; i+=2) {
			ofVec2f ptL = ofVec2f(coords[i], coords[i+1]);
			leftPoints.push_back(ptL);
			}
		}
//		ofLogNotice("PTSL: " + ofToString(leftPoints));
		
		ok2=true;
	}
//	ofLogNotice(ofToString(leftPoints));
	
	
//	ofLogNotice("PTS_R");
	ofFile fileR("ptsHomogR.txt");
	if(fileR.exists()) {
		rightPoints.clear();
		vector<float>coords;
		while(fileR) {
			string svv;
			fileR >> svv;
			vector<string> vals = ofSplitString(svv,", ");
			coords.push_back(ofToFloat(vals[0]));
		}
//		ofLogNotice("coords: " + ofToString(coords.size()) + "    " + ofToString(coords));
		if(coords.size()>0) {
			for(int i=0; i<coords.size()-1; i+=2) {
				ofVec2f ptR = ofVec2f(coords[i], coords[i+1]);
				rightPoints.push_back(ptR);
			}
		}
//		ofLogNotice("PTSL: " + ofToString(rightPoints));
		ok3=true;
	}
//	ofLogNotice(ofToString(rightPoints));	
	if(ok1 && ok2 && ok3) 		calcHomography();

}

void testApp::logHomogr() {
	if(homographyReady) {
		ofLogNotice("homography");
		int col = homography.cols;
		int row = homography.rows;
		for(int j=0; j<row; j++) {
			string rr = "";
			for(int i=0; i<col; i++) {
				rr+=ofToString(homography.at<float>(i,j)) + "   " ;
			}
			ofLogNotice() << rr;
		}	
		
		ofLogNotice("map_matrix");
		col = map_matrix.cols;
		row = map_matrix.rows;
		ofLogNotice() << col << "," << row;;
		for(int j=0; j<row; j++) {
			string rr = "";
			for(int i=0; i<col; i++) {
				rr+=ofToString(map_matrix.at<float>(i,j)) + "   " ;
//				rr+=ofToString(map_matrix.at<float>(i,j)) + "   " ;
			}
			ofLogNotice() << rr;
		}	
		
	}
}

ofVec2f testApp::proyectaPunto(ofVec2f ptSrc) {	
	ofVec2f ppt = ofVec2f(0,0);
	if(homographyReady) { 
		vector<cv::Vec2f> psSrc, psDst;
		psSrc.push_back(toCv(ptSrc));
		psSrc.push_back(cv::Vec2f(0,0));
		
		// invertir!
		//// metodo de matriz con puntos
		// http://stackoverflow.com/questions/17852182/runnig-cvwarpperspective-on-points
	//	perspectiveTransform(psSrc, psDst, homography.inv());// map_matrix);
	//	perspectiveTransform(psSrc, psDst, map_matrix.inv());
		perspectiveTransform(psSrc, psDst, homography_inv);// map_matrix);
		
	// para varios puntos:
		// void perspectiveTransform(vector<Point2f> origPoints, vector<Point2f> transformedPoints, Mat h)		
		// http://answers.opencv.org/question/1858/homography-for-cordinates-of-poins/#1859
		
		ppt = toOf(psDst[0]);
	}
	else {
		ppt = ptSrc;
	}
	return ppt;
	
	// otros http://stackoverflow.com/questions/17852182/runnig-cvwarpperspective-on-points	
}

vector<ofVec2f> testApp::proyectaPuntos(vector<ofVec2f> ptsSrc) {	
	
	vector<ofVec2f> ppts;
	if(homographyReady) { 
		vector<cv::Vec2f> psSrc, psDst;
		for(int i=0; i<ptsSrc.size(); i++) {
			psSrc.push_back(toCv(ptsSrc[i]));
		}
		
		// invertir!
		//// metodo de matriz con puntos
		// http://stackoverflow.com/questions/17852182/runnig-cvwarpperspective-on-points
		//	perspectiveTransform(psSrc, psDst, homography.inv());// map_matrix);
		//	perspectiveTransform(psSrc, psDst, map_matrix.inv());
		perspectiveTransform(psSrc, psDst, homography_inv);// map_matrix);
		
		// para varios puntos:
		// void perspectiveTransform(vector<Point2f> origPoints, vector<Point2f> transformedPoints, Mat h)		
		// http://answers.opencv.org/question/1858/homography-for-cordinates-of-poins/#1859
		
		for(int i=0; i<psDst.size(); i++) {
			ppts.push_back(toOf(psDst[i]));
		}
	}
	else {
		ppts = ptsSrc;
	}
	return ppts;
	
	// otros http://stackoverflow.com/questions/17852182/runnig-cvwarpperspective-on-points	
	
	
	
}

void drawPoints(vector<ofVec2f>& points) {
	ofNoFill();
	for(int i = 0; i < points.size(); i++) {
		ofCircle(points[i], 10);
		ofCircle(points[i], 1);
	}
}

void testApp::draw() {
	
	ofSetColor(255);
	left.draw(0, 0);
	right.draw(left.getWidth(), 0);
	if(homographyReady) {
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
		ofSetColor(255, 128);
		warpedColor.draw(0, 0);
		ofDisableBlendMode();
		
		
		ofSetColor(ofColor::purple);
		ofNoFill();
		ofEllipse(ofGetMouseX(), ofGetMouseY(), 15,15);
		
		ofVec2f proy = proyectaPunto( ofVec2f(ofGetMouseX(), ofGetMouseY()) );
		ofSetColor(ofColor::green);
		ofPushMatrix();
		ofTranslate(left.getWidth(), 0, 0);
			ofEllipse(proy.x, proy.y, 20,20);
		ofPopMatrix();
		
	}
	
	ofSetColor(ofColor::red);
	drawPoints(leftPoints);
	ofSetColor(ofColor::blue);
	drawPoints(rightPoints);
	ofSetColor(128);
	for(int i = 0; i < leftPoints.size(); i++) {
		ofLine(leftPoints[i], rightPoints[i]);
	}
	
	ofSetColor(255);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
	ofDrawBitmapString(ofToString(warpedColor.getWidth())+","+ofToString(warpedColor.getHeight()), 10, 40);
	
}

bool testApp::movePoint(vector<ofVec2f>& points, ofVec2f point) {
	for(int i = 0; i < points.size(); i++) {
		if(points[i].distance(point) < 20) {
			movingPoint = true;
			curPoint = &points[i];
			return true;
		}
	}
	return false;
}

void testApp::mousePressed(int x, int y, int button) {
	ofVec2f cur(x, y);
	ofVec2f rightOffset(left.getWidth(), 0);
	if(!movePoint(leftPoints, cur) && !movePoint(rightPoints, cur)) {
		if(x > left.getWidth()) {
			cur -= rightOffset;
		}
		leftPoints.push_back(cur);
		rightPoints.push_back(cur + rightOffset);
		
		calcHomography();
	}
}

void testApp::mouseDragged(int x, int y, int button) {
	if(movingPoint) {
		curPoint->set(x, y);
		calcHomography();
	}
}

void testApp::mouseReleased(int x, int y, int button) {
	movingPoint = false;
}

void testApp::keyPressed(int key) {
	if(key == 's') {
		saveMatrix();
	}
	// GGGG reset
	if(key=='r') {
		homographyReady = false;
		leftPoints.clear();
		rightPoints.clear();
	}
	
	if(key=='l') 	loadMatrix();
	
	if(key=='p') logHomogr();
}