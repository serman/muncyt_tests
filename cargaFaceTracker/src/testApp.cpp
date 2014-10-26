#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	swEnLinea = true;
	swWireframe = false;
	
	// cargar caras
	cargaCaras();
	
}

void testApp::cargaCaras() {
	string caraNombre = "capturas/imagen_0";
	cara.setup(caraNombre);
	caras.push_back(cara);
	

	for(int i=2; i<7; i++) {
		string caraNombre = "capturas/imagen_"+ofToString(i);
		Cara caraTmp;
		caraTmp.setup(caraNombre);
		caras.push_back(caraTmp);
	}
	
}


//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	ofBackground(0);
	ofNoFill();
	
	ofEnableDepthTest();
	
	float scaleX = ofMap(ofGetMouseX(), 0,ofGetWidth(), 0.0, 10.0);
	float scaleY = ofMap(ofGetMouseY(), 0,ofGetHeight(), 0.0, 10.0);
	float scaleZ = 2.0;
	
	int nCaras = caras.size();
	
	float zz = -600;
	
	if(swEnLinea) {
	// EN LINEA
		for(int i=0; i<nCaras; i++){
			ofPushMatrix();
	//		ofRotateY(360.0/nCaras*i);
			ofTranslate(ofGetWidth()/nCaras*(i+0.5), ofGetHeight()/2.0, 0);
			//	ofScale(scaleX,scaleY,1);
			ofRotateY(ofMap(ofGetMouseX(), 0,ofGetWidth(), -180, 180.0)*0.5 + 180);
			//	ofRotateX(ofMap(ofGetMouseY(), 0,ofGetHeight(), -180, 180.0)*0.5);
			ofScale(6,6,scaleZ);//scaleY);//1);
			ofSetColor(255);
			caras[i].img.bind();
			caras[i].meanObjMesh.drawFaces();
			caras[i].img.unbind();	
			if(swWireframe) caras[i].objMesh.drawWireframe();
			
			ofSetColor(0,255,0);
			//	ofLine(0,0,0,ofGetHeight());
			//	caraObjMesh.draw();
			//	caraMeanObjMesh.drawWireframe();
			ofPopMatrix();
		}
	}
	else {
	// EN CIRCULO QUE GIRA
		ofPushMatrix();
		ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0, zz);
//		ofRotateX(90);
		ofRotateY(ofGetElapsedTimeMillis()/100.0*0.5);
		for(int i=0; i<nCaras; i++){
			ofPushMatrix();
			ofRotateY(360.0/nCaras*i);
			ofTranslate(-zz, 0, 0);
			//	ofScale(scaleX,scaleY,1);
//			ofRotateY(ofMap(ofGetMouseX(), 0,ofGetWidth(), -180, 180.0)*0.5 + 180);
			//	ofRotateX(ofMap(ofGetMouseY(), 0,ofGetHeight(), -180, 180.0)*0.5);
			ofScale(6,6,scaleZ);//scaleY);//1);
//			ofRotateY(-360.0/nCaras*i+90);
			ofRotateY(-90);
			ofSetColor(255);
			caras[i].img.bind();
			caras[i].objMesh.drawFaces();
			caras[i].img.unbind();
			ofNoFill();
			if(swWireframe) caras[i].objMesh.drawWireframe();
			
			ofSetColor(0,255,0);
			//	ofLine(0,0,0,ofGetHeight());
			//	caraObjMesh.draw();
			//	caraMeanObjMesh.drawWireframe();
			ofPopMatrix();
		}		
		ofPopMatrix();
	}
	
	
	ofSetColor(255);
	ofLine(0,ofGetHeight()/2, ofGetWidth(),ofGetHeight()/2);
	ofLine(0,ofGetHeight()*5/6, ofGetWidth(),ofGetHeight()*5/6);
	ofLine(ofGetWidth()/2,0,ofGetWidth()/2,ofGetHeight());
	ofLine(ofGetWidth()/2,0,ofGetWidth()/2,ofGetHeight());
	
	ofDrawBitmapString("'m' para cambiar modo de presentaci—n", 20, 20);
	ofDrawBitmapString("'w' para dibujar wireframe", 20, 35);
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='m') swEnLinea=!swEnLinea;
	else if(key=='w') swWireframe=!swWireframe;
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
