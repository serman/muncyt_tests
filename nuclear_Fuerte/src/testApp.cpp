#include "testApp.h"
#include "utilsGeom.cpp"

//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(true);
	
	zentro = ofVec3f(ofGetWidth()/2.0,ofGetHeight()/2.0,0);
	
	swDifraccion = false;
	
	
	
	
	
	testPath.setFilled(false);
	testPath.setStrokeColor(ofColor::blueSteel);
	testPath.setColor(ofColor::blueSteel);	
	
	
//	camino.setClosed(true);
//	camino1.setClosed(true);
}

//--------------------------------------------------------------
void testApp::update(){
	// add alguna particula desde un lateral
	addParticleLateral();
	
	// update partics
	
	// aplicar fuerzas de interaccion part-part

	
	for(int i=0;i<particulas.size();i++) {
		
		// aplicar fuerzas globales
		
		
		// mover 
		particulas[i].update();
		
//		float rnd = ofRandom(1.0);
//		if(rnd<0.02) particulas[i].forceRadAround(zentro, 3.0, 300.0);
		
		
		// check si partic esta dentro o fuera de shillouette
		if(camino1.size()>0) {
			if(GeometryUtils::estaDentro(particulas[i].position, camino1)) {
				particulas[i].color = ofColor::red;
				particulas[i].setInside(true);	
			}
			else {
				particulas[i].color = particulas[i].color_orig; 
				particulas[i].setInside(false);	
			}
		}

		// check si partic choca con shillouette
		if(camino1.size()>0) {
			ofVec2f ptChoque = GeometryUtils::getIntersectionPoly(particulas[i].position_prev, particulas[i].position, camino1);
			if(ptChoque!=ofVec2f(0,0)) {
				ptsChoque.push_back(ptChoque);
			}
		}
	}
	
	
	// remove particulas marcadas para borrar (por ej que esten fuera de screen)
	for(int i=particulas.size()-1; i>0;i--) {
		if(particulas[i].position.x>ofGetWidth()+50) particulas.erase(particulas.begin()+i);
	}

	// Update emitter
	emitter.setPos_XY(mouseX-zentro.x, mouseY-zentro.y);

}


void testApp::addParticleLateral() {
	
	
	//	float rnd = ofRandom(1.0);
	//	if(rnd<0.15) {
	if(ofGetFrameNum()%20==0) {
		//		ParticleX( ofVec3f _position , ofColor _color, float mass = 1.0, float charge = 0.0 )
		//		ParticleX( ofVec3f _position , ofVec3f _vel , ofColor _color, float mass = 1.0, float charge = 0.0 )
		float v = ofMap(mouseX,0,ofGetHeight(), 4, 10);
		ParticleX p = ParticleX(ofVec3f(0,ofRandom(ofGetHeight()),0), ofVec3f(v,0,0), ofColor(255,255,255) );
		particulas.push_back(p);
		
		
		// pos, vel, color, mass, charge
//		ParticleX pTmp = ParticleX(ofVec3f(0,ofRandom(ofGetHeight())) , ofVec3f(ofRandom(10.0, 30.0), 0) , ofColor::white, 1.0, 0.0);
		
	}
	
	
}

//--------------------------------------------------------------
void testApp::draw(){

	ofBackground(ofColor::darkBlue);
	
	
	ofPushStyle();
	ofEnableSmoothing();
	ofEnableAntiAliasing();
	ofSetColor(ofColor::white, 200);
	camino.draw();
	ofSetColor(ofColor::lime, 60);
	ofSetLineWidth(15);
	camino1.draw();
	ofPopStyle();
	
	testPath.draw();

	for(int i=0;i<particulas.size();i++) {
		particulas[i].draw();
//		particulas[i].drawMemoPath();
	}
	
	
	ofPushStyle();
	for(int i=0;i<ptsChoque.size();i++) {
		ofCircle(ptsChoque[i].x, ptsChoque[i].y, 3);
//		ofDrawBitmapString(ofToString(ptsChoque[i].x)+","+ofToString(ptsChoque[i].y), ptsChoque[i]);
	}
	ofPopStyle();
	
	// 
	// NUCLEAR FUERTE
	// 
	// Escena centrada y simetr’a circular
	ofPushMatrix();
		ofTranslate(zentro.x, zentro.y, zentro.z);
		// Decorados
	
	
		// Emisor
		emitter.draw();
	
	ofPopMatrix();
	
	
	ofPushStyle();
	int hLin = 15; int dLin = 15;
	ofDrawBitmapString("'c' clear camino", 10,hLin); hLin+=dLin;
	ofDrawBitmapString("num pts camino1: " + ofToString(camino1.getVertices().size()), 10,hLin); hLin+=dLin;
	ofDrawBitmapString("num pts camino simpl: " + ofToString(camino.getVertices().size()), 10,hLin); hLin+=dLin;
	ofDrawBitmapString("Num Partics: " + ofToString(particulas.size()), 10,hLin); hLin+=dLin;
	ofDrawBitmapString("FR: " + ofToString(ofGetFrameRate()), 10,hLin); hLin+=dLin;
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	if(key=='c') {
		camino.clear();
		camino1.clear();
		ptsChoque.clear();
		
		testPath.clear();
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
	camino1.addVertex(x,y);

	
	testPath.lineTo(x+30*ofRandom(1.0),y);	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
//	if(camino.getVertices().size()>4) camino.curveTo(x,y);
//	else camino.addVertex(x,y);
	
	camino1.addVertex(x,y);
	
	testPath.newSubPath();
	testPath.moveTo(x+30,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

	camino = camino1;
	camino.simplify(5.2f);

	camino.setClosed(true);
	camino1.setClosed(true);

	
	
	testPath.close();

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