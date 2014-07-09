#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "Strings.h"

// -------------------------------------------------

class testApp : public ofBaseApp {
	
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
	void resized(int w, int h);

	ofxBox2d	box2d;			  //	the box2d world

	StringL		stringL1, stringL2, stringL3;
	StringC		stringC1, stringC2, stringC3;
	
};



