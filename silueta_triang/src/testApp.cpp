#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	cam.initGrabber(640, 480);
	background.setLearningTime(900);
	background.setThresholdValue(10);
	
	thres = 25;
	background.setThresholdValue(thres);
	lr = 0.0001;
	
	contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(200);
	contourFinder.setSortBySize(true);
	
	// Triangulacion
	bAddPts = false;
	bSoloEnContorno = true;
	triangulation.reset();
	doTriang = true;
	
	doTessel = false;
	
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		background.update(cam, thresholded);
		thresholded.update();

		// test para saber que es getForeground y getBackground
//		toOf(background.getBackground(), bck);
//		toOf(background.getForeground(), foreg);
		
		
		// hallar contorno
//		contourFinder.setThreshold(ofMap(mouseX, 0, ofGetWidth(), 0, 255));

		contourFinder.findContours(thresholded);
		if(contourFinder.size()>0) {
			
		// Pasar los puntos del contorno a la triangulacion
			ofPolyline contSimple = contourFinder.getPolyline(0);
			contSimple.simplify();
			ofPolyline resampled = contSimple.getResampledBySpacing(30);

			if(resampled.size()>=3) {
				if(doTriang) {
					triangulation.reset();
					for(int i=0; i<resampled.size(); i++) {
						triangulation.addPoint(resampled[i]);
					}
					
					// add algunos puntos extra
					if(bAddPts) {
						int nPtsRnd = 100;//floor(ofRandom(10,30));
						ofRectangle bounds = resampled.getBoundingBox();
						
						for(int i=0; i<nPtsRnd; i++) {
							float px = bounds.x+ofRandom(bounds.width);
							float py = bounds.y+ofRandom(bounds.height);
							if( resampled.inside(ofPoint(px, py)) ){
								triangulation.addPoint(ofPoint(px, py));
							}
						}
					}
					
					triangulation.triangulate();
					
					if(bSoloEnContorno) {
						// Si no va, quitar triangs cuyo centro este fuera del contorno.
						triangContMesh.clear();
						triangContMesh.setMode(OF_PRIMITIVE_TRIANGLES);
						ofMesh tt = triangulation.triangleMesh;
						
						// Con vertex: no queda bien. Se nota la implementacion del algoritmo en vertical, o algo asi
//						vector<ofVec3f> verts = tt.getVertices();
//						for(int i=0; i<verts.size()-2; i++) {
//							ofVec3f a = verts[i];
//							ofVec3f b = verts[i+1];
//							ofVec3f c = verts[i+2];
//							ofVec3f pm = (a+b+c)/3.0;
//							
//							if( resampled.inside(ofPoint(pm.x,pm.y)) ) {
//								triangContMesh.addVertex(a);
//								triangContMesh.addVertex(b);
//								triangContMesh.addVertex(c);
//							}
//						}
						
						// con faces: se deberia aprovechar el efecto de triangulacion
						vector<ofMeshFace> faces = tt.getUniqueFaces();
						for(int i=0; i<faces.size(); i++) {
							ofVec3f a = faces[i].getVertex(0);
							ofVec3f b = faces[i].getVertex(1);
							ofVec3f c = faces[i].getVertex(2);
							ofVec3f pm = (a+b+c)/3.0;
							
							if( resampled.inside(ofPoint(pm.x,pm.y)) ) {
								triangContMesh.addVertex(a);
								triangContMesh.addVertex(b);
								triangContMesh.addVertex(c);
							}
						}
					}
					
				}
				
				// Probar con ofTessellator
				if(doTessel) {
					contornoTes.clear();
					tessel.tessellateToMesh(resampled, OF_POLY_WINDING_ODD, contornoTes);
				}
			}
		}
		else {
			triangulation.reset();
		}
		

		
		
	}
}

void testApp::draw() {
	cam.draw(0, 0);
	thresholded.draw(640, 0);
	
	// test para saber que es getForeground y getBackground
//	bck.draw(ofGetWidth()/2-320/2, 0,  320, 240);
//	foreg.draw(ofGetWidth()/2-320/2, 240,  320, 240);
	
	// dibujar contornos
	if(contourFinder.size()>0) {
		ofPushStyle();
			ofPolyline contSimple = contourFinder.getPolyline(0);
			contSimple.simplify();
			ofPolyline resampled = contSimple.getResampledBySpacing(25);
			ofSetColor(ofColor::darkRed);
			resampled.draw();
			ofDrawBitmapString(ofToString(resampled.size()), resampled.getCentroid2D()+ofPoint(0,0));
		ofPopStyle();
		
		// triangulacion
		if(doTriang) {
			ofNoFill();
			ofSetColor(ofColor::wheat);
			ofNoFill();
			if(!bSoloEnContorno) {
				triangulation.draw();
			}
			else {
				triangContMesh.drawWireframe();
				// si se quiere filled:
				// triangContMesh.draw();
			}
			// tambien se puede dibujar con Mesh
			// Mola porque se puede texturizar y aplicar vertex_shaders
			// ofMesh triangulation.triangleMesh;
		}
		
		// test con tessellator
		if(doTessel) {
			ofNoFill();
			ofSetColor(ofColor::indigo);
			contornoTes.setMode(OF_PRIMITIVE_LINE_LOOP);
			contornoTes.draw();
		}
		
		
	}
	
	ofSetColor(255);
	ofDrawBitmapString("fr: " + ofToString(ofGetFrameRate()), 10,10);
	ofDrawBitmapString("(mouseY+click) threshold: " + ofToString(thres), 10,25);
	ofDrawBitmapString("(q/a) learning rate: " + ofToString(lr), 10,40);
	ofDrawBitmapString("(t) Do Delaunay: " + ofToString(doTriang), 10,55);
	ofDrawBitmapString("   (c) Solo contorno: " + ofToString(bSoloEnContorno), 10,70);
	ofDrawBitmapString("   (x) Add xtra points: " + ofToString(bAddPts), 10,85);
	ofDrawBitmapString("(r) Do Tessell: " + ofToString(doTessel), 10,100);
	
}



void testApp::keyPressed(int key) {
	if(key == ' ') {
		background.reset();
	}
	if(key == 'v') {
		cam.videoSettings();
	}
	if(key=='q') {
		lr+=0.00005;
		background.setLearningRate(lr);
	}
	else if(key=='a') {
		lr-=0.00005;
		background.setLearningRate(lr);
	}
	else if(key=='t') doTriang=!doTriang;
	else if(key=='r') doTessel=!doTessel;
	else if(key=='c') bSoloEnContorno=!bSoloEnContorno;
	else if(key=='x') bAddPts=!bAddPts;
		
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	thres = ofMap(mouseY,0,ofGetHeight(),0,255);
	background.setThresholdValue(thres);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	thres = ofMap(mouseY,0,ofGetHeight(),0,255);
	background.setThresholdValue(thres);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}	


