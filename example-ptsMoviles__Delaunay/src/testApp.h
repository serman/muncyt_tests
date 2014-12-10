#pragma once

#include "ofMain.h"
#include "ofxDelaunay.h"

#include "Particle.h"

class testApp : public ofBaseApp{
  public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
	ofVec2f zentro;
	
    ofxDelaunay triangulation;
	
	bool bFill;
	ofMesh	myMesh;
	float radio;
	void addColors();

	void addPart();
	int	nPuntosBase;
	vector<ofPoint>	puntos;
	vector<ofPoint>	vels;
	void removeParts();
	
	
	void setupColores();
	ofColor				coloresAll[6];
	ofColor				colores[3];
	ofColor				antiColores[3];	
	
};
