#include "testApp.h"
#include "utilsGeom.cpp"

//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(true);
	
	zentro = ofVec3f(ofGetWidth()/2.0,ofGetHeight()/2.0,0);
	
	swDifraccion = false;
	
	ratePartic = 20;
	
	bDrawCaminos = true;
	bDrawPtosChoque = true;
	bTiltCamino = false;	
	
	
	testPath.setFilled(false);
	testPath.setStrokeColor(ofColor::blueSteel);
	testPath.setColor(ofColor::blueSteel);	
	
	
//	camino.setClosed(true);
//	camino1.setClosed(true);
	
	setupGUI();
}

//--------------------------------------------------------------
void testApp::update(){
	// add alguna particula desde un lateral
	addParticleLateral();
	
	// cambiar camino
	if(bTiltCamino) tiltCamino();
	
	// update partics
	
	// aplicar fuerzas de interaccion part-part

	
	for(int i=0;i<particulas.size();i++) {
		
		// aplicar fuerzas globales
		if(swMagnetField && particulas[i].inside) {
			particulas[i].forceMagnetZ(magnetField);
		}
		
		
		
		// mover 
		particulas[i].update();
		
//		float rnd = ofRandom(1.0);
//		if(rnd<0.02) particulas[i].forceRadAround(zentro, 3.0, 300.0);
		
		
		
		// COLISION CON CAMINO
		if(camino.size()>0) {
			bool bCruce = false;
			if(GeometryUtils::estaDentro(particulas[i].position, camino)) {
				particulas[i].color = ofColor::red;
				bCruce = particulas[i].setInside(true);	
			}
			else {
				particulas[i].color = particulas[i].color_orig; 
				bCruce = particulas[i].setInside(false);	
			}
			
			if(bCruce) {
				// check si partic choca con shillouette

				ofVec2f ptChoque = GeometryUtils::getIntersectionPoly(particulas[i].position_prev, particulas[i].position, camino);
				if(ptChoque!=ofVec2f(0,0)) {
					ptsChoque.push_back(ptChoque);
					
					// add el punto al camino de la particula
					particulas[i].insertPtChoque(ptChoque);
					
					// recalcular la direccion de la particula.
//					ofVec2f dirI = particulas[i].position - ptChoque;
//					ofVec2f dirI = particulas[i].velocity;
					// rotate random
					float angRotMax = 15;
					float angRot =  ofRandom(-angRotMax, angRotMax);
					ofColor cc = ofColor::fromHsb(ofMap(abs(angRot), 0, angRotMax, 0,255), 255, 255, 100);
					particulas[i].colorExcited = cc;
					particulas[i].velocity.rotate(angRot, ofVec3f(0,0,1));
					
				}
			}
		}
		
		
	}
	
	
	// remove particulas marcadas para borrar (por ej que esten fuera de screen)
	for(int i=particulas.size()-1; i>0;i--) {
		if( particulas[i].position.x>(ofGetWidth()+50) ){
			
			particulas.erase(particulas.begin()+i);
		}
	}

	// Update emitter
	emitter.setPos_XY(mouseX-zentro.x, mouseY-zentro.y);

}

void testApp::tiltCamino() {
	ofPolyline pl = camino;
	camino.clear();
	
	float maxDespl = 2.0;
	for(int i=0; i<pl.size(); i++) {
		camino.addVertex( pl[i]+ofVec2f(ofRandom(-maxDespl, maxDespl), ofRandom(-maxDespl, maxDespl)) );
	}
	camino.close();
	
	
}


void testApp::addParticleLateral() {
	
	//	float rnd = ofRandom(1.0);
	//	if(rnd<0.15) {
	if(ofGetFrameNum()%ratePartic==0) {
		//		ParticleS( ofVec3f _position , ofColor _color, float mass = 1.0, float charge = 0.0 )
		//		ParticleS( ofVec3f _position , ofVec3f _vel , ofColor _color, float mass = 1.0, float charge = 0.0 )
		float v = ofMap(mouseX,0,ofGetHeight(), 4, 10);
		float mass = 1+ofRandom(5);
		float carga = floor(ofRandom(10))-5;	// de -5 a 5
		ParticleS p = ParticleS(ofVec3f(0,ofRandom(ofGetHeight()),0), ofVec3f(v,0,0), ofColor(255,255,255), mass, carga );
		particulas.push_back(p);
		
		
		// pos, vel, color, mass, charge
//		ParticleS pTmp = ParticleS(ofVec3f(0,ofRandom(ofGetHeight())) , ofVec3f(ofRandom(10.0, 30.0), 0) , ofColor::white, 1.0, 0.0);
		
	}
}

//--------------------------------------------------------------
void testApp::draw(){

	ofBackground(ofColor::black);
	
	//
	//
	// PARTICULAS 
	//
	
	ofEnableSmoothing();
	ofEnableAntiAliasing();

	if(bDrawCaminos) {
		ofPushStyle();
		ofSetColor(ofColor::lime, 60);
		ofSetLineWidth(15);
		camino.draw();
		ofSetColor(ofColor::white, 200);
		ofSetLineWidth(1);
		camino1.draw();
		ofPopStyle();
		
		testPath.draw();
	}
	
	for(int i=0;i<particulas.size();i++) {
		particulas[i].draw();
//		particulas[i].drawMemoPath();
	}
	
	if(bDrawPtosChoque) {
		ofPushStyle();
		for(int i=0;i<ptsChoque.size();i++) {
			ofCircle(ptsChoque[i].x, ptsChoque[i].y, 3);
	//		ofDrawBitmapString(ofToString(ptsChoque[i].x)+","+ofToString(ptsChoque[i].y), ptsChoque[i]);
		}
		ofPopStyle();
	}
	
	
	
	// 
	// NUCLEAR FUERTE
	// 
	// Escena centrada y simetr�a circular
	ofPushMatrix();
		ofTranslate(zentro.x, zentro.y, zentro.z);
		// Decorados
	
	
		// Emisores
		emitter.draw();
		for(int i=0; i<emitters.size(); i++) {
			emitters[i].draw();
		}
	
	ofPopMatrix();
	
	
	//
	// INFO
	// 
	ofPushStyle();
	int hLin = ofGetHeight()-25; int dLin = -15;
	ofDrawBitmapString("'c' clear camino", 10,hLin); hLin+=dLin;
	ofDrawBitmapString("num pts camino1: " + ofToString(camino1.getVertices().size()), 10,hLin); hLin+=dLin;
	ofDrawBitmapString("num pts camino simpl: " + ofToString(camino.getVertices().size()), 10,hLin); hLin+=dLin;
	ofDrawBitmapString("Num Partics: " + ofToString(particulas.size()), 10,hLin); hLin+=dLin;
	ofDrawBitmapString("p clear partics", 10,hLin); hLin+=dLin;
	ofDrawBitmapString("o clear emitters", 10,hLin); hLin+=dLin;
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
	else if(key=='d') swDifraccion=!swDifraccion;
	else if(key=='q') ratePartic++;
	else if(key=='a') ratePartic = (ratePartic>2)? ratePartic-1 : 1;
	else if(key=='p') {
		particulas.clear();
	}
	else if(key=='o') {
		emitters.clear();
	}
	else if(key=='e') bDrawCaminos=!bDrawCaminos;
	else if(key=='r') bDrawPtosChoque=!bDrawPtosChoque;
	else if(key=='t') bTiltCamino=!bTiltCamino;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if(button==0) {		
		camino1.addVertex(x,y);
		
		testPath.lineTo(x-15+30*ofRandom(1.0),y);	
	}
	else if(button==2) {
		// mover el �ltimo emitter
		ofVec2f posTmp(x,y);
		posTmp -= zentro;
//		float rhoTmp = posTmp.length();
//		float angTmp = atan2(posTmp.y, posTmp.x);
//		emitters[emitters.size()-1].setPos_Radial(rhoTmp, angTmp);
		emitters[emitters.size()-1].setPos_XY(mouseX-zentro.x, mouseY-zentro.y);

	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
//	if(camino.getVertices().size()>4) camino.curveTo(x,y);
//	else camino.addVertex(x,y);
	
	if(button==0) {
		camino1.addVertex(x,y);
		
		testPath.newSubPath();
		testPath.moveTo(x+30,y);
	}
	else if(button==2) {
		// add nuevo emitter
		// 
		Emisor emTmp;
		ofVec2f posTmp(x,y);
		posTmp -= zentro;
		float rhoTmp = posTmp.length();
		float angTmp = atan2(posTmp.y, posTmp.x);
//		emTmp.setPos_Radial(rhoTmp, angTmp);
		emTmp.setPos_XY(mouseX-zentro.x, mouseY-zentro.y);
		
		ofColor cTmp = ofColor::fromHsb(angTmp*RAD_TO_DEG, 255, 255, 255);
		emTmp.setColor(cTmp);
		
		emitters.push_back(emTmp);
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

	if(button==0) {
		camino = camino1;
		camino.simplify(5.2f);

		camino.setClosed(true);
		camino1.setClosed(true);

		
		testPath.close();
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



// - - - - GUI - - - - 
void testApp::setupGUI() {
	
	gui1 = new ofxUISuperCanvas("STRONG NUCLEAR");
    gui1->addSpacer();
	//    gui1->addLabel("Press 'h' to Hide GUIs", OFX_UI_FONT_SMALL);
    
	//    gui1->addSpacer();
	gui1->addLabel("Magnetic Field");
	gui1->addToggle("Magnetic Field ON/OFF", &swMagnetField );
	gui1->addSlider("Magnetic Force", -5, 5.0, &magnetField);
	
	gui1->addSpacer();
	gui1->addToggle("(d) Difracci�n ON/OFF", &swDifraccion );
	gui1->addToggle("(e) Draw Camino ON/OFF", &bDrawCaminos );
	gui1->addToggle("(r) Draw Colisiones ON/OFF", &bDrawPtosChoque );
	gui1->addToggle("(t) Tilt Camino ON/OFF", &bTiltCamino );

//	ofDrawBitmapString("'c' clear camino", 10,hLin); hLin+=dLin;
//	ofDrawBitmapString("p clear partics", 10,hLin); hLin+=dLin;
	//	
	gui1->addSpacer();
	gui1->addIntSlider("(q/a)_Rate_partics", 1,120, &ratePartic);
	gui1->addSpacer();
	
    gui1->autoSizeToFitWidgets();
	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);	
}

void testApp::guiEvent(ofxUIEventArgs &e) {
	string name = e.getName();
	int kind = e.getKind();
	//	cout << "got event from: " << name << endl;
//	if(name == "radio_Sol")
//	{
//		ofxUISlider *slider = (ofxUISlider *) e.widget;
//		rSol = slider->getScaledValue();
//		sol.setRadius(rSol);
//		setMfromR();
//	}
//	else if(name == "cam_z")
//	{
//		ofxUISlider *slider = (ofxUISlider *) e.widget;
//		zCam = slider->getScaledValue();
//		cam.setDistance(zCam);
//	}
//	else if(name == "TOGGLE_OrthoCamera")
//	{	// swOrtho
//		ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
//		swOrtho = toggle->getValue();
//		if(swOrtho) cam.enableOrtho();
//		else cam.disableOrtho();
//	}
	
}


void testApp::exit()
{
//    gui1->saveSettings("gui1Settings.xml");    
//	delete gui1;
	
	//    float *buffer;
	//    ofImage *img;
	
	//	delete[] buffer;
	//    delete img;
	
	// borrar
	// - emitters
	// - particles
	
}
