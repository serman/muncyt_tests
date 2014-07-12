#pragma once

#include "ofMain.h"
#include "ofxTuio.h"
#include "tangibleObject.h"
#include "handShadow.h"
#include "tangiblesHandler.h"
#include "movingObj.h"
#include "consts.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void	tuioAdded(ofxTuioCursor & tuioCursor);
	void	tuioRemoved(ofxTuioCursor & tuioCursor);
	void	tuioUpdated(ofxTuioCursor & tuioCursor);
    
    ofxTuioClient   tuioClient;
    ofRectangle slider1;
    ofRectangle moveRect;
    ofRectangle jumpingRect;
    ofColor jcolor;
    
    int id_slider1;
    int id_moverect;
    int id_jumpingRect;
    movingObj o1;

    tangiblesHandler touchElements;
    void showDebug();
    float puntoX;
    float puntoY;
};
