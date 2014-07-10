#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofEnableAntiAliasing();

	box2d.init();
	box2d.setGravity(0, 0);
	box2d.setFPS(30.0);

	isKeyPressed = false;
	isMousePressed = false;
	
	// add objects
	for (int i=0; i<100; i++) {
		// circulos
		float r = ofRandom(3, 6);		
		ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
		circle.get()->setPhysics(3.0, 0.53, 0.9);
		circle.get()->setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()/2, r);
		circle.get()->setFriction(0.9);
		circles.push_back(circle);

		// rectangulos
		float w = ofRandom(4, 10);	
		float h = ofRandom(w, 20);	
		ofPtr<ofxBox2dRect> rect = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
		rect.get()->setPhysics(3.0, 0.53, 0.9);
		rect.get()->setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()/2, w, h);
		boxes.push_back(rect);
	}
	
	bola.loadImage("images/dot.png");
	ladoPart1 = 5;
	ladoPart2 = 10;
	
	coloresBN.push_back(ofColor(0));
	coloresBN.push_back(ofColor(150));
	coloresBN.push_back(ofColor(255));
	
}

//--------------------------------------------------------------
void testApp::update() {
	
	box2d.update();	
	ofVec2f mouse(ofGetMouseX(), ofGetMouseY());
	float minDis = ofGetMousePressed() ? 300 : 200;

	ofVec2f center(ofGetWidth()/2.0, ofGetHeight()/2.0);
	
	
	// PONER FUERZAS DE:
	//  - REPULSION si dist-mouse < distMinima
	//  - ATRACCION si dist-mouse > distMaxima
	f(mouse
	if(isMousePressed) {
		for(int i=0; i<circles.size(); i++) {
			float dis = mouse.distance(circles[i].get()->getPosition());
			if(dis < minDis) circles[i].get()->addRepulsionForce(mouse, 1.5);//3, 9);
	//		if(dis < minDis) circles[i].get()->addRepulsionForce(mouse, ofMap(mouse.y, 0,ofGetHeight(), 2,8 ));//9);
			else circles[i].get()->addAttractionPoint(mouse, 3.0);//4.0);
		}
		for(int i=0; i<boxes.size(); i++) {
			float dis = mouse.distance(boxes[i].get()->getPosition());
			if(dis < minDis) boxes[i].get()->addRepulsionForce(mouse, 3.5);
			else boxes[i].get()->addAttractionPoint(mouse, 2.0);//4.0);
		}
	}
	
	// Fuerza de confinamiento
	float distConf = ofGetHeight()/2.0*0.9;
	
	for(int i=0; i<circles.size(); i++) {
		float dis = center.distance(circles[i].get()->getPosition());
		if(dis > distConf) circles[i].get()->addAttractionPoint(center, 0.9);//3, 9);
	}
	for(int i=0; i<boxes.size(); i++) {
		float dis = center.distance(boxes[i].get()->getPosition());
		if(dis > distConf) boxes[i].get()->addAttractionPoint(center, 0.9);//3, 9);
	}
	
}


//--------------------------------------------------------------
void testApp::draw() {
	
	ofEnableAlphaBlending();
	for(int i=0; i<circles.size(); i++) {
		ofFill();
//		ofSetHexColor(0xf6c738);
//		circles[i].get()->draw();
		
		ofVec2f pos = circles[i].get()->getPosition();
		float rr = 3*circles[i].get()->getRadius();
		ofPushMatrix();
		ofPushStyle();
		ofTranslate(pos.x,pos.y,0);
		ofSetColor(ofColor::royalBlue, 200);
		bola.draw(-rr,-rr,2*rr,2*rr);
		ofPopStyle();
		ofPopMatrix();
	}
	
	for(int i=0; i<boxes.size(); i++) {
		ofFill();
//		ofSetHexColor(0xBF2545);
//		boxes[i].get()->draw();
		
		ofVec2f pos = boxes[i].get()->getPosition();
		float rr = 3*boxes[i].get()->getHeight();
		ofPushMatrix();
		ofPushStyle();
		ofTranslate(pos.x,pos.y,0);
		ofSetColor(ofColor::red, 180);
		bola.draw(-rr,-rr,2*rr,2*rr);
		ofPopStyle();
		ofPopMatrix();
		
	}
	
	// draw the ground
	box2d.drawGround();
	
	ofDisableAlphaBlending();
	
	string info = "";
	info += "Press [c] for circles\n";
	info += "Press [b] for blocks\n";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	if(key == 'c') {
		float r = ofRandom(14, 20);		// a random radius 4px - 20px
		ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
		circle.get()->setPhysics(3.0, 0.53, 0.9);
		circle.get()->setup(box2d.getWorld(), mouseX, mouseY, r);
		circles.push_back(circle);
	}
	
	if(key == 'b') {
		float w = ofRandom(14, 20);	
		float h = ofRandom(14, 20);	
		ofPtr<ofxBox2dRect> rect = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
		rect.get()->setPhysics(3.0, 0.53, 0.9);
		rect.get()->setup(box2d.getWorld(), mouseX, mouseY, w, h);
		boxes.push_back(rect);
	}
	
	if(key == 't') ofToggleFullscreen();
	
	isKeyPressed = true;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
	isKeyPressed = false;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	isMousePressed = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	isMousePressed = false;
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
}

