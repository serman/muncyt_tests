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
	numPointsXtra = 100;
	
	doTessel = false;	
	
	cargaColores();
	bFill = false;
	
	bDrawOld = false;
	
	setupGUI();
}

void testApp::setupGUI() {
    gui = new ofxUISuperCanvas("CONTROL");
    gui->addSpacer();
    gui->addLabel("_Control_", OFX_UI_FONT_MEDIUM);
    gui->addSpacer();
	gui->addToggle("Delaunay", &doTriang);
	gui->addToggle("Fill", &bFill);
    gui->addIntSlider("modoFill", 0, 2, &modoFill);
	gui->addToggle("Draw Old", &bDrawOld);
	gui->addToggle("Add Pts Xtra", &bAddPts);
    gui->addIntSlider("numPointsXtra", 0, 500, &numPointsXtra);
    gui->addSpacer();
	gui->addToggle("Path Tessel", &doTessel);
    gui->addSpacer();
    gui->addSlider("Treshold", 0, 255, thres);
    gui->addSpacer();
	
	

    gui->addSpacer();
    gui->addTextArea("TEXT AREA", "HIDE & SHOW GUI BY PRESSING 'g'. MOUSE OVER A SLIDER AND PRESS UP, DOWN, LEFT, RIGHT", OFX_UI_FONT_SMALL);
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
//    gui->loadSettings("guiSettings.xml");	
}

void testApp::cargaColores() {
	// paleta de colores fluor
	// http://www.colourlovers.com/palette/970972/FLUOR
	//
	colorFluor.clear();
//	colorFluor.push_back(ofColor::red);	// rojo
//	colorFluor.push_back(ofColor::green);	// green
//	colorFluor.push_back(ofColor::blue);	// blue
	colorFluor.push_back(ofColor::fromHex(0x0DE0FC));	// melting flowers
	colorFluor.push_back(ofColor::fromHex(0x38FC48));	// Dead Nuclear
	colorFluor.push_back(ofColor::fromHex(0xF938FC));	// Calcinha
	colorFluor.push_back(ofColor::fromHex(0xFA00AB));	// ow!
	colorFluor.push_back(ofColor::fromHex(0xDFFC00));	// Limei Green
	
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
						ofRectangle bounds = resampled.getBoundingBox();
						
						for(int i=0; i<numPointsXtra; i++) {
							float px = bounds.x+ofRandom(bounds.width);
							float py = bounds.y+ofRandom(bounds.height);
							if( resampled.inside(ofPoint(px, py)) ){
								triangulation.addPoint(ofPoint(px, py));
							}
						}
					}
					
					triangulation.triangulate();
					
					if(bSoloEnContorno) {
						
						triangContMesh_old = triangContMesh;
						
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
								// supongo el area de imagen dividido en 10 franjas divididas a su vez en 
								// colorFluor.size() bandas de distinto color
								float hBandaPpal = cam.height/10;
								float hBanda = hBandaPpal/colorFluor.size();
								
								ofColor ctmpa, ctmpb, ctmpc;
								if(modoFill==0) {
									// RANDOM
									ofColor ctmp = colorFluor[i%colorFluor.size()];
									ctmpa = ctmp;
									ctmpb = ctmp;
									ctmpc = ctmp;
								}
								else if(modoFill==1) {
									// asignar color segun la posicion del punto medio
										int nBanda = floor(pm.y/hBanda);
										ofColor ctmp = colorFluor[nBanda%colorFluor.size()];
										ctmpa = ctmp;
										ctmpb = ctmp;
										ctmpc = ctmp;
								}
								else if(modoFill==2) {
									int nBanda = floor(a.y/hBanda);
									ctmpa = colorFluor[nBanda%colorFluor.size()];
									nBanda = floor(b.y/hBanda);
									ctmpb = colorFluor[nBanda%colorFluor.size()];
									nBanda = floor(c.y/hBanda);
									ctmpc = colorFluor[nBanda%colorFluor.size()];
								}
								triangContMesh.addVertex(a);
								triangContMesh.addVertex(b);
								triangContMesh.addVertex(c);
								triangContMesh.addColor(ctmpa);
								triangContMesh.addColor(ctmpb);
								triangContMesh.addColor(ctmpc);
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
				if(bFill)	{
					triangContMesh.draw();
					if(bDrawOld) {
						ofEnableBlendMode(OF_BLENDMODE_ADD);
						triangContMesh_old.draw();
						ofDisableBlendMode();
					}
				}
				else		triangContMesh.drawWireframe();
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
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2,0);
	ofDrawBitmapString("fr: " + ofToString(ofGetFrameRate()), 10,10);
	ofDrawBitmapString("(mouseY+click) threshold: " + ofToString(thres), 10,25);
	ofDrawBitmapString("(q/a) learning rate: " + ofToString(lr), 10,40);
	ofDrawBitmapString("(t) Do Delaunay: " + ofToString(doTriang), 10,55);
	ofDrawBitmapString("   (c) Solo contorno: " + ofToString(bSoloEnContorno), 10,70);
	ofDrawBitmapString("   (x) Add xtra points: " + ofToString(bAddPts), 10,85);
	ofDrawBitmapString("(r) Do Tessell: " + ofToString(doTessel), 10,100);
	ofDrawBitmapString("(f) Fill Delaunay: " + ofToString(bFill), 10,120);
	ofPopMatrix();
	
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
	else if(key=='f') bFill=!bFill;
		
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
}


//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
//	thres = ofMap(mouseY,0,ofGetHeight(),0,255);
//	background.setThresholdValue(thres);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
//	thres = ofMap(mouseY,0,ofGetHeight(),0,255);
//	background.setThresholdValue(thres);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}	

// --------- GUI ----------

void testApp::guiEvent(ofxUIEventArgs &e)
{
	ofLogNotice("GUIEVENT");		
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	
	if(name == "Treshold")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		ofLogNotice("threshold event");		
		thres = slider->getScaledValue();
		background.setThresholdValue(thres);
	}
	else if(name == "xxnumPointsXtra")
	{
		ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget; 
		ofLogNotice("numPointsXtra event: " + ofToString(slider->getScaledValue()));
		numPointsXtra = slider->getScaledValue();
	}	
	else if(name == "xxDelaunay")
	{
		ofxUIToggle *slider = (ofxUIToggle *) e.widget; 
		doTriang = slider->getValue();
	}	
	else if(name == "xxFill")
	{
		ofxUIToggle *slider = (ofxUIToggle *) e.widget; 
		bFill = slider->getValue();
	}	
	else if(name == "xxmodoFill")
	{
		ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget; 
		modoFill = slider->getScaledValue();
	}	
	else if(name == "xxDraw Old")
	{
		ofxUIToggle *slider = (ofxUIToggle *) e.widget; 
		bDrawOld = slider->getValue();
	}	
	else if(name == "xxAdd Pts Xtra")
	{
		ofxUIToggle *slider = (ofxUIToggle *) e.widget; 
		bAddPts = slider->getValue();
	}	
	else if(name == "xxPath Tessel")
	{
		ofxUIToggle *slider = (ofxUIToggle *) e.widget; 
		doTessel = slider->getValue();
	}	
	
}
//--------------------------------------------------------------
void testApp::exit()
{
    gui->saveSettings("guiSettings.xml");
    delete gui; 
}

