#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	swEnLinea = true;
	swWireframe = false;
	swTexture = true;
	
	modo = ENLINEA;
	
	// cargar caras
	cargaCaras();
	
	ftracker.setup();
	
	tMuestraCara = 1000.0;
	nCaraAct = 0;
	
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

	
	
	// Analizar FT
	if( modo == FT2D) {
	   // Cambio de cara en FT
		if(	tMuestraCara < (ofGetElapsedTimeMillis()-tLastCara) ) {
			tLastCara = ofGetElapsedTimeMillis();
			nCaraAct++;
			nCaraAct%=caras.size();
			
		}
		
		
		// Analizar en FT
		ftracker.update(ofxCv::toCv(caras[nCaraAct].img));
	}
		
		  
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
	
//	if(swEnLinea) {
	if(modo == ENLINEA) {
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
			if(swTexture) {
				caras[i].img.bind();
				caras[i].meanObjMesh.drawFaces();
				caras[i].img.unbind();	
			}
			if(swWireframe) caras[i].objMesh.drawWireframe();
			
			ofSetColor(0,255,0);
			//	ofLine(0,0,0,ofGetHeight());
			//	caraObjMesh.draw();
			//	caraMeanObjMesh.drawWireframe();
			ofPopMatrix();
		}
	}
	else if(modo==CIRCULO) {
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
			ofScale(6,6,6);//scaleZ);//scaleY);//1);
//			ofRotateY(-360.0/nCaras*i+90);
			ofRotateY(-90);
			ofSetColor(255);
			if(swTexture) {
				caras[i].img.bind();
				caras[i].objMesh.drawFaces();
				caras[i].img.unbind();
			} 
			ofNoFill();
			if(swWireframe) {
//				caras[i].objMesh.drawWireframe();
				
//				caras[i].objVbo.draw(GL_LINES, 0, caras[i].objVbo.getNumVertices());
				caras[i].objVbo.draw(GL_LINES, ofGetFrameNum()%caras[i].objVbo.getNumVertices(), 30);
			}
			
			ofSetColor(0,255,0);
			//	ofLine(0,0,0,ofGetHeight());
			//	caraObjMesh.draw();
			//	caraMeanObjMesh.drawWireframe();
			ofPopMatrix();
		}		
		ofPopMatrix();
	}
	else if(modo==FT2D) {
		// Dibujar imagen y FT
		int ladoCara = 500;
		int px = (ofGetWidth()-ladoCara)/2.0;
		int py = (ofGetHeight()-ladoCara)/2.0;
		
		ofPushMatrix();
		ofPushStyle();
		ofTranslate(px,py,0);
		caras[nCaraAct].img.draw(0,0, ladoCara, ladoCara);
		ofSetColor(255,255,0);
		ofSetLineWidth(1.5);
		ofDisableDepthTest();
		ftracker.draw(true);
		ofEnableDepthTest();
		ofPopStyle();
		ofPopMatrix();
	}
	
	ofSetColor(255);
	ofLine(0,ofGetHeight()/2, ofGetWidth(),ofGetHeight()/2);
	ofLine(0,ofGetHeight()*5/6, ofGetWidth(),ofGetHeight()*5/6);
	ofLine(ofGetWidth()/2,0,ofGetWidth()/2,ofGetHeight());
	ofLine(ofGetWidth()/2,0,ofGetWidth()/2,ofGetHeight());
	
	ofDrawBitmapString("'m' para cambiar modo de presentaci—n", 20, 20);
	ofDrawBitmapString("'w' para dibujar wireframe", 20, 35);
	ofDrawBitmapString("'t' para dibujar texturas", 20, 50);
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
//	if(key=='m') swEnLinea=!swEnLinea;
	if(key=='m') {modo++;modo%=3;}
	else if(key=='w') swWireframe=!swWireframe;
	else if(key=='t') swTexture=!swTexture;
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
