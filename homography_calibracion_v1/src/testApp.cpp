#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	
	// carga imagenes
	left.loadImage("left.jpg");
	right.loadImage("right.jpg");
	imitate(warpedColor, right);
	
	// zonas donde se muestran las imagenes
	c1_Dst = ofRectangle(0,0, left.getWidth(), left.getHeight());
	c1_Src = ofRectangle(left.getWidth(),0, right.getWidth(), right.getHeight());
	
	// para control de puntos
	movingPoint = false;
	
	imgCalibr.loadImage("proyeccion_640x480.png");

}

void testApp::update() {

	// Esto deber’a hacerlo solo cuando cambie en algo la homografia
	// Por ahora no va con referencia a ofImage
	//	calib1.warpImage(right, warpedColor);
	if(calib1.homographyReady) {
		warpPerspective(right, warpedColor, calib1.homography, CV_INTER_LINEAR);
		warpedColor.update();
	}
	
	
}


void testApp::draw() {
	
	ofSetColor(255);
	left.draw(c1_Dst);
	right.draw(c1_Src);
	
	// Dibujar elementos homografia:
	// puntos SRC y DST

	
	
	if(calib1.homographyReady) {
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
		ofSetColor(255, 128);
		warpedColor.draw(c1_Dst);
		ofDisableBlendMode();
		

		// Bolillo del mouse
		if(c1_Dst.inside(mouseX, mouseY)) {
			ofSetColor(ofColor::lime);
			ofNoFill();
			ofEllipse(ofGetMouseX(), ofGetMouseY(), 15,15);
			
			// me pasa el punto desde Dst a Src
			// Este no va bien, pero antes s’!!!!
			ofVec2f proy = calib1.transf_Punto( ofVec2f(ofGetMouseX() - c1_Dst.x, ofGetMouseY() - c1_Dst.y), true ) ;
			ofSetColor(ofColor::green);
			ofPushMatrix();
				ofTranslate(c1_Src.x, c1_Src.y, 0);
				ofEllipse(proy.x, proy.y, 20,20);
			ofPopMatrix();
			
		}
		else if(c1_Src.inside(mouseX, mouseY)) {
			ofSetColor(ofColor::pink);
			ofNoFill();
			ofEllipse(ofGetMouseX(), ofGetMouseY(), 15,15);
			
			// me pasa el punto desde Src a Dst
			// Este va muy bien
			ofVec2f proy = calib1.transf_Punto( ofVec2f(ofGetMouseX() - c1_Src.x, ofGetMouseY() - c1_Src.y) , false);
			ofSetColor(ofColor::green);
			ofPushMatrix();
				ofTranslate(c1_Dst.x, c1_Dst.y, 0);
				ofEllipse(proy.x, proy.y, 20,20);
			ofPopMatrix();			
		}
		

		
	}
	
	// dibuja puntos y lineas que los unen
	calib1.drawPoints(c1_Dst, c1_Src);
	
	ofSetColor(255);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
	ofDrawBitmapString(ofToString(warpedColor.getWidth())+","+ofToString(warpedColor.getHeight()), 10, 40);
	
}




//bool testApp::movePoint(Calibrador ccc, ofVec2f point) {
bool testApp::movePoint(vector<ofVec2f>& points, ofVec2f point) {
	// devuelve true se hay un punto cerca de point
	// y guarda su referencia en curPoint
	//
	for(int i = 0; i < points.size(); i++) {
		if(points[i].distance(point) < 20) {
			movingPoint = true;
			curPoint = &points[i];
			return true;
		}
	}
	return false;
}

void testApp::mousePressed(int x, int y, int button) {
	ofVec2f cur(x, y);
	ofVec2f offsetDst = ofVec2f(c1_Dst.x, c1_Dst.y);
	ofVec2f offsetSrc = ofVec2f(c1_Src.x, c1_Src.y);
	
	// selecciona un punto de calibracion de los del area de interes 
	bool selectPtDst = movePoint(calib1.ptsDst, cur-offsetDst);
	bool selectPtSrc = movePoint(calib1.ptsSrc, cur-offsetSrc);
	
	if(!selectPtDst && !selectPtSrc) {
		// Si no selecciona ni un punto de los que ya hay en la calibracion
		// add puntos para calibrar
		
		// ver si esta en una zona de la imagen
		// Supongo tama–os de imagenes iguales!!!!
		if(c1_Dst.inside(cur)) {
			calib1.addPts(cur-offsetDst, cur-offsetDst);	
			// calib1 se encarga de updateHomogr
		}
		else if(c1_Src.inside(cur)) {
			calib1.addPts(cur-offsetSrc, cur-offsetSrc);
			// calib1 se encarga de updateHomogr
		}
		
		
	}
	
	else {
		// Si ha seleccionado algun punto anotar si es de Dst o de Src
		if(selectPtDst) isDstPt = true;
		else if(selectPtSrc) isDstPt = false;
	}
		
}

void testApp::mouseDragged(int x, int y, int button) {
	if(movingPoint) {
		if(isDstPt) curPoint->set(x-c1_Dst.x, y-c1_Dst.y);
		else		curPoint->set(x-c1_Src.x, y-c1_Src.y);
		
		// update homography
		// en calib1
		calib1.calcHomography();
	}
}

void testApp::mouseReleased(int x, int y, int button) {
	movingPoint = false;
	calib1.calcHomography();	
}

void testApp::keyPressed(int key) {
	if(key == 's') {
		calib1.saveMatrix("calib1");
	}
	else if(key=='l') {
		calib1.loadMatrix("calib1");
	}
	
	// GGGG reset
	if(key=='r') {
		calib1.reset();
	}
	
	
	if(key=='p') calib1.logHomogr();
}