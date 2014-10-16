#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableSmoothing();
    ofBackground(0);
    
	bFill = true;
	
	setupColores();
	myMesh.clear();
	
	puntos.clear();
	vels.clear();
	
	// poner puntos en circulo
	nPuntosBase=0;
	zentro = ofVec2f(ofGetWidth()/2.0, ofGetHeight()/2.0);
	radio = ofGetHeight()/2.0*0.8;
	for(int i=0; i<60; i++) {
		float ang=(float)TWO_PI/60.0*i;
		puntos.push_back(ofPoint(zentro.x+radio*cos(ang), zentro.y+radio*sin(ang)));
		vels.push_back(ofPoint(0,0));
		
		nPuntosBase++;
	}
	// poner cuadricula de puntos
	int nLado = 11;
	float lado = 2*radio;
	for(int j=0; j<nLado; j++) {
		for(int i=0; i<nLado; i++) {
			ofPoint pTmp = ofPoint(-lado/2+i*lado/nLado,-lado/2+j*lado/nLado);
			if(pTmp.length()<radio) {
				pTmp+=zentro;
				puntos.push_back(pTmp);
				vels.push_back(ofPoint(0,0));
				nPuntosBase++;
			}
		}
	}
	
	
//	int nPtsMoviles = 1;
//	for(int i=0; i<nPtsMoviles; i++) {
	addPart();
//	}
	
//	radio1 = ofGetHeight()/2.0*0.25;
//	for(int i=0; i<60; i++) {
//		float ang=(float)TWO_PI/60.0*i;
//		puntos.push_back(ofPoint(zentro.x+radio1*cos(ang), zentro.y+radio1*sin(ang)));
//		vels.push_back(ofPoint(ofRandom(-2,2), ofRandom(-2,2)) );
//	}
	
	triangulation.addPoints(puntos);
	triangulation.triangulate();
	addColors();
}

void testApp::addColors() {
    int npts = triangulation.triangleMesh.getVertices().size();
	int nColors = triangulation.triangleMesh.getColors().size();
	for(int i=0; i<npts; i++) {
		ofColor cc = coloresAll[i%6];
		cc.a = ofMap(ofGetMouseX(),0,ofGetWidth(), 0,255);
		ofFloatColor fc = ofFloatColor((float)cc.r/255.0, (float)cc.g/255.0, (float)cc.b/255.0);
		triangulation.triangleMesh.addColor(cc);
	}
}	

void testApp::addPart() {
	float radio1 = radio*0.5;
	float ang=ofRandom(TWO_PI);
	puntos.push_back(ofPoint(zentro.x+radio1*cos(ang), zentro.y+radio1*sin(ang)));
	vels.push_back(ofPoint(ofRandom(-4,4), ofRandom(-4,4)) );
	
}

void testApp::removeParts() {
	if(puntos.size()>nPuntosBase) {
		puntos.erase(puntos.begin()+nPuntosBase, puntos.end());
		vels.erase(vels.begin()+nPuntosBase, vels.end());
		
	}
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	if(puntos.size()>nPuntosBase) {
		triangulation.reset();
		
		// mover puntos
		for(int i=nPuntosBase;i<puntos.size();i++) {
			puntos[i] += vels[i];
			ofPoint pt = puntos[i]-zentro;
			if(pt.length()>radio) {
				float angPos = atan2(pt.y, pt.x);
				puntos[i] = ofPoint(radio*0.95*cos(angPos), radio*0.95*sin(angPos));
				puntos[i]+=zentro;
				// angulo entre veloc y nueva posic
				float angRel = pt.angle(vels[i]); // degrees (-180,180)
				vels[i].rotate(2*angRel, ofVec3f(0,0,1));// = ofPoint(-vels[i].y, -vels[i].x);				
				
//				vels[i] = ofPoint(-vels[i].y, -vels[i].x);				
			}
		}
		
		// update Triang
		triangulation.addPoints(puntos);
		triangulation.triangulate();
		addColors();		
	
	}

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(255);

	
	ofEnableAlphaBlending();
	
	ofPushStyle();
	ofSetColor(255,0,0, 30);
	ofSetLineWidth(10);
	ofLine(0,0, zentro.x, zentro.y);
	ofSetColor(255,0,255);
	ofSetLineWidth(10);
	ofLine(zentro.x, zentro.y, ofGetWidth(), ofGetHeight());
	ofPopStyle();
	
//    ofNoFill();
	if(bFill)	ofFill();
	else		ofNoFill();
    triangulation.draw();
	ofDisableAlphaBlending();
	
	ofSetColor(80);
    ofDrawBitmapString("'r' to reset", ofPoint(10,20));
    ofDrawBitmapString("'f' to fill: "+ofToString(bFill), ofPoint(10,40));
    ofDrawBitmapString("fr: "+ofToString(ofGetFrameRate()), ofPoint(10,60));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 'r'){
        triangulation.reset();
    }
    if(key == 'f'){
        bFill = !bFill;
    }
	if(key == 'd'){
		removeParts();
	}
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(button==0) 	addPart();
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

	if(button==2) {
		puntos.push_back(ofPoint(x,y));
		int npts = triangulation.addPoint(ofPoint(x,y));
		triangulation.triangulate();
		addColors();
	}
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}



void testApp::setupColores() {
	colores[0] = ofColor::red; 
	colores[1] = ofColor::green;
	colores[2] = ofColor::blue;
	antiColores[0] = colores[0]; antiColores[0].invert();
	antiColores[1] = colores[1]; antiColores[1].invert();
	antiColores[2] = colores[2]; antiColores[2].invert();
	for(int i=0; i<3; i++) {
		coloresAll[i] = colores[i];
		coloresAll[i+3] = antiColores[i];
	}
}
