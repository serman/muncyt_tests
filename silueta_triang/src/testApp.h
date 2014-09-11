#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxDelaunay.h"
#include "ofxXmlSettings.h"
#include "ofxUI.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void exit(); 
	
	void keyPressed(int key);

	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	
	ofVideoGrabber cam;
	ofxCv::RunningBackground background;
	ofImage thresholded;
	float	thres, lr;

	// test
	ofImage bck, foreg;
	
	ofxCv::ContourFinder contourFinder;
	
	bool doTriang;
	ofxDelaunay triangulation;
	bool	bAddPts;		// si a–ade puntos a la triangulacion
	bool	bSoloEnContorno;	// si muestra solo los triangulos de dentro del contorno
	ofMesh	triangContMesh;
	ofMesh	triangContMesh_old;
	int numPointsXtra;
	
	bool doTessel;
	ofTessellator tessel;
	ofMesh contornoTes;	// contorno teselado
	
	
	// Colores
	vector<ofColor> colorFluor;
	void cargaColores();
	
	
	bool bFill;
	
	int modoFill;
	
	bool bDrawOld;
	
	// GUI
	ofxUISuperCanvas *gui;
	void guiEvent(ofxUIEventArgs &e);
	void setupGUI();
	
	
	
};
