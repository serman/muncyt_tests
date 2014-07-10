#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
    ofDisableAntiAliasing();
    
    cargaColores();
	
	box2d.init();
	box2d.setGravity(10, 0);
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
	
//	stringL1.setup(box2d.getWorld(), ofPoint(20, ofGetHeight()*1/2), ofPoint(ofGetWidth()-20, ofGetHeight()*1/2), 16, 0.01);
	stringL2.setup(box2d.getWorld(), ofPoint(100, ofGetHeight()*1/5), ofPoint(ofGetWidth()-100, ofGetHeight()*1/5), 8);
//	stringL3.setup(box2d.getWorld(), ofPoint(60, ofGetHeight()*1/3), ofPoint(ofGetWidth()-60, ofGetHeight()*2/3), 16, 0.1);
	
	stringC1.setup(box2d.getWorld(), ofPoint(200, ofGetHeight()*3/5), 150, 16, 0.9, 0.95);
	stringC2.setup(box2d.getWorld(), ofPoint(550, ofGetHeight()*3/5), 100, 8, 0.3, 0.95);
	stringC3.setup(box2d.getWorld(), ofPoint(700, ofGetHeight()*3/5), 120, 8, 1.5, 1.6);
	
	aro1.setup(ofPoint(700,300), 30);
	aro2.setup(ofPoint(700,600), 70);
	
	aro2.wrot=1.05;
	
	for(int i=0; i<10; i++) {
		ofPoint p=ofPoint(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
		Ring rr;
		rr.setup(p, ofRandom(10,40));		
		rr.wrot=ofRandom(-1.3,1.3);
		rr.color=colorFluor[i%colorFluor.size()];
		aros.push_back(rr);
	}
	
				
}


void testApp::cargaColores() {
	// paleta de colores fluor
	// http://www.colourlovers.com/palette/970972/FLUOR
	//
	colorFluor.clear();
	colorFluor.push_back(ofColor::fromHex(0x0DE0FC));	// melting flowers
	colorFluor.push_back(ofColor::fromHex(0x38FC48));	// Dead Nuclear
	colorFluor.push_back(ofColor::fromHex(0xF938FC));	// Calcinha
	colorFluor.push_back(ofColor::fromHex(0xFA00AB));	// ow!
	colorFluor.push_back(ofColor::fromHex(0xDFFC00));	// Limei Green
	
}

//--------------------------------------------------------------
void testApp::update() {
	box2d.update();	
	
	aro1.update();
	aro2.update();

	for(int i=0; i<aros.size(); i++) {
		aros[i].update();
	}
}


//--------------------------------------------------------------
void testApp::draw() {
		
	stringL1.draw();
	stringL2.draw();
	stringL3.draw();
	
	stringC1.draw();
	stringC2.draw();
	stringC3.draw();
	
	ofPushStyle();
	ofSetColor(colorFluor[2]);
	aro1.draw();
	ofSetColor(colorFluor[1]);
	aro2.draw();
	ofPopStyle();
	
	for(int i=0; i<aros.size(); i++) {
		aros[i].draw();
	}
	
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

