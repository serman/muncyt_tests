#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0);
	
	zentro = ofVec2f(ofGetWidth()/2.0, ofGetHeight()/2.0);
	
	ofEnableDepthTest();
	
	setupFig();

	setupFig1();
	
	setupPts();
}

void testApp::setupFig() {
	float radio = 30.0;
	ofMesh mesh;
	int nn = 3;
	for(int i=0; i<nn; i++) {
		float angT = (float) TWO_PI/nn*i;
		ofVec3f vtx(radio*cos(angT), radio*sin(angT), 0);
		mesh.addVertex(vtx);
		float hh = (float) 255/nn*i;
		ofColor cc = ofColor::fromHsb(hh, 255,255,255);
		ofFloatColor fcc((float)cc.r/255.0f, (float)cc.g/255.0f, (float)cc.b/255.0f, (float)cc.a/255.0f);
		mesh.addColor(fcc);
	}
	
//	mesh.setMode(OF_PRIMITIVE_POINTS);
	
	figVbo.setMesh(mesh, GL_STATIC_DRAW);
}

void testApp::setupFig1() {
	float radio = 30.0;
	ofMesh mesh;
	int nn = 3;
	for(int i=0; i<nn; i++) {
		float angT = (float) TWO_PI/nn*i;
		ofVec3f vtx(radio*cos(angT), radio*sin(angT), 0);
		mesh.addVertex(vtx);
		float hh = (float) 255/nn*i;
		ofColor cc = ofColor::fromHsb(hh, 255,255,255);
		ofFloatColor fcc((float)cc.r/255.0f, (float)cc.g/255.0f, (float)cc.b/255.0f, (float)cc.a/255.0f);
		mesh.addColor(fcc);
	}
	
	//	mesh.setMode(OF_PRIMITIVE_POINTS);
	
	fig1Vbo.setMesh(mesh, GL_STATIC_DRAW);
}

void testApp::setupPts() {
	// pner muchisimos puntos con colores RGB
	// unir solo si distancia es < dMin
	// luego representarlos pero dibujando solo una parte
	// Cada frame cambia esa parte
	
	int nPts = 1000;
	ofMesh mesh;
	for(int i=0; i<nPts; i++) {
		float angT = (float) TWO_PI/60*i;
		float radio = zentro.y / nPts * i;
		ofVec3f vtx(radio*cos(angT), radio*sin(angT), 0);
		mesh.addVertex(vtx);
		float hh = (float) 255/nPts*i;
		ofColor cc = ofColor::fromHsb(hh, 255,255,255);
		ofFloatColor fcc((float)cc.r/255.0f, (float)cc.g/255.0f, (float)cc.b/255.0f, (float)cc.a/255.0f);
		mesh.addColor(fcc);
	}
	
	ptsVbo.setMesh(mesh, GL_STATIC_DRAW);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	
	
	ofSetColor(0,0,255);
	ofFill();
	figVbo.draw(GL_LINE_LOOP,0,3);
	
	ofPushMatrix();
	ofTranslate(zentro.x, zentro.y, 0);
	figVbo.draw(GL_TRIANGLES,0,3);
	
	
	ptIniAct  = floor(ofMap(ofGetMouseX(), 0, ofGetWidth(),  0, ptsVbo.getNumVertices()-1) );
	nPtsDraw  = floor(ofMap(ofGetMouseY(), 0, ofGetHeight(), 0, ptsVbo.getNumVertices()) );
	ptsVbo.draw(GL_LINES,ptIniAct,nPtsDraw);//ptsVbo.getNumVertices());
	
	ofPopMatrix();

	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

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
