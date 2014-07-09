#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
    ofDisableAntiAliasing();
    
    
	box2d.init();
	box2d.setGravity(10, 0);
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
	
	
	stringL1.setup(box2d.getWorld(), ofPoint(20, ofGetHeight()*1/2), ofPoint(ofGetWidth()-20, ofGetHeight()*1/2), 16, 0.01);
	stringL2.setup(box2d.getWorld(), ofPoint(100, ofGetHeight()*1/5), ofPoint(ofGetWidth()-100, ofGetHeight()*1/5), 8);
	stringL3.setup(box2d.getWorld(), ofPoint(60, ofGetHeight()*1/3), ofPoint(ofGetWidth()-60, ofGetHeight()*2/3), 16, 0.1);
	
	stringC1.setup(box2d.getWorld(), ofPoint(100, ofGetHeight()*3/5), 150, 16, 0.60);
	
	
}


//--------------------------------------------------------------
void testApp::update() {
	box2d.update();	
}


//--------------------------------------------------------------
void testApp::draw() {
		
	stringL1.draw();
	stringL2.draw();
	stringL3.draw();
	
	stringC1.draw();
	
	string info = "";
	info += "click and pull the chain around\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	if(key == 't') ofToggleFullscreen();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
}

