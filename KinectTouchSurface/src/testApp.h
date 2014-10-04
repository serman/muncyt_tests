#ifndef _TEST_APP
#define _TEST_APP

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxXmlSettings.h"
#include "ofxUI.h"

#include "BlobX.h"
#include "TUIO.h"


class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	ofxKinect kinect;
	// ofxOpenNI openNIDevice;			//Depth camera
	
	int wk, hk;	// domensiones de la imagen de la kinect
	
	int modo;	
	
	// MODO 0
	int W, H;						//Screen size
	vector<ofPoint> corners; 		//Marked corners of the surface
	ofShortPixels backgroundDepth;	//Background depth image
	
	ofRectangle	rectCalib;	// zona donde se podra hacer la calibracion
	
	ofPixels outPixels;	//Pixels array holding current drawing
	ofImage  outImage;	//Output image built from outPixels
	
	bool	calibrating;	//Working mode selector : calibrate or draw
	ofPoint shift;			//Point used for drawing shifted color image
	
	bool getBackground;	// update background para bck-substraction
	
	float minV;		//Minimal distance in mm
	float maxV; 	//Maximal distance in mm
	
	void updateModo0();
	void updateModo1();

	void	drawMarco(ofRectangle rect);
	void	drawMarco(float xx, float yy, float ww, float hh);

	
	
	
	// MODO 1
	// http://forum.openframeworks.cc/t/universal-multitouch-wall-using-microsoft-sdk-openframeworks-and-ofxkinectnui/9908/37

	// Presentacion
	int wk1, hk1;
	ofRectangle	rect_RGB, rect_Depth, rect_Bckgnd, rect_Blobs;
	
	
	ofxCvGrayscaleImage grayImage; // grayscale depth image


	ofImage					colorImg;  
	
	ofxCvGrayscaleImage		tempGrayImage;  
	ofxCvGrayscaleImage		paso_openCV;
	ofxCvGrayscaleImage		grayImage_No_Blur;	
	ofxCvGrayscaleImage		backgroundImage;	// para ver el background
//	ofxCvFloatImage		tempGrayImage;  
//	ofxCvShortImage		backgroundImage;	// para ver el background


	// background substraction
	// mejorara con shortImages o floatImages
	ofxCvGrayscaleImage	grayThreshNear;
	ofxCvGrayscaleImage	grayThreshFar;
	float				thresholdLow, thresholdHigh;
	bool				bLearnBakground;
	
	
	// ContourFinder CF
	ofxCv::ContourFinder contourFinderX;
	void	drawBlobs(int wk1, int hk1);
	int		max_blobs;
	float	min_blob_size, max_blob_size;
	bool	bFindHoles;
	float				threshold;			// contraste
	int					blurcv;	
	bool bt;	// activar o no el threshold antes de cfX
	
	bool	bWarp;
	
	
	// Calibracion
	bool	bSetZentros;
	bool	bZentro, bZentroDepth;
	ofVec2f	zentro, zentroDepth;
	ofVec2f	zentroDst;
	ofPoint					entrada[4];				// imagen de la camara
	ofPoint					destino_camera_warp[4];	// imagen generada	
	
	
	//
	// GUI
	//
	void	setupGUI();
	ofxUISuperCanvas *gui1;	
	void	guiEvent(ofxUIEventArgs &e);

	// modo reducido
	bool	bPeque;
	
	// Settings en XML
	void loadXMLSettings();
	void saveSettings();
	
	
	
	//Communication
	// TUIO viene con la clas de tracking/contourFinder de ofxNCore (que no la cargo)
	// Si se quiere reutilizar hay que sustituir una clase llamada Blob
	bool	bTUIO;
	TUIO				myTUIO;
	vector<BlobX>	blobs;
//	string				tmpLocalHost;
//  int					tmpPort;
//	int					tmpFlashPort;
	
	
};

#endif
